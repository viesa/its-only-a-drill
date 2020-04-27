#include "core/AppClient.h"
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

    GroundListItems groundListItems;

    Player player; // player == entity 0

    Map map;
    MapList mapList;
};

AppClient *AppClientCreate(SDL_bool *running, Clock *clock, Input *input, FPSManager *fpsManager)
{
    srand(time(NULL));
    CursorInitialize();
    EntityManagerInitialize();

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
    app->menu = MenuCreate(app->gfx, app->font, &app->state);
    app->player = PlayerCreate(app->camera);
    app->middleOfMap = Vec2Create((float)app->gfx->mapWidth / 2.0f, (float)app->gfx->mapHeight / 2.0f);

    ClientManagerInitialize(&app->player);
    ClientStart();

    for (int i = 1; i < 10; i++)
    {
        EntityIndexP npc = EntityManagerAdd(ET_Woman, Vec2Create(100.0f * i, 0.0f));
        ENTITY_ARRAY[*npc].isNPC = SDL_TRUE;
    }
    ENTITY_ARRAY[*app->player.entity].entityState = EntityPlayer;

    ScoreCreate(0);
    ScoreIncrement(100, 0);

    app->groundListItems = GroundListCreate();
    ENTITY_ARRAY[*app->player.entity].inventory = InventoryCreate();

    app->state.gameState = GS_Menu;
    app->state.menuState = MS_Splash;

    app->map.contents = NULL;
    app->map.n = 0;
    app->mapList = MapListCreate("maps", app->clock);

    return app;
}
void AppClientDestroy(AppClient *app)
{
    ClientStop();
    GraphicsDestroy(app->gfx);
    AudioDestroy(app->audio);
    CameraDestroy(app->camera);
    EntityManagerDestroy();

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
    ClientManagerUpdate();

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
            if (ENTITY_ARRAY[*app->player.entity].inventory.top < MAX_PLYER_ITEMS)
            {
                for (int tmp = 0; tmp < 2; tmp++)
                {
                    if (SDL_HasIntersection(&ENTITY_ARRAY[*app->player.entity].drawables[0].dst, &app->groundListItems.contents[tmp].drawable.dst))
                    {
                        ItemPickup(&ENTITY_ARRAY[*app->player.entity].inventory, &app->groundListItems.contents[tmp], &app->groundListItems, tmp);
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
            if (ENTITY_ARRAY[*app->player.entity].inventory.top > 1) // can't drop the knife
            {
                ItemDrop(&app->groundListItems, &ENTITY_ARRAY[*app->player.entity].inventory, ENTITY_ARRAY[*app->player.entity].position);
            }
        }

        if (InputIsKeyDown(app->input, SDL_SCANCODE_TAB))
        {
            if (InputIsKeyPressed(app->input, SDL_SCANCODE_2))
            {
                log_info("You Pressed 2 while tab");
                ItemDynamicDrop(&app->groundListItems, &ENTITY_ARRAY[*app->player.entity].inventory, ENTITY_ARRAY[*app->player.entity].position, 2);
            }
        }

        if (InputIsKeyPressed(app->input, SDL_SCANCODE_3))
        {
            log_info("You Pressed 3");
            InventorySelectItem(&ENTITY_ARRAY[*app->player.entity].inventory, 3);
        }

        if (InputIsKeyPressed(app->input, SDL_SCANCODE_4))
        {
            log_info("You Pressed 4");
            InventorySelectItem(&ENTITY_ARRAY[*app->player.entity].inventory, 4);
        }

        if (InputIsKeyPressed(app->input, SDL_SCANCODE_5))
        {
            log_info("You Pressed 5");
            InventorySelectItem(&ENTITY_ARRAY[*app->player.entity].inventory, 5);
        }

        Entity *ePlayer = &ENTITY_ARRAY[*app->player.entity];
        if (InputIsMousePressed(app->input, BUTTON_LEFT))
        { // always the item on hand is in the last place in the inventory list
            // if there is ammo in ur weapon shoot
            if (ePlayer->inventory.contents[ePlayer->inventory.top - 1].Stats.ammo > 0)
            {
                playerShoot(app->player.entity, app->camera, app->input, &ePlayer->inventory.contents[ePlayer->inventory.top - 1], app->clock);
            }
        }
        weaponUpdate(&ePlayer->inventory.contents[ePlayer->inventory.top - 1], app->clock);

        BehaviorMoveEntity();
        if (InputIsKeyPressed(app->input, SDL_SCANCODE_K))
        {
            ENTITY_ARRAY[1].desiredPoint.x = 180;
            ENTITY_ARRAY[1].desiredPoint.y = 180;
            ENTITY_ARRAY[1].entityState = Aggressive;
        }

        //PlayerUpdate(&app->player, &app->entityManager.entities[0], app->input, app->clock, app->camera);
        PlayerUpdate(&app->player, app->input, app->clock, app->camera);

        // EntityUpdate most be after input, playerupdate
        EntityManagerUpdate(app->clock);

        //CompressedEntity sendCompressedEntity = EntityCompress(app->ENTITY_ARRAY[*0]);
        //ClientSend(app->client, PT_CompressedEntity, &sendCompressedEntity, sizeof(CompressedEntity));

        ClientSend(PT_Entity, &ENTITY_ARRAY[*app->player.entity], sizeof(Entity));
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

        UpdateItemDraw(&ENTITY_ARRAY[*app->player.entity].inventory, &app->groundListItems, app->camera);

        for (int i = 0; i < ENTITY_ARRAY_SIZE; i++)
        {
            if (ENTITY_ARRAY[i].isNPC)
                EntityDraw(&ENTITY_ARRAY[i], app->camera);
        }
        PlayerDraw(&app->player, app->camera);
        ClientManagerDrawConnectedPlayers(app->camera);
        GuiUpdate(app->gui);

        if (InputIsKeyDown(app->input, SDL_SCANCODE_TAB))
        {
            InventoryDisplay(app->gfx, &ENTITY_ARRAY[*app->player.entity].inventory);
        }

        InventoryDisplayEquiped(app->camera, &ENTITY_ARRAY[*app->player.entity].inventory, ENTITY_ARRAY[*app->player.entity].position);

        break;
    }
    default:
        break;
    }
}