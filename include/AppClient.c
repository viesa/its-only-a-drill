#include "core/AppClient.h"
#include "Items.h"
#include "Player.h"
#include "./core/Behavior.h"
#include "Map.h"
#include "MapList.h"
#include "core/Weapon.h"
#include "core/Behavior.h"
#include "./core/EntityManager.h"
//#define DEGBUG
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
    FpsManger *FPSControls;
    Input *input;
    Menu *menu;

    Vec2 middleOfMap;

    //UDP stuff (client, server messages etc.)
    UDPClient *client;
    SDL_Thread *listenThread;

    GroundListItems groundListItems;

    EntityManager entityManager;
    Player player; // player == entity 0

    Map map;
    MapList mapList;
};
void ListenToServer(void *args)
{
    UDPClient *client = (UDPClient *)args;
    while (client->isActive)
    {
        SDL_Delay(5);
        if (client->hasPacket)
            continue;
        UDPClientListen(client, MAX_MSGLEN);
    }
}
AppClient *AppClientCreate(SDL_bool *running, Clock *clock, Input *input, UDPClient *client, FpsManger *FPSControls)
{
    AppClient *app = (AppClient *)SDL_malloc(sizeof(AppClient));
    app->running = running;
    app->state = StateCreate();
    app->clock = clock;
    app->FPSControls = FPSControls;
    app->gfx = GraphicsCreate();
    app->audio = AudioCreate();
    app->font = FontCreate(app->gfx);
    app->gui = GuiCreate(app->font, app->clock);
    app->camera = CameraCreate(app->gfx, NULL);
    app->input = input;
    app->menu = MenuCreate(app->gfx, app->font, &app->state);
    app->player = PlayerCreate();
    app->middleOfMap = Vec2Create((float)app->gfx->mapWidth / 2.0f, (float)app->gfx->mapHeight / 2.0f);

    app->client = client;
#ifdef DEGBUG
    app->listenThread = SDL_CreateThread((SDL_ThreadFunction)ListenToServer, "Server Listen Thread", (void *)app->client);
#endif
    app->entityManager = EntityManagerCreate();
    EntityManagerAdd(&app->entityManager, EntityCreate((Vec2){50, 50}, EntityWoman, 0));
    EntityManagerAdd(&app->entityManager, EntityCreate((Vec2){300, 0}, EntityWoman, 1));
    EntityManagerAdd(&app->entityManager, EntityCreate((Vec2){500, 0}, EntityWoman, 2));
    app->entityManager.entities[0].entityState = EntityPlayer;

#ifdef DEGBUG
    for (int i = 1; i < MaxEntities; i++)
    {
        app->entities[i].id = 0;
    }
    app->nrEntities = 1;
#endif
    ScoreCreate(0);
    ScoreIncrement(100, 0);

    app->groundListItems = GroundListCreate();
    app->entityManager.entities[0].inventory = InventoryCreate();

#ifdef DEGBUG
    if (UDPClientSend(app->client, UDPTypeText, "alive\0", 6))
    {
        log_info("Sending Message: alive\n");
        SDL_Delay(1000);
        if (app->client->hasPacket && UDPPackageDecode((char *)app->client->pack->data) == UDPTypeint)
        {
            UDPPackageRemoveTypeNULL(app->client->pack);
            log_info("Incoming Message: %d\n", *(int *)app->client->pack->data);
            app->entities[0].id = *(int *)app->client->pack->data;
            app->client->hasPacket = SDL_FALSE;
        }
    }
#endif
#ifndef DEGBUG
    UDPClientSend(app->client, UDPTypeText, "alive\0", 6);
#endif
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
    UDPClientDestroy(app->client);
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
    GraphicsClearScreen(app->gfx);
    AppClientUpdate(app);
    AppClientDraw(app);
    GraphicsPresentScreen(app->gfx);
}

