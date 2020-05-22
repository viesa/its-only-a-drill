#include "Menu.h"

#include "Library.h"

typedef struct Menu
{
    Drawable mainMenuDbl;
    float mainMenuDblDelta;
    int mainMenuDblDir;
    Drawable lobbyHostDbl;
    Drawable lobbyNormalDbl;
    LoadingBar *loadingBar;
    float loopCount;
    int loopSwing;
    int swingDir;
    int activeIndex;
    int lastIndex;
    int keybindingstate;
    SDL_bool indexChanged;
    float fetchSessionsTimer;
    float fetchLobbyTimer;
    Sound MenuStep;
    Music MenuTheme;
    Music MenuTheme2;
    Music GameTheme;
    int themecheck;
    int themecheck2;
    SDL_Color clr[10];
    int lobbyNumRounds;
    SDL_bool startedInTransition;
    SDL_bool startedOutTransition;

    Anim previewLeg;
    Anim previewBody;

    MapList *mapList;
} Menu;

static Menu *menu;

void MenuInitialize(MapList *mapList)
{
    menu = MALLOC(Menu);
    ALLOC_ERROR_CHECK(menu);

    menu->loadingBar = LoadingBarCreate();
    menu->loopCount = 0;
    menu->activeIndex = 0;
    menu->lastIndex = 0;
    menu->keybindingstate = 0;
    menu->fetchSessionsTimer = FETCH_SESSIONS_INTERVAL;
    menu->fetchLobbyTimer = FETCH_LOBBY_INTERVAL;
    menu->mainMenuDblDelta = 0.0f;
    menu->mainMenuDblDir = 1.0f;
    menu->startedInTransition = SDL_FALSE;
    menu->startedOutTransition = SDL_FALSE;

    SDL_Rect src = {0, 0, 3413, 1920};
    SDL_Rect dst = {0, 0, WindowGetWidth(), WindowGetHeight()};
    menu->mainMenuDbl = DrawableCreate(src, dst, SS_Menu);
    menu->lobbyNormalDbl = DrawableCreate((SDL_Rect){0, 0, 1920, 1080}, dst, SS_Lobby);
    menu->lobbyHostDbl = DrawableCreate((SDL_Rect){0, 0, 1920, 1080}, dst, SS_Lobby);
    TransitionStart(TT_FadeOut, 2);
    LoadingBarShow(menu->loadingBar);

    menu->MenuStep = SoundCreate(SF_MenuStep);
    menu->MenuTheme = MusicCreate(MF_MainMusic);
    menu->MenuTheme2 = MusicCreate(MF_MainMusicTwo);
    menu->GameTheme = MusicCreate(MF_GameMusic);
    menu->themecheck = 0;
    menu->themecheck2 = 0;

    menu->lobbyNumRounds = 5;

    menu->previewLeg = AnimCreate(AN_PlayerLegs, ANRO_RepeatFromEnd, SS_None, 4, 0.05f);
    menu->previewBody = AnimCreate(AN_PlayerBody, ANRO_RepeatFromEnd, SS_None, 4, 0.05f);

    menu->mapList = mapList;
}

void MenuUninitialize()
{
    LoadingBarDestroy(menu->loadingBar);
    FREE(menu);
}

void MenuResetActiveIndex()
{
    menu->activeIndex = 0;
}

void MenuResetFetchSessionsTimer()
{
    menu->fetchSessionsTimer = FETCH_SESSIONS_INTERVAL;
}

void MenuResetFetchLobbyTimer()
{
    menu->fetchLobbyTimer = FETCH_LOBBY_INTERVAL;
}

void MenuSetStartedInTransistion(SDL_bool started)
{
    menu->startedInTransition = started;
}

void MenuSetStartedOutTransistion(SDL_bool started)
{
    menu->startedOutTransition = started;
}

void MenuUpdate()
{
    menu->mainMenuDblDelta += 20.0f * ClockGetDeltaTime() * menu->mainMenuDblDir;
    menu->mainMenuDbl.src.x = menu->mainMenuDblDelta;
    if (menu->mainMenuDbl.src.x + menu->mainMenuDbl.src.w > 5760 && menu->mainMenuDblDir == 1)
    {
        menu->mainMenuDbl.src.x = 5760 - menu->mainMenuDbl.src.w;
        menu->mainMenuDblDir = -1;
    }
    else if (menu->mainMenuDbl.src.x < 0 && menu->mainMenuDblDir == -1)
    {
        menu->mainMenuDbl.src.x = 0;
        menu->mainMenuDblDir = 1;
    }

    if (menu->loopCount < 2 * PI)
    {
        menu->loopCount += ClockGetDeltaTime() * 2 * PI;
    }
    else
    {
        menu->loopCount = 0;
    }

    if (menu->themecheck == menu->themecheck2)
    {
        if (menu->themecheck != 0)
        {
            MusicStop(&menu->GameTheme);
            MusicPlay(&menu->MenuTheme2, -1);
            menu->themecheck++;
        }
    }

    //Update text colors
    menu->clr[0] = (SDL_Color){215, 159, 227, 255};
    menu->clr[1] = (SDL_Color){215, 139, 207, 255};
    menu->clr[2] = (SDL_Color){215, 119, 187, 255};
    menu->clr[3] = (SDL_Color){215, 99, 167, 255};
    menu->clr[4] = (SDL_Color){215, 79, 147, 255};
    menu->clr[5] = (SDL_Color){215, 59, 127, 255};
    menu->clr[6] = (SDL_Color){215, 39, 107, 255};
    menu->clr[7] = (SDL_Color){215, 19, 87, 255};
    menu->clr[8] = (SDL_Color){40, 180, 184, 255};
    menu->clr[9] = (SDL_Color){40, 180, 184, 255};

    AnimUpdate(&menu->previewBody, ClockGetDeltaTime());
    AnimUpdate(&menu->previewLeg, ClockGetDeltaTime());

    //Get input
    menu->activeIndex += (InputIsKeyPressed(SDL_SCANCODE_S) || InputIsKeyPressed(SDL_SCANCODE_DOWN)) -
                         (InputIsKeyPressed(SDL_SCANCODE_W) || InputIsKeyPressed(SDL_SCANCODE_UP));

    if (menu->lastIndex != menu->activeIndex && MenuStateGet() != MS_Splash && MenuStateGet() != MS_Name)
    {
        menu->indexChanged = SDL_TRUE;
        SoundStop(&menu->MenuStep);
        SoundPlay(&menu->MenuStep, 0);
    }
    else
    {
        menu->indexChanged = SDL_FALSE;
    }
    menu->lastIndex = menu->activeIndex;

    //Decides what shall be drawn on top
    switch (MenuStateGet())
    {
    case MS_Splash:
        MenuUpdateSplash();
        break;
    case MS_Name:
        MenuUpdateName();
        break;
    case MS_MainMenu:
        MenuUpdateMainMenu();
        break;
    case MS_InGameMenu:
        MenuUpdateInGameMenu();
        break;
    case MS_JoinLobby:
        MenuUpdateJoinLobby();
        break;
    case MS_HostLobby:
        MenuUpdateHostLobby();
        break;
    case MS_RoundsLobby:
        MenuUpdateRoundsLobby();
        break;
    case MS_WaitingForLobby:
        MenuUpdateWaitingForLobby();
        break;
    case MS_Lobby:
        MenuUpdateLobby();
        break;
    case MS_Options:
        MenuUpdateOptions();
        break;
    case MS_Resolution:
        MenuUpdateResolution();
        break;
    case MS_FPS:
        MenuUpdateFPS();
        break;
    case MS_KEYBINDING:
        MenuUpdateKeybinding();
        break;
    case MS_Audio:
        MenuUpdateAudio();
        break;
    case MS_CustomMap:
        MenuUpdateCustomMap();
        break;
    case MS_Skin:
        MenuUpdateSkin();
        break;
    case MS_Summary:
        MenuUpdateSummary();
        break;
    default:
        break;
    }
}

