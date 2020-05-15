#include "Menu.h"

#include "Library.h"

Menu *MenuCreate(Graphics *gfx, Camera *camera, Font *font, Keybinding *bindings, MapList *mapList)
{
    Menu *menu = MALLOC(Menu);
    menu->gfx = gfx;
    menu->camera = camera;
    menu->font = font;
    menu->loadingBar = LoadingBarCreate(menu->gfx);
    menu->bindings = bindings;
    menu->loopCount = 0;
    menu->loopSwing = 87;
    menu->swingDir = 0;
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
    SDL_Rect dst = {0, 0, gfx->window->width, gfx->window->height};
    menu->mainMenuDbl = DrawableCreate(src, dst, SS_Menu);
    menu->lobbyNormalDbl = DrawableCreate((SDL_Rect){0, 0, 1920, 1080}, dst, SS_Lobby);
    menu->lobbyHostDbl = DrawableCreate((SDL_Rect){0, 0, 1920, 1080}, dst, SS_Lobby);
    TransitionInitalize(menu->gfx, menu->font);
    TransitionStart(TT_FadeOut, 2);
    LoadingBarShow(menu->loadingBar);

    menu->MenuStep = SoundCreate(SF_MenuStep);
    menu->MenuTheme = MusicCreate(MF_MainMusic);
    menu->themecheck = 0;

    menu->previewLeg = AnimCreate(AN_PlayerLegs, ANRO_RepeatFromEnd, SS_None, 4, 0.05f);
    menu->previewBody = AnimCreate(AN_PlayerBody, ANRO_RepeatFromEnd, SS_None, 4, 0.05f);

    menu->mapList = mapList;

    menu->volumeMaster = 1;
    menu->volumeSFX = 64;
    menu->volumeMusic = 64;

    return menu;
}

void MenuUpdate(Menu *menu, FPSManager *fpsManager, Player *player)
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
        menu->loopCount += .1f;
    }
    else
    {
        menu->loopCount = 0;
    }

    if (menu->loopSwing >= 255)
        menu->swingDir = 0; // go back down

    if (menu->loopSwing <= 200)
        menu->swingDir = 1; //go back up

    if (menu->swingDir)
    {
        menu->loopSwing++;
    }
    else
    {
        menu->loopSwing--;
    }

    //Update text colors
    menu->clr[0] = (SDL_Color){menu->loopSwing, 159, 227, 255};
    menu->clr[1] = (SDL_Color){menu->loopSwing, 139, 207, 255};
    menu->clr[2] = (SDL_Color){menu->loopSwing, 119, 187, 255};
    menu->clr[3] = (SDL_Color){menu->loopSwing, 99, 167, 255};
    menu->clr[4] = (SDL_Color){menu->loopSwing, 79, 147, 255};
    menu->clr[5] = (SDL_Color){menu->loopSwing, 59, 127, 255};
    menu->clr[6] = (SDL_Color){menu->loopSwing, 39, 107, 255};
    menu->clr[7] = (SDL_Color){menu->loopSwing, 19, 87, 255};
    menu->clr[8] = (SDL_Color){255 - menu->loopSwing, 180, 184, 255};
    menu->clr[9] = (SDL_Color){255 - menu->loopSwing, 180, 184, 255};

    AnimUpdate(&menu->previewBody, ClockGetDeltaTime());
    AnimUpdate(&menu->previewLeg, ClockGetDeltaTime());

    //Get input
    menu->activeIndex += (InputIsKeyPressed(SDL_SCANCODE_S) || InputIsKeyPressed(SDL_SCANCODE_DOWN)) -
                         (InputIsKeyPressed(SDL_SCANCODE_W) || InputIsKeyPressed(SDL_SCANCODE_UP));

    if (menu->lastIndex != menu->activeIndex)
    {
        menu->indexChanged = SDL_TRUE;
        SoundPlay(&menu->MenuStep, 0);
    }
    else
    {
        menu->indexChanged = SDL_FALSE;
    }
    menu->lastIndex = menu->activeIndex;

    //actually do some shit with the volumes?
    AudioSetSFX(menu->volumeSFX);
    AudioSetMusic(menu->volumeMusic);

    //MASTER LAST
    AudioSetMaster(menu->volumeMaster);

    //Decides what shall be drawn on top
    switch (MenuStateGet())
    {
    case MS_Splash:
        MenuUpdateSplash(menu);
        break;
    case MS_Name:
        MenuUpdateName(menu);
        break;
    case MS_MainMenu:
        MenuUpdateMainMenu(menu, player, fpsManager);
        break;
    case MS_InGameMenu:
        MenuUpdateInGameMenu(menu);
        break;
    case MS_JoinLobby:
        MenuUpdateJoinLobby(menu);
        break;
    case MS_HostLobby:
        MenuUpdateHostLobby(menu);
        break;
    case MS_WaitingForLobby:
        MenuUpdateWaitingForLobby(menu);
        break;
    case MS_Lobby:
        MenuUpdateLobby(menu);
        break;
    case MS_Options:
        MenuUpdateOptions(menu);
        break;
    case MS_Resolution:
        MenuUpdateResolution(menu);
        break;
    case MS_FPS:
        MenuUpdateFPS(menu, fpsManager);
        break;
    case MS_KEYBINDING:
        MenuUpdateKeybinding(menu);
        break;
    case MS_Audio:
        MenuUpdateAudio(menu);
        break;
    case MS_CustomMap:
        MenuUpdateCustomMap(menu, player);
        break;
    case MS_Skin:
        MenuUpdateSkin(menu, player);
        break;
    default:
        break;
    }
}

