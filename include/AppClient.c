#include "AppClient.h"

struct AppClient
{
    SDL_bool *running;

    Graphics *gfx;
    Font *font;
    Gui *gui;
    Camera *camera;
    FPSManager *fpsManager;
    Menu *menu;
    Keybinding *bindings;
    Vec2 middleOfMap;
    MovingPattern *movingPattern;

    GroundListItems groundListItems;

    Player player; // player == entity 0

    MapList *mapList;
};

AppClient *AppClientCreate(SDL_bool *running, FPSManager *fpsManager)
{
    srand(time(NULL));
    CursorInitialize();
    EntityManagerInitialize();
    StateInitialize();
    MapInitialize();
    AudioInitialize();

    AppClient *app = (AppClient *)SDL_malloc(sizeof(AppClient));
    app->running = running;
    app->fpsManager = fpsManager;
    app->gfx = GraphicsCreate();
    app->font = FontCreate(app->gfx);
    app->gui = GuiCreate(app->font);
    app->camera = CameraCreate(app->gfx, NULL);
    app->bindings = KeybindingCreate();
    app->mapList = MapListCreate("maps");
    app->menu = MenuCreate(app->gfx, app->camera, app->font, app->bindings, app->mapList);
    app->player = PlayerCreate(app->camera);
    app->movingPattern = behaviorPathsCreate();
    app->middleOfMap = Vec2Create((float)app->gfx->mapWidth / 2.0f, (float)app->gfx->mapHeight / 2.0f);

    StateSetMenu(app->menu);

    NotifyInitialize(app->font);

    LobbyInitialize();
    ClientInitialize(&app->player);
    ClientManagerInitialize();

    for (int i = 1; i < 10; i++)
    {
        EntityIndexP npc = EntityManagerAdd(ET_Woman, Vec2Create(100.0f * i + 300.0f, 0.0f));
        ENTITY_ARRAY[*npc].isNPC = SDL_TRUE;
    }
    ENTITY_ARRAY[*app->player.entity].entityState = EntityPlayer;

    ScoreCreate(0);
    ScoreIncrement(100, 0);

    app->groundListItems = GroundListCreate();

    GameStateSet(GS_Menu);
    MenuStateSet(MS_Splash);

    return app;
}
void AppClientDestroy(AppClient *app)
{
    if (clientManager.inLobby || clientManager.inGame)
    {
        ClientTCPSend(PT_LeaveSession, NULL, 0);
    }
    ClientManagerUninitialize();
    ClientUninitialize();
    LobbyUninitialize();
    AudioUninitialize();

    GraphicsDestroy(app->gfx);
    CameraDestroy(app->camera);
    pathFree(app->movingPattern);

    MenuDestroy(app->menu);
    KeybindingFree(app->bindings);
    GuiDestroy(app->gui);
    FontDestroy(app->font);

    MapUninitialize();
    EntityManagerUninitalize();

    SDL_free(app);
}

void AppClientRun(AppClient *app)
{
    WindowClear(app->gfx->window);
    AppClientDraw(app);
    AppClientUpdate(app);
    // AppClientDraw(app);
    WindowPresent(app->gfx->window);
}