void MenuUpdateSplash()
{
    if (!menu->loadingBar->active)
    {
        if (InputIsAnyKeyDown())
        {
            InputClearPortalContent();
            MenuStateSet(MS_Name);
        }

        menu->mainMenuDbl.dst.w = WindowGetWidth();
        menu->mainMenuDbl.dst.h = WindowGetHeight();
        GraphicsDraw(menu->mainMenuDbl);

        FontDraw3DCustom(TTF_Antilles_XXL, "It's Only a Drill", WindowGetWidth() / 2, WindowGetHeight() / 4, FAL_C, 0, cos(menu->loopCount) * 1.5, sin(menu->loopCount), 10, menu->clr);
        FontDraw(FontGetDynamicSizing(), "Press any key", WindowGetWidth() / 2, WindowGetHeight() / 4 * 3, FAL_C, 0, menu->clr[9]);
    }
    //Fade-in animation
    TransitionDraw();
    LoadingBarUpdate(menu->loadingBar);
    LoadingBarAdd(menu->loadingBar, ClockGetFPS() * 4 / 100);

    if (menu->themecheck == 0)
    {
        MusicPlay(&menu->MenuTheme, 1);
        menu->themecheck++;
    }
}

void MenuUpdateName()
{
    menu->mainMenuDbl.dst.w = WindowGetWidth();
    menu->mainMenuDbl.dst.h = WindowGetHeight();
    GraphicsDraw(menu->mainMenuDbl);

    if (InputIsKeyPressed(SDL_SCANCODE_BACKSPACE))
        InputPortalBackspace();

    FontDraw3DCustom(TTF_Antilles_XXL, InputGetPortalContent(), WindowGetWidth() / 2, WindowGetHeight() / 4, FAL_C, 0, cos(menu->loopCount) * 1.5, sin(menu->loopCount), 10, menu->clr);
    FontDraw(FontGetDynamicSizing(), "Type a name, press [Enter] when done.", WindowGetWidth() / 2, WindowGetHeight() / 4 * 3, FAL_C, 0, menu->clr[9]);

    if (InputIsKeyPressed(SDL_SCANCODE_RETURN))
    {
        ClientSetName(InputGetPortalContent());
        ClientTCPSend(PT_Connect, ClientGetName(), strlen(ClientGetName()) + 1);
        MenuStateSet(MS_MainMenu);
    }
}

void MenuUpdateMainMenu()
{
    if (menu->themecheck == 1)
    {
        MusicPlay(&menu->MenuTheme2, -1);
        menu->themecheck++;
        menu->themecheck2++;
    }
    //Determine menu options
    int optionLength = 7;
    char options[7][100] = {
        {"Join lobby"},
        {"Host lobby"},
        {"Local game"},
        {"Change name"},
        {"Change skin"},
        {"Options"},
        {"Exit"}};
    // makes it loop
    menu->activeIndex = (menu->activeIndex > optionLength - 1) ? 0 : menu->activeIndex;
    menu->activeIndex = (menu->activeIndex < 0) ? optionLength - 1 : menu->activeIndex;

    if (InputIsKeyPressed(SDL_SCANCODE_E) || InputIsKeyPressed(SDL_SCANCODE_RETURN))
    {

        switch (menu->activeIndex)
        {
        case 0:
        {
            if (ConStateGet() == CON_Online)
            {
                MenuStateSet(MS_JoinLobby);
            }
            else
            {
                Notify("You are not connected", 1.0f, NT_WARN);
            }
            break;
        }
        case 1:
        {
            if (ConStateGet() == CON_Online)
            {
                MenuStateSet(MS_HostLobby);
            }
            else
            {
                Notify("You are not connected", 1.0f, NT_WARN);
            }
            break;
        }
        case 2:
        {
            MenuStateSet(MS_CustomMap);
            break;
        }
        case 3:
        {
            MenuStateSet(MS_Name);
            break;
        }
        case 4:
        {
            MenuStateSet(MS_Skin);
            break;
        }
        case 5:
        {
            MenuStateSet(MS_Options);
            break;
        }
        case 6:
        {
        }
        case 7:
        {
            SDL_Event quit;
            quit.type = SDL_QUIT;
            SDL_PushEvent(&quit);
            break;
        }
        default:
            break;
        }
    }
    MenuDraw(options, optionLength);
    MenuTitleDraw("It's Only a Drill");
}