void MenuUpdateSplash(Menu *menu)
{
    if (!menu->loadingBar->active)
    {
        if (InputIsAnyKeyDown())
        {
            InputClearPortalContent();
            MenuStateSet(MS_Name);
        }

        GraphicsDraw(menu->gfx, menu->mainMenuDbl);

        FontDraw3DCustom(menu->font, TTF_Antilles_XXL, "It's Only a Drill", menu->gfx->window->width / 2, menu->gfx->window->height / 4, FAL_C, 0, cos(menu->loopCount) * 1.5, sin(menu->loopCount), 10, menu->clr);
        FontDraw(menu->font, FontGetDynamicSizing(menu->font), "Press any key", menu->gfx->window->width / 2, menu->gfx->window->height / 4 * 3, FAL_C, 0, menu->clr[9]);
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

void MenuUpdateName(Menu *menu)
{
    GraphicsDraw(menu->gfx, menu->mainMenuDbl);

    if (InputIsKeyPressed(SDL_SCANCODE_BACKSPACE))
        InputPortalBackspace();

    FontDraw3DCustom(menu->font, TTF_Antilles_XXL, InputGetPortalContent(), menu->gfx->window->width / 2, menu->gfx->window->height / 4, FAL_C, 0, cos(menu->loopCount) * 1.5, sin(menu->loopCount), 10, menu->clr);
    FontDraw(menu->font, FontGetDynamicSizing(menu->font), "Type a name, press [Enter] when done.", menu->gfx->window->width / 2, menu->gfx->window->height / 4 * 3, FAL_C, 0, menu->clr[9]);

    if (InputIsKeyPressed(SDL_SCANCODE_RETURN))
    {
        strcpy(client.name, InputGetPortalContent());
        ClientTCPSend(PT_Connect, client.name, strlen(client.name) + 1);
        MenuStateSet(MS_MainMenu);
    }
}

void MenuUpdateMainMenu(Menu *menu, Player *player, FPSManager *fps)
{
    //Determine menu options
    int optionLength = 8;
    char options[8][100] = {
        {"Join lobby"},
        {"Host lobby"},
        {"Local game"},
        {"Change name"},
        {"Change skin"},
        {"Options"},
        {"Save options"},
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
            Settings settings = SettingsCreate((int)PlayerGetEntity(player)->drawables[0].spriteSheet,
                                               menu->gfx->window->width,
                                               menu->gfx->window->height, *menu->bindings,
                                               menu->gfx->window->isFullscreen,
                                               menu->gfx->window->vsyncEnabled,
                                               fps->desiredFPS);
            SettingsSave(settings);
            break;
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
    MenuDraw(menu, options, optionLength);
    MenuTitleDraw(menu, "It's Only a Drill");
}

void MenuUpdateInGameMenu(Menu *menu)
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
            clientManager.inGame = SDL_FALSE;
            NPCManagerClearNPCS();
            break;
        }
        default:
            break;
        }
    }

    MenuDraw(menu, options, optionLength);

    MenuTitleDraw(menu, "Quick Menu");

    MenuDrawPreviewMap(menu);

    MapDraw(menu->camera);

    if (menu->startedInTransition && TransitionIsDone())
    {
        menu->startedInTransition = SDL_FALSE;
    }
    else if (menu->startedOutTransition && TransitionIsDone())
    {
        MenuStateSet(MS_None);
        GameStateSet(GS_Playing);
        menu->startedOutTransition = SDL_FALSE;
    }
    else if ((menu->startedInTransition || menu->startedOutTransition) && !TransitionIsDone())
    {
        CameraSetScale(menu->camera, TransitionGetSaveSlot(0));
        int x = (int)TransitionGetSaveSlot(1);
        int y = (int)TransitionGetSaveSlot(2);
        int w = (int)TransitionGetSaveSlot(3);
        int h = (int)TransitionGetSaveSlot(4);
        CameraSetViewPort(menu->camera, (SDL_Rect){x, y, w, h});
        TransitionDraw();
    }
}