void AppClientUpdate(AppClient *app)
{
    ClientUpdate();
    ClientManagerUpdate();

    switch (GameStateGet())
    {
    case GS_Menu:
    {
        CursorChange(CT_Normal);
#ifndef WIN_DEBUG
        MapListUpdate(app->mapList);
#endif
        switch (MenuStateGet())
        {
        case MS_CustomMap:
        case MS_HostLobby:
        case MS_JoinLobby:
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
        if (InputIsKeyPressed(SDL_SCANCODE_ESCAPE))
        {
            GameStateSet(GS_Menu);
            if (clientManager.inGame == SDL_TRUE)
            {
                MenuStateSet(MS_InGameMenu);
            }
            else
            {
                MenuStateSet(MS_MainMenu);
            }
            break;
        }
        CameraUpdate(app->camera);

        if (InputIsKeyPressed(SDL_SCANCODE_Q))
        { // if player is near to the item, then take it!
            if (app->player.inventory.top < MAX_PLYER_ITEMS)
            {
                for (int tmp = 0; tmp < 2; tmp++)
                {
                    if (SDL_HasIntersection(&ENTITY_ARRAY[*app->player.entity].drawables[0].dst, &app->groundListItems.contents[tmp].drawable.dst))
                    {
                        ItemPickup(&app->player.inventory, &app->groundListItems.contents[tmp], &app->groundListItems, tmp);
                        log_info("you picked up an item. \n");
                    }
                }
            }
            else
            {
                log_info("You can't pick this item. Your item list is full! \n");
            }
        }

        if (InputIsKeyPressed(SDL_SCANCODE_Z))
        {
            if (app->player.inventory.top > 1) // can't drop the knife
            {
                ItemDrop(&app->groundListItems, &app->player.inventory, ENTITY_ARRAY[*app->player.entity].position);
            }
        }

        if (InputIsKeyDown(SDL_SCANCODE_TAB))
        {
            if (InputIsKeyPressed(SDL_SCANCODE_2))
            {
                log_info("You Pressed 2 while tab");
                ItemDynamicDrop(&app->groundListItems, &app->player.inventory, ENTITY_ARRAY[*app->player.entity].position, 2);
            }
        }

        if (InputIsKeyPressed(SDL_SCANCODE_3))
        {
            log_info("You Pressed 3");
            InventorySelectItem(&app->player.inventory, 3);
        }

        if (InputIsKeyPressed(SDL_SCANCODE_4))
        {
            log_info("You Pressed 4");
            InventorySelectItem(&app->player.inventory, 4);
        }

        if (InputIsKeyPressed(SDL_SCANCODE_5))
        {
            log_info("You Pressed 5");
            InventorySelectItem(&app->player.inventory, 5);
        }

        if (InputIsMousePressed(BUTTON_LEFT))
        { // always the item on hand is in the last place in the inventory list
            // if there is ammo in ur weapon shoot
            if (ENTITY_ARRAY[*app->player.entity].entityState == EntityPlayer) // if player isn't dead
            {
                if (app->player.inventory.contents[app->player.inventory.top - 1].Stats.ammo > 0)
                {
                    playerShoot(app->player.entity, app->camera, &app->player.inventory.contents[app->player.inventory.top - 1], app->gfx->window->renderer);
                }
            }
        }
        weaponUpdate(&app->player.inventory.contents[app->player.inventory.top - 1]);

        BehaviorMoveEntity(app->movingPattern, app->gfx->window->renderer, app->camera, &app->player);

        if (InputIsKeyPressed(SDL_SCANCODE_K))
        {
            ENTITY_ARRAY[2].desiredPoint.x = 180;
            ENTITY_ARRAY[2].desiredPoint.y = 180;
            ENTITY_ARRAY[2].entityState = Neutral;
        }
        if (InputIsKeyPressed(SDL_SCANCODE_O))
        {
            log_debug("Player health = %d", ENTITY_ARRAY[*app->player.entity].health);
        }

        PlayerUpdate(&app->player, app->camera);

        // EntityUpdate most be after input, playerupdate
        EntityManagerUpdate();

        // Sends player to server
        CompressedEntity cEntity = EntityCompress(&ENTITY_ARRAY[*app->player.entity]);
        ClientUDPSend(PT_CompressedEntity, &cEntity, sizeof(CompressedEntity));

        break;
    }
    default:
        break;
    }
}

void AppClientDraw(AppClient *app)
{
    switch (GameStateGet())
    {
    case GS_Menu:
    {
        MenuUpdate(app->menu, app->fpsManager, &app->player);
        switch (MenuStateGet())
        {
        case MS_HostLobby:
        case MS_CustomMap:
        case MS_Lobby:
            CameraSetFollow(app->camera, &app->middleOfMap);
            break;

        default:
            break;
        }
        break;
    }
    case GS_Playing:
    {
        CameraSetFollow(app->camera, &app->player.aimFollow);
        MapDraw(app->camera);

        UpdateItemDraw(&app->player.inventory, &app->groundListItems, app->camera);

        for (int i = 1; i < ENTITY_ARRAY_SIZE; i++)
        {
            if (ENTITY_ARRAY[i].isNPC)
                EntityDraw(&ENTITY_ARRAY[i], app->camera);
        }
        PlayerDraw(&app->player, app->camera);
        ClientManagerDrawConnectedPlayers(app->camera);
        GuiUpdate(app->gui);

        if (InputIsKeyDown(app->bindings->KeyArray[INVENTORY]))
        {
            InventoryDisplay(app->gfx, &app->player.inventory);
        }

        InventoryDisplayEquiped(app->camera, &app->player.inventory, ENTITY_ARRAY[*app->player.entity].position);

        break;
    }
    default:
        break;
    }
    NotifierUpdate();
}