void MenuUpdateInGameMenu()
{
    int optionLength = 3;
    char options[3][100] = {
        {"Resume"},
        {"Options"},
        {"Leave"}};

    menu->activeIndex = (menu->activeIndex > optionLength - 1) ? 0 : menu->activeIndex;
    menu->activeIndex = (menu->activeIndex < 0) ? optionLength - 1 : menu->activeIndex;

    if (!menu->startedInTransition && !menu->startedOutTransition && (InputIsKeyPressed(SDL_SCANCODE_E) || InputIsKeyPressed(SDL_SCANCODE_RETURN)))
    {

        switch (menu->activeIndex)
        {
        case 0:
        {
            if (!menu->startedOutTransition)
            {
                TransitionStart(TT_MenuToMap, 0.30f);
                menu->startedOutTransition = SDL_TRUE;
            }
            break;
        }
        case 1:
        {
            MenuStateSet(MS_Options);
            break;
        }
        case 2:
        {
            MenuStateSet(MS_MainMenu);
            ClientTCPSend(PT_LeaveSession, NULL, 0);
            ClientManagerSetInGame(SDL_FALSE);
            NPCManagerClearNPCS();
            break;
        }
        default:
            break;
        }
    }

    MenuDraw(options, optionLength);

    MenuTitleDraw("Quick Menu");

    MenuDrawPreviewMap();

    MapDraw();

    if (menu->startedInTransition && TransitionIsDone())
    {
        menu->startedInTransition = SDL_FALSE;
    }
    else if (menu->startedOutTransition && TransitionIsDone())
    {
        MenuStateSet(MS_None);
        GameStateSet(GS_Playing);
        MusicStop(&menu->MenuTheme2);
        MusicPlay(&menu->GameTheme, -1);
        menu->themecheck2++;
        menu->startedOutTransition = SDL_FALSE;
    }
    else if ((menu->startedInTransition || menu->startedOutTransition) && !TransitionIsDone())
    {
        CameraSetScale(TransitionGetSaveSlot(0));
        int x = (int)TransitionGetSaveSlot(1);
        int y = (int)TransitionGetSaveSlot(2);
        int w = (int)TransitionGetSaveSlot(3);
        int h = (int)TransitionGetSaveSlot(4);
        CameraSetViewPort((SDL_Rect){x, y, w, h});
        TransitionDraw();
    }
}

void MenuUpdateHostLobby()
{
    //Determine menu options
    int optionLength = MapListGetNumMaps(menu->mapList) + 1;
    char options[optionLength][100];
    for (int i = 0; i < optionLength; i++)
        for (int j = 0; j < 100; j++)
            options[i][j] = 0;

    for (int i = 0, j = 0; j < MapListGetNumMaps(menu->mapList); i++, j++)
    {
        MapInfo mapInfo = MapListGetMaps(menu->mapList)[j];
        sprintf(options[i], "%s", mapInfo.name);
    }
    strcpy(options[optionLength - 1], "Back");

    menu->activeIndex = (menu->activeIndex > optionLength - 1) ? 0 : menu->activeIndex;
    menu->activeIndex = (menu->activeIndex < 0) ? optionLength - 1 : menu->activeIndex;

    if (InputIsKeyPressed(SDL_SCANCODE_E) || InputIsKeyPressed(SDL_SCANCODE_RETURN))
    {
        if (menu->activeIndex == optionLength - 1)
        {
            MenuStateSet(MS_MainMenu);
        }
        else
        {
            MenuStateSet(MS_RoundsLobby);
        }
    }
    else if (menu->indexChanged && menu->activeIndex != optionLength - 1)
    {
        MapInfo mapInfo = MapListGetMaps(menu->mapList)[menu->activeIndex];
        JSON *mapdata = JSONCreate(mapInfo.filename);
        MapGenerateNew(mapdata);
        JSONDestroy(mapdata);
    }

    MenuDraw(options, optionLength);

    if (menu->activeIndex != optionLength - 1)
    {
        MenuDrawPreviewMap();
    }

    if (menu->startedOutTransition && TransitionIsDone())
    {
        MenuStateSet(MS_None);
        GameStateSet(GS_Playing);
        MusicStop(&menu->MenuTheme2);
        MusicPlay(&menu->GameTheme, -1);
        menu->themecheck2++;
        menu->startedOutTransition = SDL_FALSE;
    }
    else if (menu->startedOutTransition && !TransitionIsDone())
    {
        TransitionDraw();
        CameraSetScale(TransitionGetSaveSlot(0));
        int x = (int)TransitionGetSaveSlot(1);
        int y = (int)TransitionGetSaveSlot(2);
        int w = (int)TransitionGetSaveSlot(3);
        int h = (int)TransitionGetSaveSlot(4);
        CameraSetViewPort((SDL_Rect){x, y, w, h});
    }

    MenuTitleDraw("Host Lobby");
}

void MenuUpdateRoundsLobby()
{
    int optionLength = 2;
    char options[2][100] = {
        {"Num rounds"},
        {"Start lobby"}};

    sprintf(options[0], "Rounds: %d", menu->lobbyNumRounds);

    menu->activeIndex = (menu->activeIndex > optionLength - 1) ? 0 : menu->activeIndex;
    menu->activeIndex = (menu->activeIndex < 0) ? optionLength - 1 : menu->activeIndex;

    if (InputIsKeyPressed(SDL_SCANCODE_A) || InputIsKeyPressed(SDL_SCANCODE_LEFT))
    {
        if (menu->activeIndex == 0)
            menu->lobbyNumRounds -= 1;
    }

    if (InputIsKeyPressed(SDL_SCANCODE_D) || InputIsKeyPressed(SDL_SCANCODE_RIGHT))
    {
        if (menu->activeIndex == 0)
            menu->lobbyNumRounds += 1;
    }

    // Loop back
    if (menu->lobbyNumRounds < 1)
        menu->lobbyNumRounds = 1;

    if (InputIsKeyPressed(SDL_SCANCODE_E) || InputIsKeyPressed(SDL_SCANCODE_RETURN))
    {
        if (menu->activeIndex == 1)
        {
            MapInfo mapInfo = MapListGetMaps(menu->mapList)[menu->activeIndex];
            FileIO lfile = FileIOCreate(mapInfo.filename);
            FileIORead(&lfile);

            char *sessionDetails = MALLOC_N(char, sizeof(int) + lfile.size);
            SDL_memcpy(sessionDetails, &menu->lobbyNumRounds, sizeof(int));
            SDL_memcpy(sessionDetails + sizeof(int), lfile.contents, lfile.size);

            ClientTCPSend(PT_CreateSession, sessionDetails, sizeof(int) + lfile.size);

            FREE(sessionDetails);

            MenuStateSet(MS_WaitingForLobby);
        }
    }

    MenuDraw(options, optionLength);
    FontDraw(FontGetDynamicSizing(), "Choose number of rounds with [A]/[D].", WindowGetWidth() / 2, WindowGetHeight() / 4 * 3, FAL_C, 0, menu->clr[9]);
    MenuTitleDraw("Number of rounds");
}