void MenuUpdateHostLobby(Menu *menu)
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
            MapInfo mapInfo = MapListGetMaps(menu->mapList)[menu->activeIndex];
            FileIO lfile = FileIOCreate(mapInfo.filename);
            FileIORead(&lfile);

            ClientTCPSend(PT_CreateSession, lfile.contents, lfile.size);

            MenuStateSet(MS_WaitingForLobby);
        }
    }
    else if (menu->indexChanged && menu->activeIndex != optionLength - 1)
    {
        MapInfo mapInfo = MapListGetMaps(menu->mapList)[menu->activeIndex];
        JSON *mapdata = JSONCreate(mapInfo.filename);
        MapGenerateNew(mapdata);
        JSONDestroy(mapdata);
    }

    MenuDraw(menu, options, optionLength);

    if (menu->activeIndex != optionLength - 1)
    {
        MenuDrawPreviewMap(menu);
    }

    if (menu->startedOutTransition && TransitionIsDone())
    {
        MenuStateSet(MS_None);
        GameStateSet(GS_Playing);
        MusicStop(&menu->MenuTheme);
        menu->startedOutTransition = SDL_FALSE;
    }
    else if (menu->startedOutTransition && !TransitionIsDone())
    {
        TransitionDraw();
        CameraSetScale(menu->camera, TransitionGetSaveSlot(0));
        int x = (int)TransitionGetSaveSlot(1);
        int y = (int)TransitionGetSaveSlot(2);
        int w = (int)TransitionGetSaveSlot(3);
        int h = (int)TransitionGetSaveSlot(4);
        CameraSetViewPort(menu->camera, (SDL_Rect){x, y, w, h});
    }

    MenuTitleDraw(menu, "Host Lobby");
}

void MenuUpdateJoinLobby(Menu *menu)
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
    int optionLength = clientManager.joinList->size + 1;
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
    MenuDraw(menu, options, optionLength);
    MenuTitleDraw(menu, "Join Lobby");
}

void MenuUpdateWaitingForLobby(Menu *menu)
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

    MenuDraw(menu, options, optionLength);
}

