#include "core/AppClient.h"
#include <time.h>
#include "Items.h"
#include "Player.h"
#include "Map.h"
#include "MapList.h"
#include "core/Behavior.h"
#include "core/Weapon.h"
#include "core/Score.h"
#include "core/Inventory.h"
#define MaxEntities 5
struct AppClient
{
    SDL_bool *running;
    State state;

    Graphics *gfx;
    Audio *audio;
    Font *font;
    Gui *gui;
    Camera *camera;
    Clock *clock;
    FPSManager *fpsManager;
    Input *input;
    Menu *menu;

    Vec2 middleOfMap;

    //UDP stuff (client, server messages etc.)
    UDPClient *client;
    SDL_Thread *listenThread;
    SDL_Thread *updateThread;
    UDPManager udpManager;

    GroundListItems groundListItems;

    EntityManager *entityManager;
    Player player; // player == entity 0

    Map map;
    MapList mapList;
};
void ListenToServer(void *args)
{
    UDPClient *client = (UDPClient *)args;
    while (client->isActive)
    {
        //SDL_Delay(5);
        if (client->hasPacket)
            continue;
        UDPClientListen(client, MAX_MSGLEN);
    }
}
void UpdateFromServer(void *args)
{
    AppClient *app = (AppClient *)args;
    while (app->client->isActive)
    {
        //SDL_Delay(5);
        UDPManagerUpdate(&app->udpManager, app->client, app->entityManager);
    }
}
AppClient *AppClientCreate(SDL_bool *running, Clock *clock, Input *input, UDPClient *client, FPSManager *fpsManager)
{

    srand(time(NULL));
    CursorInitialize();

    AppClient *app = (AppClient *)SDL_malloc(sizeof(AppClient));
    app->running = running;
    app->state = StateCreate();
    app->clock = clock;
    app->fpsManager = fpsManager;
    app->gfx = GraphicsCreate();
    app->audio = AudioCreate();
    app->font = FontCreate(app->gfx);
    app->gui = GuiCreate(app->font, app->clock);
    app->camera = CameraCreate(app->gfx, NULL);
    app->input = input;
    app->entityManager = EntityManagerCreate();
    app->menu = MenuCreate(app->gfx, app->entityManager, app->font, &app->state);
    app->player = PlayerCreate(app->camera, app->entityManager);
    app->middleOfMap = Vec2Create((float)app->gfx->mapWidth / 2.0f, (float)app->gfx->mapHeight / 2.0f);

    app->client = client;
    app->udpManager = UDPManagerCreate();

    app->listenThread = SDL_CreateThread((SDL_ThreadFunction)ListenToServer, "Server Listen Thread", (void *)app->client);
    if (UDPClientSend(app->client, UDPTypeText, "alive\0", 6))
    {
        log_info("Sending Message: alive\n");
        SDL_Delay(2000);
        if (app->client->hasPacket && UDPPackageDecode((char *)app->client->pack->data) == UDPTypeint)
        {
            UDPPackageRemoveTypeNULL(app->client->pack);
            log_info("Incoming Message: %d\n", *(int *)app->client->pack->data);
            app->entityManager->entities[0].id = *(int *)app->client->pack->data;
            app->client->hasPacket = SDL_FALSE;
        }
    }
    app->updateThread = SDL_CreateThread((SDL_ThreadFunction)UpdateFromServer, "Server Update Thread", (void *)app);
    for (int i = 1; i < 10; i++)
    {
        Entity *npc = EntityManagerAdd(app->entityManager, ET_Woman, Vec2Create(100.0f * i, 0.0f));
        npc->isNPC = SDL_TRUE;
    }
    app->player.entity->entityState = EntityPlayer;

#ifdef APP_DEBUG
    for (int i = 1; i < app->entityManager->highestIndex; i++)
    {
        app->entityManager->entities[i].id = 0;
    }
#endif
    ScoreCreate(0);
    ScoreIncrement(100, 0);

    app->groundListItems = GroundListCreate();
    app->player.entity->inventory = InventoryCreate();

    app->state.gameState = GS_Menu;
    app->state.menuState = MS_MainMenu;

    app->map.contents = NULL;
    app->map.n = 0;
    app->mapList = MapListCreate("maps");

    return app;
}
void AppClientDestroy(AppClient *app)
{
    app->client->isActive = SDL_FALSE;
    GraphicsDestroy(app->gfx);
    AudioDestroy(app->audio);
    CameraDestroy(app->camera);

    MenuDestroy(app->menu);
    GuiDestroy(app->gui);
    FontDestroy(app->font);
    SDL_free(app);
}

void AppClientRun(AppClient *app)
{
    WindowClear(app->gfx->window);
    AppClientUpdate(app);
    AppClientDraw(app);
    WindowPresent(app->gfx->window);
}