void MenuUpdateJoinLobby()
{
    // Updates menu every 2 seconds
    if (menu->fetchSessionsTimer >= FETCH_SESSIONS_INTERVAL)
    {
        menu->fetchSessionsTimer -= FETCH_SESSIONS_INTERVAL;
        ClientTCPSend(PT_FetchSessions, NULL, 0);
    }
    else
    {
        menu->fetchSessionsTimer += ClockGetDeltaTime();
    }

    //Determine menu options
    int optionLength = ClientManagerGetJoinListSize() + 1;
    char options[optionLength][100];
    for (int i = 0; i < optionLength - 1; i++)
    {
        JoinableSession *jl = &CLIENTMANAGER_JOINLIST[i];
        sprintf(options[i], "%s [%d/%d]", jl->name, jl->currentPlayers, jl->maxPlayers);
    }
    strcpy(options[optionLength - 1], "Back");

    menu->activeIndex = (menu->activeIndex > optionLength - 1) ? 0 : menu->activeIndex;
    menu->activeIndex = (menu->activeIndex < 0) ? optionLength - 1 : menu->activeIndex;

    if (InputIsKeyPressed(SDL_SCANCODE_E) || InputIsKeyPressed(SDL_SCANCODE_RETURN))
    {

        if (menu->activeIndex == optionLength - 1)
        {

            MenuStateSet(MS_MainMenu);
        }
        else
        {
            int sessionID = CLIENTMANAGER_JOINLIST[menu->activeIndex].sessionID;
            ClientTCPSend(PT_JoinSession, &sessionID, sizeof(int));
            MenuStateSet(MS_WaitingForLobby);
        }
    }
    MenuDraw(options, optionLength);
    MenuTitleDraw("Join Lobby");
}

void MenuUpdateWaitingForLobby()
{
    //Add appropriate amount of ., fill with space after for equal text width at all times.
    int optionLength = 1;
    char options[1][100] = {
        {"Loading lobby"}};

    for (int i = 0; i < (int)floor(menu->loopCount / 2); i++)
    {
        strcat(options[0], ".");
    }
    for (int i = (int)floor(menu->loopCount / 2); i < 3; i++)
    {
        strcat(options[0], " ");
    }

    menu->activeIndex = 0;

    // State is changed by ClientManager upon receiving confirmation from server
    // We could add a timeout

    MenuDraw(options, optionLength);
}

void MenuUpdateLobby()
{
    //Draw backdrop
    GraphicsDraw(menu->mainMenuDbl);

    // Updates lobby list every 0.5 seconds
    if (menu->fetchLobbyTimer >= FETCH_LOBBY_INTERVAL)
    {
        menu->fetchLobbyTimer -= FETCH_LOBBY_INTERVAL;
        ClientTCPSend(PT_FetchLobby, NULL, 0);
    }
    else
    {
        menu->fetchLobbyTimer += ClockGetDeltaTime();
    }

    ///
    /// PLAYERLIST
    ///

    //Define playerlist
    int playerListLength = LobbyGetNumNames();
    char playerList[playerListLength][100];

    //Determine playerlist
    for (int i = 0; i < playerListLength; i++)
    {
        sprintf(playerList[i], "%s", LOBBY_NAMES[i].data);
    }

    int spacing = 1.5 * FontGetHeight(FontGetDynamicSizing());

    //Draw playerlist
    for (int i = 0; i < playerListLength; i++)
    {
        int yPos = FontGetHeight(TTF_Antilles_XXL) + 50 + spacing * i;

        if (yPos > WindowGetHeight() - 50 - 2 * spacing)
            continue;

        int xPos = WindowGetWidth() / 20;

        FontDraw3D(FontGetDynamicSizing(), playerList[i], xPos, yPos, FAL_L, 0, 1, F3D_TL, 10, menu->clr);
    }

    ///
    /// OPTIONS
    ///

    int host = (int)LobbyIsHost();

    int optionLength = 1 + host;
    char options[optionLength][100];

    //Rectify options
    menu->activeIndex = (menu->activeIndex > optionLength - 1) ? 0 : menu->activeIndex;
    menu->activeIndex = (menu->activeIndex < 0) ? optionLength - 1 : menu->activeIndex;

    strcpy(options[optionLength - 1 - host], "Leave");
    if (host)
    {
        strcpy(options[optionLength - 1], "Start");
    }

    //Draw lobby options
    for (int i = 0; i < optionLength; i++)
    {
        int yPos = WindowGetHeight() - 50 - 2 * spacing + spacing * i;

        int xPos = WindowGetWidth() / 20;

        if (i == menu->activeIndex)
        {
            int rectWidth = FontGetWidth(FontGetDynamicSizing(), options[i]);
            int rectHeight = FontGetHeight(FontGetDynamicSizing());

            SDL_Rect rect = {xPos - 20, yPos - 15, rectWidth + 40, rectHeight + 30};
            SDL_Color color = {0, 0, 0, 200};
            GraphicsDrawRect(rect, color, SDL_TRUE);

            FontDraw3DCustom(FontGetDynamicSizing(), options[i], xPos, yPos, FAL_L, 0, cos(menu->loopCount) * 1.5, sin(menu->loopCount), 10, menu->clr);
        }
        else
        {
            FontDraw3D(FontGetDynamicSizing(), options[i], xPos, yPos, FAL_L, 0, 1, F3D_TL, 10, menu->clr);
        }
    }

    if (InputIsKeyPressed(SDL_SCANCODE_E) || InputIsKeyPressed(SDL_SCANCODE_RETURN))
    {
        if (menu->activeIndex == optionLength - 1 - (int)host)
        {
            MenuStateSet(MS_MainMenu);
            ClientTCPSend(PT_LeaveSession, NULL, 0);
            LobbyClearNames();
            LobbySetIsHost(SDL_FALSE);
        }
        if (host && menu->activeIndex == optionLength - 1)
        {
            // Start the session
            ClientTCPSend(PT_StartRound, NULL, 0);
        }
    }

    ///
    /// MAP PREVIEW
    ///
    int x = WindowGetWidth() / 2 - 50;
    int y = WindowGetHeight() / 2 - 50 - FontGetHeight(FontGetDynamicSizing()) - 20;
    int w = WindowGetWidth() / 3 + 100;
    int h = WindowGetHeight() / 3 + 120 + FontGetHeight(FontGetDynamicSizing());
    SDL_Rect mapBackgroundRect = {x, y, w, h};
    GraphicsDrawRect(mapBackgroundRect, (SDL_Color){0, 0, 0, 200}, SDL_TRUE);

    MapInfo currentInfo = MapListGetMaps(menu->mapList)[menu->activeIndex];

    char maxPlayers[20];
    sprintf(maxPlayers, "Players: %d", currentInfo.maxPlayers);

    FontDraw3D(FontGetDynamicSizing(), maxPlayers,
               x + (w / 2) - FontGetWidth(FontGetDynamicSizing(), maxPlayers) / 2,
               y + 20,
               FAL_L, 0, 1, F3D_TL, 10, menu->clr);

    MapDraw();

    MenuTitleDraw("Lobby");
}