void MenuUpdateLobby(Menu *menu)
{
    //Draw backdrop
    GraphicsDraw(menu->gfx, menu->mainMenuDbl);

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
    int playerListLength = lobby.names->size;
    char playerList[playerListLength][100];

    //Determine playerlist
    for (int i = 0; i < playerListLength; i++)
    {
        sprintf(playerList[i], "%s", LOBBY_NAMES[i].data);
    }

    int spacing = 1.5 * FontGetHeight(FontGetDynamicSizing(menu->font));

    //Draw playerlist
    for (int i = 0; i < playerListLength; i++)
    {
        int yPos = FontGetHeight(TTF_Antilles_XXL) + 50 + spacing * i;

        if (yPos > menu->gfx->window->height - 50 - 2 * spacing)
            continue;

        int xPos = menu->gfx->window->width / 20;

        FontDraw3D(menu->font, FontGetDynamicSizing(menu->font), playerList[i], xPos, yPos, FAL_L, 0, 1, F3D_TL, 10, menu->clr);
    }

    ///
    /// OPTIONS
    ///

    int optionLength = 1 + (int)lobby.isHost;
    char options[optionLength][100];

    //Rectify options
    menu->activeIndex = (menu->activeIndex > optionLength - 1) ? 0 : menu->activeIndex;
    menu->activeIndex = (menu->activeIndex < 0) ? optionLength - 1 : menu->activeIndex;

    strcpy(options[optionLength - 1 - (int)lobby.isHost], "Leave");
    if (lobby.isHost)
    {
        strcpy(options[optionLength - 1], "Start");
    }

    //Draw lobby options
    for (int i = 0; i < optionLength; i++)
    {
        int yPos = menu->gfx->window->height - 50 - 2 * spacing + spacing * i;

        int xPos = menu->gfx->window->width / 20;

        if (i == menu->activeIndex)
        {
            int rectWidth = FontGetWidth(menu->font, FontGetDynamicSizing(menu->font), options[i]);
            int rectHeight = FontGetHeight(FontGetDynamicSizing(menu->font));

            SDL_Rect rect = {xPos - 20, yPos - 15, rectWidth + 40, rectHeight + 30};
            SDL_Color color = {0, 0, 0, 200};
            GraphicsDrawRect(menu->gfx, rect, color, SDL_TRUE);

            FontDraw3DCustom(menu->font, FontGetDynamicSizing(menu->font), options[i], xPos, yPos, FAL_L, 0, cos(menu->loopCount) * 1.5, sin(menu->loopCount), 10, menu->clr);
        }
        else
        {
            FontDraw3D(menu->font, FontGetDynamicSizing(menu->font), options[i], xPos, yPos, FAL_L, 0, 1, F3D_TL, 10, menu->clr);
        }
    }

    if (InputIsKeyPressed(SDL_SCANCODE_E) || InputIsKeyPressed(SDL_SCANCODE_RETURN))
    {
        if (menu->activeIndex == optionLength - 1 - (int)lobby.isHost)
        {
            MenuStateSet(MS_MainMenu);
            ClientTCPSend(PT_LeaveSession, NULL, 0);
            VectorClear(lobby.names);
            lobby.isHost = SDL_FALSE;
        }
        if (lobby.isHost && menu->activeIndex == optionLength - 1)
        {
            // Start the session
            ClientTCPSend(PT_StartSession, NULL, 0);
        }
    }

    ///
    /// MAP PREVIEW
    ///
    int x = menu->gfx->window->width / 2 - 50;
    int y = menu->gfx->window->height / 2 - 50 - FontGetHeight(FontGetDynamicSizing(menu->font)) - 20;
    int w = menu->gfx->window->width / 3 + 100;
    int h = menu->gfx->window->height / 3 + 120 + FontGetHeight(FontGetDynamicSizing(menu->font));
    SDL_Rect mapBackgroundRect = {x, y, w, h};
    GraphicsDrawRect(menu->gfx, mapBackgroundRect, (SDL_Color){0, 0, 0, 200}, SDL_TRUE);

    MapInfo currentInfo = MapListGetMaps(menu->mapList)[menu->activeIndex];

    char maxPlayers[20];
    sprintf(maxPlayers, "Players: %d", currentInfo.maxPlayers);

    FontDraw3D(menu->font, FontGetDynamicSizing(menu->font), maxPlayers,
               x + (w / 2) - FontGetWidth(menu->font, FontGetDynamicSizing(menu->font), maxPlayers) / 2,
               y + 20,
               FAL_L, 0, 1, F3D_TL, 10, menu->clr);

    MapDraw(menu->camera);

    MenuTitleDraw(menu, "Lobby");
}

