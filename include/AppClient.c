#include "AppClient.h"

struct AppClient
{
    SDL_bool *running;

    Vec2 middleOfMap;

    GroundListItems groundListItems;

    Player *player;
    MapList *mapList;
};

AppClient *AppClientCreate(SDL_bool *running)
{
    srand(time(NULL));
    CursorInitialize();
    StateInitialize();
    GraphicsInitialize();
    CameraInitialize();
    FontInitialize();
    EntityManagerInitialize();
    MapInitialize();
    AudioInitialize();
    TransitionInitialize();
    NotifyInitialize();
    LobbyInitialize();
    ClientManagerInitialize();
    GuiInitialize();
    KeybindingInitialize();

    AppClient *app = (AppClient *)SDL_malloc(sizeof(AppClient));
    app->running = running;
    app->mapList = MapListCreate("maps");
    app->player = PlayerCreate();
    app->middleOfMap = Vec2Create((float)GraphicsGetMapWidth() / 2.0f, (float)GraphicsGetMapHeight() / 2.0f);
    app->groundListItems = GroundListCreate();

    MenuInitialize(app->mapList);
    ClientInitialize(app->player);
    NPCManagerInitialize(app->player);

    ScoreCreate(0);
    ScoreIncrement(100, 0);

    GameStateSet(GS_Menu);
    MenuStateSet(MS_Splash);

    AppClientUpdateSettings(app);
    return app;
}
void AppClientDestroy(AppClient *app)
{
    if (ClientManagerIsInLobby() || ClientManagerIsInGame())
    {
        ClientTCPSend(PT_LeaveSession, NULL, 0);
    }
    NPCManagerUninitialize();
    ClientUninitialize();
    MenuUninitialize();

    KeybindingUninitialize();
    GuiUninitialize();
    ClientManagerUninitialize();
    LobbyUninitialize();
    TransitionUninitialize();
    AudioUninitialize();
    MapUninitialize();
    EntityManagerUninitialize();
    FontUninitialize();
    CameraUninitialize();
    GraphicsUninitialize();

    FREE(app);
}

void AppClientRun(AppClient *app)
{
    WindowClear();
    AppClientDraw(app);
    AppClientUpdate(app);
    // AppClientDraw(app);
    WindowPresent();
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
            CameraSetFollowSnap(&app->middleOfMap);
        case MS_InGameMenu:
            CameraUpdate();
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
        CameraUpdate();

        NPCManagerUpdate();
        PlayerUpdate(app->player);

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
        MenuUpdate(app->player);
        GuiOverlayUpdate();
        if (MenuStateGet() == MS_InGameMenu)
        {
            PlayerDraw(app->player);
            ClientManagerDrawConnectedPlayers();
        }
        break;
    }
    case GS_Playing:
    {
        CameraSetFollow(PlayerGetAimFollowP(app->player));
        MapDraw();

        NPCManagerDrawAllNPCS();
        PlayerDraw(app->player);
        ClientManagerDrawConnectedPlayers();
        GuiUpdate();
        break;
    }
    default:
        break;
    }
    NotifierUpdate();

    if (InputIsKeyPressed(SDL_SCANCODE_P))
        log_info("FPS: %f", ClockGetFPS());

#ifdef ANY_DEBUG
    GuiDrawFPS();
#endif
}
void AppClientUpdateSettings(AppClient *app)
{
    Settings settings = SettingsGetFromFile(SETTINGS_PATH);
    if (settings.resolutionH != 1) // found settings file
    {
        WindowSetSize(settings.resolutionW, settings.resolutionH); //resolution

        PlayerSetSpriteSheet(app->player, (SpriteSheet)settings.skin);

        WindowSetFullscreen(settings.isFullscreen); // fullscreen

        WindowSetVSync(settings.vsync); // vsync

        FPSManagerSetDesiredFPS(settings.fps); // fps
    }
    SettingsDestroy(&settings);
}