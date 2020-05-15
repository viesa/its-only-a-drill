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

    GroundListItems groundListItems;

    Player *player;
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
    app->player = PlayerCreate(app->camera);
    app->middleOfMap = Vec2Create((float)app->gfx->mapWidth / 2.0f, (float)app->gfx->mapHeight / 2.0f);
    AppClientUpdateSettings(app);
    app->menu = MenuCreate(app->gfx, app->camera, app->font, app->bindings, app->mapList);
    StateSetMenu(app->menu);

    NotifyInitialize(app->font);

    LobbyInitialize();
    ClientInitialize(app->player);
    ClientManagerInitialize();

    NPCManagerInitialize(app->player);

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

    NPCManagerUninitialize();
    PlayerDestroy(app->player);

    MenuDestroy(app->menu);
    KeybindingFree(app->bindings);
    GuiDestroy(app->gui);
    FontDestroy(app->font);

    MapUninitialize();
    EntityManagerUninitalize();

    CameraDestroy(app->camera);
    GraphicsDestroy(app->gfx);

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
        case MS_Lobby:
            CameraSetFollowSnap(app->camera, &app->middleOfMap);
        case MS_InGameMenu:
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
            MenuStateSet(MS_InGameMenu);
        }
        CameraUpdate(app->camera);

        PlayerUpdate(app->player, app->camera);

        // EntityUpdate most be after input, playerupdate
        EntityManagerUpdate();

        // Sends player to server
        CompressedEntity cEntity = EntityCompress(PlayerGetEntity(app->player));
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
        MenuUpdate(app->menu, app->fpsManager, app->player);
        GuiOverlayUpdate(app->gui);
        if (MenuStateGet() == MS_InGameMenu)
        {
            PlayerDraw(app->player, app->camera);
            ClientManagerDrawConnectedPlayers(app->camera);
        }
        break;
    }
    case GS_Playing:
    {
        CameraSetFollow(app->camera, PlayerGetAimFollowP(app->player));
        MapDraw(app->camera);

        NPCManagerDrawAllNPCS(app->camera);
        PlayerDraw(app->player, app->camera);
        ClientManagerDrawConnectedPlayers(app->camera);
        GuiUpdate(app->gui);
        break;
    }
    default:
        break;
    }
    NotifierUpdate();

    if (InputIsKeyPressed(SDL_SCANCODE_P))
        log_info("FPS: %f", ClockGetFPS());

#ifdef ANY_DEBUG
    GuiDrawFPS(app->gui);
#endif
}
void AppClientUpdateSettings(AppClient *app)
{
    Settings settings = SettingsGetFromFile(SETTINGS_PATH);
    if (settings.resolutionH != 1) // found settings file
    {
        *app->bindings = settings.keys; //keys

        WindowSetSize(app->gfx->window, settings.resolutionW, settings.resolutionH); //resolution

        PlayerSetSpriteSheet(app->player, (SpriteSheet)settings.skin);

        WindowSetFullscreen(app->gfx->window, settings.isFullscreen); // fullscreen

        WindowSetVSync(app->gfx->window, settings.vsync); // vsync

        app->fpsManager->desiredFPS = settings.fps; // fps
    }
    SettingsDestroy(&settings);
}