void MenuUpdateOptions(Menu *menu)
{
    //Determine menu options
    int optionLength = 7;
    char options[7][100] = {
        {"Toggle fullscreen"},
        {"Set resolution"},
        {"Toggle vSync"},
        {"SET FPS"},
        {"SET Keybinding"},
        {"Audio optiions"},
        {"Back"}};

    if (menu->gfx->window->isFullscreen)
    {
        sprintf(options[0], "Exit fullscreen");
    }
    else
    {
        sprintf(options[0], "Enter fullscreen");
    }

    if (menu->gfx->window->vsyncEnabled)
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
            if (menu->gfx->window->isFullscreen)
            {
                WindowSetFullscreen(menu->gfx->window, SDL_FALSE);
            }
            else
            {
                WindowSetFullscreen(menu->gfx->window, SDL_TRUE);
                SDL_DisplayMode displaymode;
                SDL_GetCurrentDisplayMode(0, &displaymode);
                menu->gfx->window->width = displaymode.w;
                menu->gfx->window->height = displaymode.h;
            }
            break;
        case 1: //set resolution
        {
            MenuStateSet(MS_Resolution);
        }
        break;
        case 2: //Vsync
        {
            if (menu->gfx->window->vsyncEnabled)
            {
                WindowSetVSync(menu->gfx->window, SDL_FALSE);
            }
            else
            {
                WindowSetVSync(menu->gfx->window, SDL_TRUE);
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

            if (clientManager.inGame)
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

    MenuDraw(menu, options, optionLength);
    MenuTitleDraw(menu, "Options");
}

void MenuUpdateResolution(Menu *menu)
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
            WindowSetSize(menu->gfx->window, width, height);
            break;
        case 1:
            width = 1280;
            height = 720;
            WindowSetSize(menu->gfx->window, width, height);
            break;
        case 2:
            width = 1920;
            height = 1080;
            WindowSetSize(menu->gfx->window, width, height);
            break;
        case 3:
            width = 1920;
            height = 1200;
            WindowSetSize(menu->gfx->window, width, height);
            break;
        case 4:
            width = 2560;
            height = 1440;
            WindowSetSize(menu->gfx->window, width, height);
            break;
        case 5:
        {

            MenuStateSet(MS_Options);
            break;
        }
        }
    }
    MenuDraw(menu, options, optionLength);
    MenuTitleDraw(menu, "Resolution");
}

void MenuUpdateFPS(Menu *menu, FPSManager *fpsManager)
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
            fpsManager->desiredFPS = 30;
            break;
        }
        case 1:
        {
            fpsManager->desiredFPS = 60;
            break;
        }
        break;
        case 2:
        {
            fpsManager->desiredFPS = 90;
            break;
        }
        case 3:
        {
            fpsManager->desiredFPS = 120;
            break;
        }
        case 4:
        {
            fpsManager->desiredFPS = 999; // they won't know
            break;
        }
        case 5:
        {

            MenuStateSet(MS_Options);
            break;
        }
        }
    }
    MenuDraw(menu, options, optionLength);
    MenuTitleDraw(menu, "FPS");
}