void MenuUpdateOptions()
{
    //Determine menu options
    int optionLength = 8;
    char options[8][100] = {
        {"Toggle fullscreen"},
        {"Set resolution"},
        {"Toggle vSync"},
        {"SET FPS"},
        {"SET Keybinding"},
        {"Audio options"},
        {"Save options"},
        {"Back"}};

    if (WindowIsFullscreen())
    {
        sprintf(options[0], "Exit fullscreen");
    }
    else
    {
        sprintf(options[0], "Enter fullscreen");
    }

    if (WindowIsVSyncEnabled())
    {
        sprintf(options[2], "vSync on");
    }
    else
    {
        sprintf(options[2], "vSync off");
    }

    menu->activeIndex = (menu->activeIndex > optionLength - 1) ? 0 : menu->activeIndex;
    menu->activeIndex = (menu->activeIndex < 0) ? optionLength - 1 : menu->activeIndex;

    if (InputIsKeyPressed(SDL_SCANCODE_E) || InputIsKeyPressed(SDL_SCANCODE_RETURN))
    {

        switch (menu->activeIndex)
        {
        case 0: //toggle fullscreen
            if (WindowIsFullscreen())
            {
                WindowSetFullscreen(SDL_FALSE);
            }
            else
            {
                WindowSetFullscreen(SDL_TRUE);
                SDL_DisplayMode displaymode;
                SDL_GetCurrentDisplayMode(0, &displaymode);
                WindowSetSize(displaymode.w, displaymode.h);
            }
            break;
        case 1: //set resolution
        {
            MenuStateSet(MS_Resolution);
        }
        break;
        case 2: //Vsync
        {
            if (WindowIsVSyncEnabled())
            {
                WindowSetVSync(SDL_FALSE);
            }
            else
            {
                WindowSetVSync(SDL_TRUE);
            }
        }
        break;
        case 3:
        {
            MenuStateSet(MS_FPS);
            break;
        }
        case 4:
        {
            MenuStateSet(MS_KEYBINDING);
            break;
        }
        case 5:
        {
            MenuStateSet(MS_Audio);
            break;
        }
        case 6:
        {
            SettingsGenerate();
            SettingsSave(SETTINGS_PATH);
            Notify("Settings saved", 2.0f, NT_GOOD);
            break;
        }
        case 7:
        {

            if (ClientManagerIsInGame())
            {
                MenuStateSet(MS_InGameMenu);
            }
            else
            {
                MenuStateSet(MS_MainMenu);
            }
            break;
        }
        }
    }

    MenuDraw(options, optionLength);
    MenuTitleDraw("Options");
}

void MenuUpdateResolution()
{
    //Determine menu options
    int optionLength = 6;
    char options[6][100] = {
        {"640x480"},
        {"1280x720"},
        {"1920x1080"},
        {"1920x1200"},
        {"2560x1440"},
        {"Back"}};

    menu->activeIndex = (menu->activeIndex > optionLength - 1) ? 0 : menu->activeIndex;
    menu->activeIndex = (menu->activeIndex < 0) ? optionLength - 1 : menu->activeIndex;

    if (InputIsKeyPressed(SDL_SCANCODE_E) || InputIsKeyPressed(SDL_SCANCODE_RETURN))
    {

        int width = -1;
        int height = -1;
        switch (menu->activeIndex)
        {
        case 0:
            width = 640;
            height = 480;
            WindowSetSize(width, height);
            break;
        case 1:
            width = 1280;
            height = 720;
            WindowSetSize(width, height);
            break;
        case 2:
            width = 1920;
            height = 1080;
            WindowSetSize(width, height);
            break;
        case 3:
            width = 1920;
            height = 1200;
            WindowSetSize(width, height);
            break;
        case 4:
            width = 2560;
            height = 1440;
            WindowSetSize(width, height);
            break;
        case 5:
        {

            MenuStateSet(MS_Options);
            break;
        }
        }
    }
    MenuDraw(options, optionLength);
    MenuTitleDraw("Resolution");
}

void MenuUpdateFPS()
{
    //Determine menu options
    int optionLength = 6;
    char options[6][100] = {
        {"30 FPS"},
        {"60 FPS"},
        {"90 FPS"},
        {"120 FPS"},
        {"unlimited"},
        {"Back"}};

    menu->activeIndex = (menu->activeIndex > optionLength - 1) ? 0 : menu->activeIndex;
    menu->activeIndex = (menu->activeIndex < 0) ? optionLength - 1 : menu->activeIndex;

    if (InputIsKeyPressed(SDL_SCANCODE_E) || InputIsKeyPressed(SDL_SCANCODE_RETURN))
    {

        switch (menu->activeIndex)
        {
        case 0:
        {
            FPSManagerSetDesiredFPS(30);
            break;
        }
        case 1:
        {
            FPSManagerSetDesiredFPS(60);
            break;
        }
        break;
        case 2:
        {
            FPSManagerSetDesiredFPS(90);
            break;
        }
        case 3:
        {
            FPSManagerSetDesiredFPS(120);
            break;
        }
        case 4:
        {
            FPSManagerSetDesiredFPS(0); // Zero = inactivate
            break;
        }
        case 5:
        {

            MenuStateSet(MS_Options);
            break;
        }
        }
    }
    MenuDraw(options, optionLength);
    MenuTitleDraw("FPS");
}