void AppClientUpdate(AppClient *app)
{
    switch (app->state.gameState)
    {
    case GS_Menu:
    {
        CursorChange(CT_Normal);
        MapListUpdate(&app->mapList);
        switch (app->state.menuState)
        {
        case MS_CustomMap:
            CameraUpdate(app->camera);
            break;
        default:
            break;
        }
        break;
    }
    case GS_Playing:
    {
        CursorChange(CT_Crossair);
        if (InputIsKeyPressed(app->input, SDL_SCANCODE_ESCAPE))
        {
            app->state.gameState = GS_Menu;
            app->state.menuState = MS_MainMenu;
            break;
        }
        CameraUpdate(app->camera);

        if (InputIsKeyPressed(app->input, SDL_SCANCODE_Q))
        { // if player is near to the item, then take it!
            if (app->entityManager->entities[0].inventory.top < MAX_PLYER_ITEMS)
            {
                for (int tmp = 0; tmp < 2; tmp++)
                {
                    if (SDL_HasIntersection(&app->entityManager->entities[0].drawables[0].dst, &app->groundListItems.contents[tmp].drawable.dst))
                    {
                        ItemPickup(&app->entityManager->entities[0].inventory, &app->groundListItems.contents[tmp], &app->groundListItems, tmp);
                        log_info("you picked up an item. \n");
                    }
                }
            }
            else
            {
                log_info("You can't pick this item. Your item list is full! \n");
            }
        }

        if (InputIsKeyPressed(app->input, SDL_SCANCODE_Z))
        {
            if (app->entityManager->entities[0].inventory.top > 1) // can't drop the knife
            {
                ItemDrop(&app->groundListItems, &app->entityManager->entities[0].inventory, app->entityManager->entities[0].position);
            }
        }

        if (InputIsKeyDown(app->input, SDL_SCANCODE_TAB))
        {
            if (InputIsKeyPressed(app->input, SDL_SCANCODE_2))
            {
                log_info("You Pressed 2 while tab");
                ItemDynamicDrop(&app->groundListItems, &app->entityManager->entities[0].inventory, app->entityManager->entities[0].position, 2);
            }
        }

        if (InputIsKeyPressed(app->input, SDL_SCANCODE_3))
        {
            log_info("You Pressed 3");
            InventorySelectItem(&app->entityManager->entities[0].inventory, 3);
        }

        if (InputIsKeyPressed(app->input, SDL_SCANCODE_4))
        {
            log_info("You Pressed 4");
            InventorySelectItem(&app->entityManager->entities[0].inventory, 4);
        }

        if (InputIsKeyPressed(app->input, SDL_SCANCODE_5))
        {
            log_info("You Pressed 5");
            InventorySelectItem(&app->entityManager->entities[0].inventory, 5);
        }

        if (InputIsMousePressed(app->input, BUTTON_LEFT))
        { // always the item on hand is in the last place in the inventory list
            // if there is ammo in ur weapon shoot
            if (app->entityManager->entities[0].inventory.contents[app->entityManager->entities[0].inventory.top - 1].Stats.ammo > 0)
            {
                playerShoot(&app->entityManager->entities[0], app->camera, app->entityManager->entities, app->entityManager->entities[0].inventory.contents[app->entityManager->entities[0].inventory.top - 1]);
            }
        }
        BehaviorMoveEntity(app->entityManager);
        PlayerUpdate(&app->player, app->input, app->clock, app->camera);

        // EntityUpdate most be after input, playerupdate
        EntityManagerUpdate(app->entityManager, app->clock);

        CompressedEntity sendCompressedEntity = EntityCompress(app->entityManager->entities[0]);
        UDPClientSend(app->client, UDPTypeCompressedEntity, &sendCompressedEntity, sizeof(CompressedEntity));
        // SDL_PixelFormat *fmt;
        // SDL_Color *color;
        // fmt = app->gfx->format;
        // Uint8 index;
        // index = *(Uint8 *)surface->pixels;
        // color = &fmt->palette->colors[index];
        // for (int i = 0; i < ; i++)
        // {
        //     for (int j = 0; j < count; j++)
        //     {
        //         /* code */
        //     }
        //     printf("\n");
        // }
        break;
    }
    default:
        break;
    }
}

void AppClientDraw(AppClient *app)
{
    switch (app->state.gameState)
    {
    case GS_Menu:
    {
        switch (app->state.menuState)
        {
        case MS_CustomMap:
            CameraSetFollow(app->camera, &app->middleOfMap);
            MapDraw(&app->map, app->camera);
            break;
        default:
            break;
        }
        MenuUpdate(app->menu, app->input, app->fpsManager, &app->mapList, &app->map);
        break;
    }
    case GS_Playing:
    {
        CameraSetFollow(app->camera, &app->player.aimFollow);
        MapDraw(&app->map, app->camera);

        UpdateItemDraw(&app->entityManager->entities[0].inventory, &app->groundListItems, app->camera);

        for (int i = 0; i < app->entityManager->highestIndex; i++)
        {
            if (app->entityManager->bitmap[i] && app->entityManager->entities[i].isNPC)
                EntityDraw(&app->entityManager->entities[i], app->camera);
        }
        PlayerDraw(&app->player, app->camera);
        UDPManagerDraw(&app->udpManager, app->camera);
        GuiUpdate(app->gui);

        if (InputIsKeyDown(app->input, SDL_SCANCODE_TAB))
        {
            InventoryDisplay(app->gfx, &app->player.entity->inventory);
        }

        InventoryDisplayEquiped(app->camera, &app->player.entity->inventory, app->player.entity->position);

        break;
    }
    default:
        break;
    }
}