void MenuUpdateKeybinding(Menu *menu)
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
                KeybindingChange(MOVE_UP, InputLastKeyDown(menu->bindings->KeyArray[MOVE_UP]), menu->bindings);
                menu->keybindingstate = 0;
                break;
            }
            case 1:
            {
                KeybindingChange(MOVE_LEFT, InputLastKeyDown(menu->bindings->KeyArray[MOVE_LEFT]), menu->bindings);
                menu->keybindingstate = 0;
                break;
            }
            case 2:
            {
                KeybindingChange(MOVE_RIGHT, InputLastKeyDown(menu->bindings->KeyArray[MOVE_RIGHT]), menu->bindings);
                menu->keybindingstate = 0;
                break;
            }
            case 3:
            {
                KeybindingChange(MOVE_DOWN, InputLastKeyDown(menu->bindings->KeyArray[MOVE_DOWN]), menu->bindings);
                menu->keybindingstate = 0;
                break;
            }
            case 4:
            {
                KeybindingChange(PICKUP, InputLastKeyDown(menu->bindings->KeyArray[PICKUP]), menu->bindings);
                menu->keybindingstate = 0;
                break;
            }
            case 5:
            {
                KeybindingChange(DROP, InputLastKeyDown(menu->bindings->KeyArray[DROP]), menu->bindings);
                menu->keybindingstate = 0;
                break;
            }
            case 6:
            {
                KeybindingChange(SHOOT, InputLastKeyDown(menu->bindings->KeyArray[SHOOT]), menu->bindings);
                menu->keybindingstate = 0;
                break;
            }
            case 7:
            {
                KeybindingChange(RELOAD, InputLastKeyDown(menu->bindings->KeyArray[RELOAD]), menu->bindings);
                menu->keybindingstate = 0;
                break;
            }
            case 8:
            {
                KeybindingChange(INVENTORY, InputLastKeyDown(menu->bindings->KeyArray[INVENTORY]), menu->bindings);
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
    MenuDraw(menu, options, optionLength);
    MenuTitleDraw(menu, "Keybindings");
}

void MenuUpdateAudio(Menu *menu)
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

    sprintf(options[0], "Master: %d%%", (int)(menu->volumeMaster * 100));
    sprintf(options[1], "SFX: %u/64", menu->volumeSFX);
    sprintf(options[2], "Music: %u/64", menu->volumeMusic);

    menu->activeIndex = (menu->activeIndex > optionLength - 1) ? 0 : menu->activeIndex;
    menu->activeIndex = (menu->activeIndex < 0) ? optionLength - 1 : menu->activeIndex;

    if (InputIsKeyPressed(SDL_SCANCODE_A) || InputIsKeyPressed(SDL_SCANCODE_LEFT))
    {
        switch (menu->activeIndex)
        {
        case 0:
            menu->volumeMaster -= .1;
            break;
        case 1:
            menu->volumeSFX -= step;
            ;
            break;
        case 2:
            menu->volumeMusic -= step;
            break;
        }
    }

    if (InputIsKeyPressed(SDL_SCANCODE_D) || InputIsKeyPressed(SDL_SCANCODE_RIGHT))
    {
        switch (menu->activeIndex)
        {
        case 0:
            menu->volumeMaster += .1;
            break;
        case 1:
            menu->volumeSFX += step;
            break;
        case 2:
            menu->volumeMusic += step;
            break;
        }
    }

    // Loop back
    if (menu->volumeMaster < 0)
        menu->volumeMaster = 0;
    if (menu->volumeMaster > 2)
        menu->volumeMaster = 2;

    if (menu->volumeMusic > 128)
        menu->volumeMusic = 0;
    if (menu->volumeSFX > 128)
        menu->volumeSFX = 0;

    if (menu->volumeMusic > 64)
        menu->volumeMusic = 64;
    if (menu->volumeSFX > 64)
        menu->volumeSFX = 64;

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

    MenuDraw(menu, options, optionLength);
    MenuTitleDraw(menu, "Audio options");
}

void MenuUpdateCustomMap(Menu *menu, Player *player)
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
        for (int i = 0; i < mapInfo.enemySpawns->size; i++)
            NPCManagerAddNew(MapInfoGetEnemySpawns(&mapInfo)[i].position);
        PlayerGetEntity(player)->position = MapInfoGetPlayerSpawns(&mapInfo)[0].position;
    }

    MenuDraw(menu, options, optionLength);

    if (menu->activeIndex != optionLength - 1)
    {
        MenuDrawPreviewMap(menu);
    }

    if (menu->startedOutTransition && TransitionIsDone())
    {
        MenuStateSet(MS_None);
        GameStateSet(GS_Playing);
        MusicStop(&menu->MenuTheme);
        menu->startedOutTransition = SDL_FALSE;
        // To make in-game menu work properly
        clientManager.inGame = SDL_TRUE;
    }
    else if (menu->startedOutTransition && !TransitionIsDone())
    {
        TransitionDraw();
        CameraSetScale(menu->camera, TransitionGetSaveSlot(0));
        int x = (int)TransitionGetSaveSlot(1);
        int y = (int)TransitionGetSaveSlot(2);
        int w = (int)TransitionGetSaveSlot(3);
        int h = (int)TransitionGetSaveSlot(4);
        CameraSetViewPort(menu->camera, (SDL_Rect){x, y, w, h});
    }

    MenuTitleDraw(menu, "Local game");
}