void MenuUpdateKeybinding()
{
    //Determine menu options
    int optionLength = 10;
    char options[10][100] = {
        {"Move Up"},
        {"Move Left"},
        {"Move Right"},
        {"Move Down"},
        {"Pickup"},
        {"Drop"},
        {"Shoot"},
        {"Reload"},
        {"Inventory"},
        {"Back"}};

    menu->activeIndex = (menu->activeIndex > optionLength - 1) ? 0 : menu->activeIndex;
    menu->activeIndex = (menu->activeIndex < 0) ? optionLength - 1 : menu->activeIndex;

    if (InputIsKeyPressed(SDL_SCANCODE_E) || InputIsKeyPressed(SDL_SCANCODE_RETURN))
    {
        menu->keybindingstate = 1;
    }

    if (menu->keybindingstate == 1)
    {
        if (InputIsKeyPressed(SDL_SCANCODE_E) || InputIsKeyPressed(SDL_SCANCODE_RETURN))
        {
        }
        else
        {

            switch (menu->activeIndex)
            {
            case 0:
            {
                KeybindingChange(AC_MOVE_UP, InputLastKeyDown(KEYBINDINGS_KEYS[AC_MOVE_UP]));
                menu->keybindingstate = 0;
                break;
            }
            case 1:
            {
                KeybindingChange(AC_MOVE_LEFT, InputLastKeyDown(KEYBINDINGS_KEYS[AC_MOVE_LEFT]));
                menu->keybindingstate = 0;
                break;
            }
            case 2:
            {
                KeybindingChange(AC_MOVE_RIGHT, InputLastKeyDown(KEYBINDINGS_KEYS[AC_MOVE_RIGHT]));
                menu->keybindingstate = 0;
                break;
            }
            case 3:
            {
                KeybindingChange(AC_MOVE_DOWN, InputLastKeyDown(KEYBINDINGS_KEYS[AC_MOVE_DOWN]));
                menu->keybindingstate = 0;
                break;
            }
            case 4:
            {
                KeybindingChange(AC_PICKUP, InputLastKeyDown(KEYBINDINGS_KEYS[AC_PICKUP]));
                menu->keybindingstate = 0;
                break;
            }
            case 5:
            {
                KeybindingChange(AC_DROP, InputLastKeyDown(KEYBINDINGS_KEYS[AC_DROP]));
                menu->keybindingstate = 0;
                break;
            }
            case 6:
            {
                KeybindingChange(AC_SHOOT, InputLastKeyDown(KEYBINDINGS_KEYS[AC_SHOOT]));
                menu->keybindingstate = 0;
                break;
            }
            case 7:
            {
                KeybindingChange(AC_RELOAD, InputLastKeyDown(KEYBINDINGS_KEYS[AC_RELOAD]));
                menu->keybindingstate = 0;
                break;
            }
            case 8:
            {
                KeybindingChange(AC_INVENTORY, InputLastKeyDown(KEYBINDINGS_KEYS[AC_INVENTORY]));
                menu->keybindingstate = 0;

                break;
            }
            case 9:
            {
                menu->keybindingstate = 0;

                MenuStateSet(MS_Options);
                break;
            }
            }
        }
    }
    MenuDraw(options, optionLength);
    MenuTitleDraw("Keybindings");
}

void MenuUpdateAudio()
{
    int step = 4;

    // VOLUME SETTINGS
    // VOLUME CHANNELS: MUSIC, SFX
    // VOLUME PROPERTIES: 0-128 (Uint8 format)
    //
    // MASTER VOLUME: MULTIPLIER

    //Determine menu options
    int optionLength = 4;
    char options[4][100] = {
        {"Master"},
        {"SFX"},
        {"Music"},
        {"Back"}};

    sprintf(options[0], "Master: %d%%", (int)(AudioGetMasterVolume() * 100));
    sprintf(options[1], "SFX: %u/64", AudioGetSFXVolume());
    sprintf(options[2], "Music: %u/64", AudioGetMusicVolume());

    menu->activeIndex = (menu->activeIndex > optionLength - 1) ? 0 : menu->activeIndex;
    menu->activeIndex = (menu->activeIndex < 0) ? optionLength - 1 : menu->activeIndex;

    if (InputIsKeyPressed(SDL_SCANCODE_A) || InputIsKeyPressed(SDL_SCANCODE_LEFT))
    {
        switch (menu->activeIndex)
        {
        case 0:
            AudioSetMasterVolume(AudioGetMasterVolume() - 0.1);
            break;
        case 1:
            AudioSetSFXVolume(AudioGetSFXVolume() - step);
            break;
        case 2:
            AudioSetMusicVolume(AudioGetMusicVolume() - step);
            break;
        }
    }

    if (InputIsKeyPressed(SDL_SCANCODE_D) || InputIsKeyPressed(SDL_SCANCODE_RIGHT))
    {
        switch (menu->activeIndex)
        {
        case 0:
            AudioSetMasterVolume(AudioGetMasterVolume() + 0.1);
            break;
        case 1:
            AudioSetSFXVolume(AudioGetSFXVolume() + step);
            break;
        case 2:
            AudioSetMusicVolume(AudioGetMusicVolume() + step);
            break;
        }
    }

    if (InputIsKeyPressed(SDL_SCANCODE_E) || InputIsKeyPressed(SDL_SCANCODE_RETURN))
    {
        switch (menu->activeIndex)
        {
        case 3:
        {
            MenuStateSet(MS_Options);
            break;
        }
        }
    }

    MenuDraw(options, optionLength);
    MenuTitleDraw("Audio options");
}

void MenuUpdateCustomMap()
{
    //Determine menu options
    int optionLength = MapListGetNumMaps(menu->mapList) + 1;
    char options[optionLength][100];
    for (int i = 0; i < optionLength; i++)
        for (int j = 0; j < 100; j++)
            options[i][j] = 0;

    for (int i = 0, j = 0; j < MapListGetNumMaps(menu->mapList); i++, j++)
    {
        MapInfo mapInfo = MapListGetMaps(menu->mapList)[j];
        sprintf(options[i], "%s", mapInfo.name);
    }
    strcpy(options[optionLength - 1], "Back");

    menu->activeIndex = (menu->activeIndex > optionLength - 1) ? 0 : menu->activeIndex;
    menu->activeIndex = (menu->activeIndex < 0) ? optionLength - 1 : menu->activeIndex;

    if (!menu->startedOutTransition && (InputIsKeyPressed(SDL_SCANCODE_E) || InputIsKeyPressed(SDL_SCANCODE_RETURN)))
    {
        if (menu->activeIndex == optionLength - 1)
        {
            MenuStateSet(MS_MainMenu);
        }
        else
        {
            if (!menu->startedOutTransition)
            {
                TransitionStart(TT_MenuToMap, 0.25f);
                menu->startedOutTransition = SDL_TRUE;
            }
        }
    }
    else if (menu->indexChanged && menu->activeIndex != optionLength - 1)
    {
        MapInfo mapInfo = MapListGetMaps(menu->mapList)[menu->activeIndex];
        JSON *mapdata = JSONCreate(mapInfo.filename);
        MapGenerateNew(mapdata);
        JSONDestroy(mapdata);
        NPCManagerClearNPCS();
        for (int i = 0; i < mapInfo.enemySpawns->size; i++)
            NPCManagerAddNew(MapInfoGetEnemySpawns(&mapInfo)[i].position);
        Entity *playerEntity = PlayerGetEntity();
        *playerEntity = EntityCreate(Vec2Create(0.0f, 0.0f), ET_Player, playerEntity->id);
        if (mapInfo.enemySpawns->size > 0)
            PlayerGetEntity()->position = MapInfoGetPlayerSpawns(&mapInfo)[0].position;
        else
            PlayerGetEntity()->position = Vec2Create(1000.0f, 1000.0f);
    }

    MenuDraw(options, optionLength);

    if (menu->activeIndex != optionLength - 1)
    {
        MenuDrawPreviewMap();
    }

    if (menu->startedOutTransition && TransitionIsDone())
    {
        MenuStateSet(MS_None);
        GameStateSet(GS_Playing);
        MusicStop(&menu->MenuTheme2);
        MusicPlay(&menu->GameTheme, -1);
        menu->themecheck2++;
        menu->startedOutTransition = SDL_FALSE;
        // To make in-game menu work properly
        ClientManagerSetInGame(SDL_TRUE);
    }
    else if (menu->startedOutTransition && !TransitionIsDone())
    {
        TransitionDraw();
        CameraSetScale(TransitionGetSaveSlot(0));
        int x = (int)TransitionGetSaveSlot(1);
        int y = (int)TransitionGetSaveSlot(2);
        int w = (int)TransitionGetSaveSlot(3);
        int h = (int)TransitionGetSaveSlot(4);
        CameraSetViewPort((SDL_Rect){x, y, w, h});
    }

    MenuTitleDraw("Local game");
}