void AppClientUpdate(AppClient *app)
{
#ifdef DEGBUG
    if (app->client->hasPacket)
    {
        if (UDPPackageDecode((char *)app->client->pack->data) == UDPTypeText)
        {
            log_info("%s\n", app->client->pack->data);
            app->client->hasPacket = SDL_FALSE;
        }
        if (UDPPackageDecode((char *)app->client->pack->data) == UDPTypeEntity)
        {
            UDPPackageRemoveTypeNULL(app->client->pack);
            Entity ent = *(Entity *)app->client->pack->data;
            app->client->hasPacket = SDL_FALSE;
            SDL_bool exist = SDL_FALSE;
            for (int i = 0; i < MaxEntities; i++)
            {
                if (app->entities[i].id == ent.id) //entity exists
                {
                    exist = SDL_TRUE;
                    app->entities[i] = ent;
                }
            }
            if (!exist) //entity doesnt exist, allocate
            {
                app->entities[app->nrEntities] = ent;
                app->nrEntities++;
            }
        }
    }
#endif

    switch (app->state.gameState)
    {
    case GS_Menu:
    {
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
        if (InputIsKeyPressed(app->input, SDL_SCANCODE_ESCAPE))
        {
            app->state.gameState = GS_Menu;
            app->state.menuState = MS_MainMenu;
            break;
        }
        CameraUpdate(app->camera);

        if (InputIsKeyPressed(app->input, SDL_SCANCODE_Q))
        { // if player is near to the item, then take it!
            if (app->entityManager.entities[0].inventory.top < MAX_PLYER_ITEMS)
            {
                for (int tmp = 0; tmp < 2; tmp++)
                {
                    if (SDL_HasIntersection(&app->entityManager.entities[0].drawable.dst, &app->groundListItems.contents[tmp].drawable.dst))
                    {
                        ItemPickup(&app->entityManager.entities[0].inventory, &app->groundListItems.contents[tmp], &app->groundListItems, tmp);
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
            if (app->entityManager.entities[0].inventory.top > 1) // can't drop the knife
            {
                ItemDrop(&app->groundListItems, &app->entityManager.entities[0].inventory, app->entityManager.entities[0].position);
            }
        }

        if (InputIsKeyDown(app->input, SDL_SCANCODE_TAB))
        {
            if (InputIsKeyPressed(app->input, SDL_SCANCODE_2))
            {
                log_info("You Pressed 2 while tab");
                ItemDynamicDrop(&app->groundListItems, &app->entityManager.entities[0].inventory, app->entityManager.entities[0].position, 2);
            }
        }

        if (InputIsMousePressed(app->input, BUTTON_LEFT))
        { // always the item on hand is in the last place in the inventory list
            // if there is ammo in ur weapon shoot
            if (app->entityManager.entities[0].inventory.contents[app->entityManager.entities[0].inventory.top - 1].Stats.ammo > 0)
            {
                playerShoot(&app->entityManager.entities[0], app->camera, app->entityManager.entities, app->entityManager.entities[0].inventory.contents[app->entityManager.entities[0].inventory.top - 1]);
            }
        }
        BehaviorMoveEntity(app->entityManager.entities, app->entityManager.nrEntities);
        PlayerUpdate(&app->player, &app->entityManager.entities[0], app->input, app->clock, app->camera);

        // EntityUpdate most be after input, playerupdate
        EntityUpdate(app->entityManager.entities, app->entityManager.nrEntities, app->clock);
#ifdef DEGBUG
        UDPClientSend(app->client, UDPTypeEntity, &app->entityManager.entities[0], sizeof(Entity));
#endif
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
            if (app->map.contents)
            {
                for (int i = 0; i < app->map.n; i++)
                {
                    EntityDraw(app->camera, &app->map.contents[i]);
                }
            }
            break;
        default:
            break;
        }
        MenuUpdate(app->menu, app->input, app->FPSControls, &app->mapList, &app->map);
        GraphicsChangeCursor(app->gfx, CU_Normal);
        break;
    }
    case GS_Playing:
    {
        CameraSetFollow(app->camera, &app->player.aimFollow);
        if (app->map.contents)
            for (int i = 0; i < app->map.n; i++)
                EntityDraw(app->camera, &app->map.contents[i]);

        UpdateItemDraw(&app->entityManager.entities[0].inventory, &app->groundListItems, app->camera);
        EntityDraw(app->camera, &app->entityManager.entities[0]);
        EntityDraw(app->camera, &app->entityManager.entities[1]);
        EntityDraw(app->camera, &app->entityManager.entities[2]);
        GuiUpdate(app->gui);
        if (InputIsKeyDown(app->input, SDL_SCANCODE_TAB))
        {
            InventoryDisplay(app->gfx, app->camera, &app->entityManager.entities[0].inventory, app->entityManager.entities[0].position);
        }
        GraphicsChangeCursor(app->gfx, CU_Crossair);
        break;
    }
    default:
        break;
    }

    GraphicsDrawGradient(app->gfx, (SDL_Rect){0, 0, app->gfx->windowWidth, app->gfx->windowHeight}, (SDL_Color){20, 180, 184, 50}, (SDL_Color){200, 159, 227, 50});
}