void MenuUpdateSkin(Menu *menu, Player *player)
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
    int h = menu->font->gfx->window->height / 3 * 2;

    menu->previewLeg.active->dst.x = menu->font->gfx->window->width - h - 100;
    menu->previewLeg.active->dst.y = (menu->font->gfx->window->height - h) / 2;
    menu->previewBody.active->dst.x = menu->font->gfx->window->width - h - 100;
    menu->previewBody.active->dst.y = (menu->font->gfx->window->height - h) / 2;

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
            PlayerSetSpriteSheet(player, spriteSheet);
            MenuStateSet(MS_MainMenu);
            break;
        }
        case 1:
        {
            SpriteSheet spriteSheet = SS_Character_ChernobylWorker;
            ClientTCPSend(PT_ChangeSkin, (void *)&spriteSheet, sizeof(SpriteSheet));
            PlayerSetSpriteSheet(player, spriteSheet);

            MenuStateSet(MS_MainMenu);
            break;
        }
        case 2:
        {
            SpriteSheet spriteSheet = SS_Character_IronMan;
            ClientTCPSend(PT_ChangeSkin, (void *)&spriteSheet, sizeof(SpriteSheet));
            PlayerSetSpriteSheet(player, spriteSheet);

            MenuStateSet(MS_MainMenu);
            break;
        }
        case 3:
        {
            SpriteSheet spriteSheet = SS_Character_iDubbbz;
            ClientTCPSend(PT_ChangeSkin, (void *)&spriteSheet, sizeof(SpriteSheet));
            PlayerSetSpriteSheet(player, spriteSheet);

            MenuStateSet(MS_MainMenu);
            break;
        }
        case 4:
        {
            SpriteSheet spriteSheet = SS_Character_OldMan;
            ClientTCPSend(PT_ChangeSkin, (void *)&spriteSheet, sizeof(SpriteSheet));
            PlayerSetSpriteSheet(player, spriteSheet);

            MenuStateSet(MS_MainMenu);
            break;
        }
        case 5:
        {
            SpriteSheet spriteSheet = SS_Character_Sonic;
            ClientTCPSend(PT_ChangeSkin, (void *)&spriteSheet, sizeof(SpriteSheet));
            PlayerSetSpriteSheet(player, spriteSheet);

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
    MenuDraw(menu, options, optionLength);
    MenuTitleDraw(menu, "Choose skin");

    // If you are heading out, don't draw the character again as it will be a "dirty frame"
    if (MenuStateGet() == MS_MainMenu)
        return;

    if (menu->activeIndex == optionLength - 1)
        return;
    GraphicsDraw(menu->font->gfx, *menu->previewLeg.active);
    GraphicsDraw(menu->font->gfx, *menu->previewBody.active);
}

void MenuTitleDraw(Menu *menu, char title[100])
{
    FontDraw3DCustom(menu->font, TTF_Antilles_XXL, title, menu->gfx->window->width / 20, 25, FAL_L, 0, cos(menu->loopCount) * 3, sin(menu->loopCount) * 3, 10, menu->clr);
}