void MenuUpdateSkin()
{
    //Determine menu options
    int optionLength = 7;
    char options[7][100] = {
        {"Prisoner"},
        {"Chernobyl Worker"},
        {"Iron Man"},
        {"iDubbbz"},
        {"Old man"},
        {"Sonic"},
        {"Back"}};

    menu->activeIndex = (menu->activeIndex > optionLength - 1) ? 0 : menu->activeIndex;
    menu->activeIndex = (menu->activeIndex < 0) ? optionLength - 1 : menu->activeIndex;

    // Draw player skin next to menu
    switch (menu->activeIndex)
    {
    case 0:
    {
        SpriteSheet spriteSheet = SS_Character_Prisoner;
        AnimChangeSpriteSheet(&menu->previewLeg, spriteSheet);
        AnimChangeSpriteSheet(&menu->previewBody, spriteSheet);
        break;
    }
    case 1:
    {
        SpriteSheet spriteSheet = SS_Character_ChernobylWorker;
        AnimChangeSpriteSheet(&menu->previewLeg, spriteSheet);
        AnimChangeSpriteSheet(&menu->previewBody, spriteSheet);
        break;
    }
    case 2:
    {
        SpriteSheet spriteSheet = SS_Character_IronMan;
        AnimChangeSpriteSheet(&menu->previewLeg, spriteSheet);
        AnimChangeSpriteSheet(&menu->previewBody, spriteSheet);
        break;
    }
    case 3:
    {
        SpriteSheet spriteSheet = SS_Character_iDubbbz;
        AnimChangeSpriteSheet(&menu->previewLeg, spriteSheet);
        AnimChangeSpriteSheet(&menu->previewBody, spriteSheet);
        break;
    }
    case 4:
    {
        SpriteSheet spriteSheet = SS_Character_OldMan;
        AnimChangeSpriteSheet(&menu->previewLeg, spriteSheet);
        AnimChangeSpriteSheet(&menu->previewBody, spriteSheet);
        break;
    }
    case 5:
    {
        SpriteSheet spriteSheet = SS_Character_Sonic;
        AnimChangeSpriteSheet(&menu->previewLeg, spriteSheet);
        AnimChangeSpriteSheet(&menu->previewBody, spriteSheet);
        break;
    }
    }
    int h = WindowGetHeight() / 3 * 2;

    menu->previewLeg.active->dst.x = WindowGetWidth() - h - 100;
    menu->previewLeg.active->dst.y = (WindowGetHeight() - h) / 2;
    menu->previewBody.active->dst.x = WindowGetWidth() - h - 100;
    menu->previewBody.active->dst.y = (WindowGetHeight() - h) / 2;

    menu->previewLeg.active->dst.w = h;
    menu->previewLeg.active->dst.h = h;
    menu->previewBody.active->dst.w = h;
    menu->previewBody.active->dst.h = h;

    menu->previewBody.active->rot_anchor = Vec2Create(0.5f, 0.5f);
    menu->previewLeg.active->rot_anchor = Vec2Create(0.5f, 0.5f);

    menu->previewBody.active->rot = 180;
    menu->previewLeg.active->rot = 180;

    // Change skin on click
    if (InputIsKeyPressed(SDL_SCANCODE_E) || InputIsKeyPressed(SDL_SCANCODE_RETURN))
    {
        switch (menu->activeIndex)
        {
        case 0:
        {
            SpriteSheet spriteSheet = SS_Character_Prisoner;
            ClientTCPSend(PT_ChangeSkin, (void *)&spriteSheet, sizeof(SpriteSheet));
            PlayerSetSpriteSheet(spriteSheet);
            MenuStateSet(MS_MainMenu);
            break;
        }
        case 1:
        {
            SpriteSheet spriteSheet = SS_Character_ChernobylWorker;
            ClientTCPSend(PT_ChangeSkin, (void *)&spriteSheet, sizeof(SpriteSheet));
            PlayerSetSpriteSheet(spriteSheet);

            MenuStateSet(MS_MainMenu);
            break;
        }
        case 2:
        {
            SpriteSheet spriteSheet = SS_Character_IronMan;
            ClientTCPSend(PT_ChangeSkin, (void *)&spriteSheet, sizeof(SpriteSheet));
            PlayerSetSpriteSheet(spriteSheet);

            MenuStateSet(MS_MainMenu);
            break;
        }
        case 3:
        {
            SpriteSheet spriteSheet = SS_Character_iDubbbz;
            ClientTCPSend(PT_ChangeSkin, (void *)&spriteSheet, sizeof(SpriteSheet));
            PlayerSetSpriteSheet(spriteSheet);

            MenuStateSet(MS_MainMenu);
            break;
        }
        case 4:
        {
            SpriteSheet spriteSheet = SS_Character_OldMan;
            ClientTCPSend(PT_ChangeSkin, (void *)&spriteSheet, sizeof(SpriteSheet));
            PlayerSetSpriteSheet(spriteSheet);

            MenuStateSet(MS_MainMenu);
            break;
        }
        case 5:
        {
            SpriteSheet spriteSheet = SS_Character_Sonic;
            ClientTCPSend(PT_ChangeSkin, (void *)&spriteSheet, sizeof(SpriteSheet));
            PlayerSetSpriteSheet(spriteSheet);

            MenuStateSet(MS_MainMenu);
            break;
        }
        case 6:
        {
            MenuStateSet(MS_MainMenu);
            break;
        }
        }
    }
    MenuDraw(options, optionLength);
    MenuTitleDraw("Choose skin");

    // If you are heading out, don't draw the character again as it will be a "dirty frame"
    if (MenuStateGet() == MS_MainMenu)
        return;

    if (menu->activeIndex == optionLength - 1)
        return;
    GraphicsDraw(*menu->previewLeg.active);
    GraphicsDraw(*menu->previewBody.active);
}

