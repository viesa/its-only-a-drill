#include "AppClient.h"

struct AppClient
{
    SDL_bool *running;

    Vec2 middleOfMap;

    GroundListItems groundListItems;

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
    ClientManagerInitialize();
    GuiInitialize();
    KeybindingInitialize();
    ScoreboardInitialize();
    PlayerInitialize();
    SettingsInitialize();
    NPCManagerInitialize();

    AppClient *app = (AppClient *)SDL_malloc(sizeof(AppClient));
    app->running = running;
    app->mapList = MapListCreate("maps");
    app->middleOfMap = Vec2Create((float)GraphicsGetMapWidth() / 2.0f, (float)GraphicsGetMapHeight() / 2.0f);
    app->groundListItems = GroundListCreate();

    MenuInitialize(app->mapList);
    ClientInitialize();

    GameStateSet(GS_Menu);
    MenuStateSet(MS_Splash);

    SettingsApply();
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

    SettingsUninitialize();
    PlayerUninitialize();
    ScoreboardUninitialize();
    KeybindingUninitialize();
    GuiUninitialize();
    ClientManagerUninitialize();
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
    ClientManagerHandleAllPackets();

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
        PlayerUpdate();

        // EntityUpdate most be after input, playerupdate
        EntityManagerUpdate();

        // Sends player to server
        CompressedEntity cEntity = EntityCompress(PlayerGetEntity());
        ClientUDPSend(PT_CompressedEntity, &cEntity, sizeof(CompressedEntity));
        break;
    }
    case GS_RoundFinished:
    case GS_MatchFinished:
    default:
        break;
    }
    if (ConStateGet() == CON_Online)
        ClientTCPSend(PT_FetchScoreboard, NULL, 0);
}

void AppClientDraw(AppClient *app)
{

    switch (GameStateGet())
    {
    case GS_Menu:
    {
        MenuUpdate();
        GuiOverlayUpdate();
        if (MenuStateGet() == MS_InGameMenu)
        {
            PlayerDraw();
            ClientManagerDrawConnectedPlayers();
        }
        break;
    }
    case GS_Playing:
    case GS_RoundFinished:
    case GS_MatchFinished:
    {
        CameraSetFollow(PlayerGetAimFollowP());
        MapDraw();

        NPCManagerDrawAllNPCS();
        ClientManagerDrawConnectedPlayers();
        PlayerDraw();
        ClientManagerDrawBufferedShootingLines();
        GuiUpdate();

        if (GameStateGet() == GS_RoundFinished)
        {
            GuiDrawFinishedRoundMessage();
        }
        else if (GameStateGet() == GS_MatchFinished)
        {
            GuiDrawFinishedMatchMessage();
        }

        break;
    }
    default:
        break;
    }
    NotifierUpdate();

#ifdef ANY_DEBUG
    GuiDrawFPS();
#endif
}