void MenuDrawPreviewMap(Menu *menu)
{
    int x = menu->gfx->window->width / 2 - 50;
    int y = menu->gfx->window->height / 2 - 50 - FontGetHeight(FontGetDynamicSizing(menu->font)) - 20;
    int w = menu->gfx->window->width / 3 + 100;
    int h = menu->gfx->window->height / 3 + 120 + FontGetHeight(FontGetDynamicSizing(menu->font));
    SDL_Rect mapBackgroundRect = {x, y, w, h};
    if (menu->startedOutTransition || menu->startedInTransition)
    {
        int x_t = (int)TransitionGetSaveSlot(1) - 50;
        int y_t = (int)TransitionGetSaveSlot(2) - 50 - FontGetHeight(FontGetDynamicSizing(menu->font)) - 20;
        int w_t = (int)TransitionGetSaveSlot(3) + 100;
        int h_t = (int)TransitionGetSaveSlot(4) + 120 + FontGetHeight(FontGetDynamicSizing(menu->font));
        GraphicsDrawRect(menu->gfx, (SDL_Rect){x_t, y_t, w_t, h_t}, (SDL_Color){0, 0, 0, (int)TransitionGetSaveSlot(5)}, SDL_TRUE);
    }
    else
    {
        GraphicsDrawRect(menu->gfx, mapBackgroundRect, (SDL_Color){0, 0, 0, 200}, SDL_TRUE);
    }

    MapInfo currentInfo = MapListGetMaps(menu->mapList)[menu->activeIndex];

    if (!menu->startedOutTransition && !menu->startedInTransition)
    {
        char maxPlayers[20];
        sprintf(maxPlayers, "Players: %d", currentInfo.maxPlayers);
        FontDraw3D(menu->font, FontGetDynamicSizing(menu->font), maxPlayers,
                   x + (w / 2) - FontGetWidth(menu->font, FontGetDynamicSizing(menu->font), maxPlayers) / 2,
                   y + 20,
                   FAL_L, 0, 1, F3D_TL, 10, menu->clr);
    }
    MapDraw(menu->camera);
}

void MenuDraw(Menu *menu, char options[][100], int optionLength)
{
    //Get windows size
    menu->mainMenuDbl.dst.w = menu->gfx->window->width;
    menu->mainMenuDbl.dst.h = menu->gfx->window->height;

    //Draw background
    GraphicsDraw(menu->gfx, menu->mainMenuDbl);

    //Get font spacing
    //*1.5 factor for space between lines
    int spacing = 1.5 * FontGetHeight(FontGetDynamicSizing(menu->font));

    //Draw menu options
    for (int i = 0; i < optionLength; i++)
    {
        //Calculate scrolling menu
        int screenCenterY = menu->gfx->window->height / 2;
        int yPos;

        if (spacing * optionLength > menu->font->gfx->window->height - FontGetHeight(TTF_Antilles_XXL) - 50)
        {
            yPos = screenCenterY - (spacing * menu->activeIndex) + spacing * i;
        }
        else
        {
            yPos = FontGetHeight(TTF_Antilles_XXL) + 50 + spacing * i;
        }

        if (yPos + FontGetHeight(FontGetDynamicSizing(menu->font)) < FontGetHeight(TTF_Antilles_XXL) + 50)
            continue;

        int xPos = menu->gfx->window->width / 20; //menu->gfx->window->width / 2

        if (i == menu->activeIndex)
        {
            int rectWidth = FontGetWidth(menu->font, FontGetDynamicSizing(menu->font), options[i]);
            int rectHeight = FontGetHeight(FontGetDynamicSizing(menu->font));

            SDL_Rect rect = {xPos - 20, yPos - 15, rectWidth + 40, rectHeight + 30};
            SDL_Color color = {0, 0, 0, 200};
            GraphicsDrawRect(menu->gfx, rect, color, SDL_TRUE);
            FontDraw3DCustom(menu->font, FontGetDynamicSizing(menu->font), options[i], xPos, yPos, FAL_L, 0, cos(menu->loopCount) * 1.5, sin(menu->loopCount), 10, menu->clr);
        }
        else
        {
            FontDraw3D(menu->font, FontGetDynamicSizing(menu->font), options[i], xPos, yPos, FAL_L, 0, 1, F3D_TL, 10, menu->clr);
        }
    }
}

void MenuDestroy(Menu *menu)
{
    LoadingBarDestroy(menu->loadingBar);
    SDL_free(menu);
}