void MenuUpdateSummary()
{

    menu->mainMenuDbl.dst.w = WindowGetWidth();
    menu->mainMenuDbl.dst.h = WindowGetHeight();
    GraphicsDraw(menu->mainMenuDbl);

    ScoreboardEntry *allScores = ScoreboardGetAllScores();

    int playerListLength = 20;
    if (ScoreboardGetNumPlayers() < 20)
        playerListLength = ScoreboardGetNumPlayers();

    char playerList[playerListLength][100];

    int playerPlace = 0;
    for (int i = 0; i < ScoreboardGetNumPlayers(); i++)
    {
        if (allScores[i].id == PlayerGetEntity()->id)
        {
            playerPlace = i;
        }
    }

    //Determine playerlist
    char playerPlaceStr[100];
    sprintf(playerPlaceStr, "Finished %d", playerPlace + 1);
    MenuTitleDraw(playerPlaceStr);

    for (int i = 0; i < playerListLength; i++)
    {
        sprintf(playerList[i], "%d: %s [%d pts]", i + 1, allScores[i].name, allScores[i].score);
    }

    int spacing = 1.5 * FontGetHeight(FontGetDynamicSizing());

    //Draw playerlist
    for (int i = 0; i < playerListLength; i++)
    {
        int yPos = FontGetHeight(TTF_Antilles_XXL) + 50 + spacing * i;

        if (yPos > WindowGetHeight() - 50 - 2 * spacing)
            continue;

        int xPos = WindowGetWidth() / 20;

        FontDraw3D(FontGetDynamicSizing(), playerList[i], xPos, yPos, FAL_L, 0, 1, F3D_TL, 10, menu->clr);
    }

    FontCachedDraw(FontGetDynamicSizing(), "Press [Enter] to return to the Main Menu.", WindowGetWidth() / 2, WindowGetHeight() / 4 * 3, FAL_C, 0, menu->clr[9]);

    if (InputIsKeyPressed(SDL_SCANCODE_RETURN))
    {
        MenuStateSet(MS_MainMenu);
        ClientManagerSetInGame(SDL_FALSE);
    }
}

void MenuTitleDraw(char title[100])
{
    FontDraw3DCustom(TTF_Antilles_XXL, title, WindowGetWidth() / 20, 25, FAL_L, 0, cos(menu->loopCount) * 3, sin(menu->loopCount) * 3, 10, menu->clr);
}

void MenuDrawPreviewMap()
{
    int x = WindowGetWidth() / 2 - 50;
    int y = WindowGetHeight() / 2 - 50 - FontGetHeight(FontGetDynamicSizing()) - 20;
    int w = WindowGetWidth() / 3 + 100;
    int h = WindowGetHeight() / 3 + 120 + FontGetHeight(FontGetDynamicSizing());
    SDL_Rect mapBackgroundRect = {x, y, w, h};
    if (menu->startedOutTransition || menu->startedInTransition)
    {
        int x_t = (int)TransitionGetSaveSlot(1) - 50;
        int y_t = (int)TransitionGetSaveSlot(2) - 50 - FontGetHeight(FontGetDynamicSizing()) - 20;
        int w_t = (int)TransitionGetSaveSlot(3) + 100;
        int h_t = (int)TransitionGetSaveSlot(4) + 120 + FontGetHeight(FontGetDynamicSizing());
        GraphicsDrawRect((SDL_Rect){x_t, y_t, w_t, h_t}, (SDL_Color){0, 0, 0, (int)TransitionGetSaveSlot(5)}, SDL_TRUE);
    }
    else
    {
        GraphicsDrawRect(mapBackgroundRect, (SDL_Color){0, 0, 0, 200}, SDL_TRUE);
    }

    MapInfo currentInfo = MapListGetMaps(menu->mapList)[menu->activeIndex];

    if (!menu->startedOutTransition && !menu->startedInTransition)
    {
        char maxPlayers[20];
        sprintf(maxPlayers, "Players: %d", currentInfo.maxPlayers);
        FontDraw3D(FontGetDynamicSizing(), maxPlayers,
                   x + (w / 2) - FontGetWidth(FontGetDynamicSizing(), maxPlayers) / 2,
                   y + 20,
                   FAL_L, 0, 1, F3D_TL, 10, menu->clr);
    }
    MapDraw();
}

void MenuDraw(char options[][100], int optionLength)
{
    //Get windows size
    menu->mainMenuDbl.dst.w = WindowGetWidth();
    menu->mainMenuDbl.dst.h = WindowGetHeight();

    //Draw background
    GraphicsDraw(menu->mainMenuDbl);

    //Get font spacing
    //*1.5 factor for space between lines
    int spacing = 1.5 * FontGetHeight(FontGetDynamicSizing());

    //Draw menu options
    for (int i = 0; i < optionLength; i++)
    {
        //Calculate scrolling menu
        int screenCenterY = WindowGetHeight() / 2;
        int yPos;

        if (spacing * optionLength > WindowGetHeight() - FontGetHeight(TTF_Antilles_XXL) - 50)
        {
            yPos = screenCenterY - (spacing * menu->activeIndex) + spacing * i;
        }
        else
        {
            yPos = FontGetHeight(TTF_Antilles_XXL) + 50 + spacing * i;
        }

        if (yPos + FontGetHeight(FontGetDynamicSizing()) < FontGetHeight(TTF_Antilles_XXL) + 50)
            continue;

        int xPos = WindowGetWidth() / 20; //WindowGetWidth() / 2

        if (i == menu->activeIndex)
        {
            int rectWidth = FontGetWidth(FontGetDynamicSizing(), options[i]);
            int rectHeight = FontGetHeight(FontGetDynamicSizing());

            SDL_Rect rect = {xPos - 20, yPos - 15, rectWidth + 40, rectHeight + 30};
            SDL_Color color = {0, 0, 0, 200};
            GraphicsDrawRect(rect, color, SDL_TRUE);
            FontDraw3DCustom(FontGetDynamicSizing(), options[i], xPos, yPos, FAL_L, 0, cos(menu->loopCount) * 1.5, sin(menu->loopCount), 10, menu->clr);
        }
        else
        {
            FontDraw3D(FontGetDynamicSizing(), options[i], xPos, yPos, FAL_L, 0, 1, F3D_TL, 10, menu->clr);
        }
    }
}