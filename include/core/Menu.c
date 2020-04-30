#include "Menu.h"

#include "Library.h"

Menu *MenuCreate(Graphics *gfx, Font *font, State *state)
{
    Menu *menu = MALLOC(Menu);
    menu->gfx = gfx;
    menu->font = font;
    menu->loadingBar = LoadingBarCreate(menu->gfx);
    menu->state = state;
    menu->loopCount = 0;
    menu->loopSwing = 87;
    menu->swingDir = 0;
    menu->activeIndex = 0;
    menu->lastIndex = 0;

    SDL_Rect src = {0, 0, 1919, 942};
    SDL_Rect dst = {0, 0, gfx->window->width, gfx->window->height};
    menu->mainMenuDbl = DrawableCreate(src, dst, SS_Menu);
    menu->lobbyDbl = DrawableCreate((SDL_Rect){0, 0, 1920, 1080}, dst, SS_Lobby);
    TransitionInitalize(menu->gfx, menu->font);
    TransitionStart(TT_FadeOut, 2);
    LoadingBarShow(menu->loadingBar);
    return menu;
}

void MenuUpdate(Menu *menu, FPSManager *fpsManager, MapList *mapList, Map *map)
{
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

    //Get input
    menu->activeIndex += (InputIsKeyPressed(SDL_SCANCODE_S) || InputIsKeyPressed(SDL_SCANCODE_DOWN)) -
                         (InputIsKeyPressed(SDL_SCANCODE_W) || InputIsKeyPressed(SDL_SCANCODE_UP));

    if (menu->lastIndex != menu->activeIndex)
    {
        menu->indexChanged = SDL_TRUE;
    }
    else
    {
        menu->indexChanged = SDL_FALSE;
    }
    menu->lastIndex = menu->activeIndex;

    //Decides what shall be drawn on top
    switch (menu->state->menuState)
    {
    case MS_Splash:
        MenuUpdateSplash(menu, map);
        break;
    case MS_Name:
        MenuUpdateName(menu);
        break;
    case MS_MainMenu:
        MenuUpdateMainMenu(menu, map);
        break;
    case MS_JoinLobby:
        MenuUpdateJoinLobby(menu);
        break;
    case MS_HostLobby:
        MenuUpdateHostLobby(menu, mapList, map);
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
    case MS_CustomMap:
        MenuUpdateCustomMap(menu, mapList, map);
        break;
    default:
        break;
    }
}

void MenuUpdateName(Menu *menu)
{
    SDL_Color vitalsColor[10] = {
        {menu->loopSwing, 159, 227},
        {menu->loopSwing, 139, 207},
        {menu->loopSwing, 119, 187},
        {menu->loopSwing, 99, 167},
        {menu->loopSwing, 79, 147},
        {menu->loopSwing, 59, 127},
        {menu->loopSwing, 39, 107},
        {menu->loopSwing, 19, 87},
        {255 - menu->loopSwing, 180, 184},
        {255 - menu->loopSwing, 180, 184}};

    //SDL_StartTextInput(); - starts input? maybe needed, seems to work without idk
    if (InputIsKeyPressed(SDL_SCANCODE_BACKSPACE))
        InputPortalBackspace();

    FontDraw3DCustom(menu->font, TTF_Antilles_XXL, InputGetPortalContent(), menu->gfx->window->width / 2, menu->gfx->window->height / 4, FAL_C, 0, cos(menu->loopCount) * 1.5, sin(menu->loopCount), 10, vitalsColor);
    FontDraw(menu->font, FontGetDynamicSizing(menu->font), "Type a name, press [Enter] when done.", menu->gfx->window->width / 2, menu->gfx->window->height / 4 * 3, FAL_C, 0, vitalsColor[9]);

    if (InputIsKeyPressed(SDL_SCANCODE_RETURN))
    {
        strcpy(client.name, InputGetPortalContent());
        menu->state->menuState = MS_MainMenu;
    }
}

void MenuUpdateSplash(Menu *menu, Map *map)
{
    if (!menu->loadingBar->active)
    {
        if (InputIsAnyKeyDown())
        {
            menu->state->menuState = MS_Name;
        }

        GraphicsDraw(menu->gfx, menu->mainMenuDbl);

        SDL_Color vitalsColor[10] = {
            {menu->loopSwing, 159, 227},
            {menu->loopSwing, 139, 207},
            {menu->loopSwing, 119, 187},
            {menu->loopSwing, 99, 167},
            {menu->loopSwing, 79, 147},
            {menu->loopSwing, 59, 127},
            {menu->loopSwing, 39, 107},
            {menu->loopSwing, 19, 87},
            {255 - menu->loopSwing, 180, 184},
            {255 - menu->loopSwing, 180, 184}};

        FontDraw3DCustom(menu->font, TTF_Antilles_XXL, "It's Only a Drill", menu->gfx->window->width / 2, menu->gfx->window->height / 4, FAL_C, 0, cos(menu->loopCount) * 1.5, sin(menu->loopCount), 10, vitalsColor);
        FontDraw(menu->font, FontGetDynamicSizing(menu->font), "Press the any key", menu->gfx->window->width / 2, menu->gfx->window->height / 4 * 3, FAL_C, 0, vitalsColor[9]);
    }
    //Fade-in animation
    TransitionDraw();
    LoadingBarUpdate(menu->loadingBar);
    LoadingBarAdd(menu->loadingBar, ClockGetFPS() * 4 / 100);
}

void MenuUpdateMainMenu(Menu *menu, Map *map)
{
    //Determine menu options
    int optionLength = 5;
    char options[5][100] = {
        {"Join lobby"},
        {"Host lobby"},
        {"Local game"},
        {"Options"},
        {"Exit"}};
    // makes it loop
    menu->activeIndex = (menu->activeIndex > optionLength - 1) ? 0 : menu->activeIndex;
    menu->activeIndex = (menu->activeIndex < 0) ? optionLength - 1 : menu->activeIndex;

    if (InputIsKeyPressed(SDL_SCANCODE_E) || InputIsKeyPressed(SDL_SCANCODE_RETURN))
    {
        menu->indexChanged = SDL_TRUE;
        switch (menu->activeIndex)
        {
        case 0:
        {
            menu->state->menuState = MS_JoinLobby;
            menu->activeIndex = 0;
            menu->indexChanged = SDL_TRUE;
            break;
        }
        case 1:
        {
            menu->state->menuState = MS_HostLobby;
            menu->activeIndex = 0;
            menu->indexChanged = SDL_TRUE;
            break;
        }
        case 2:
        {
            menu->state->menuState = MS_CustomMap;
            menu->activeIndex = 0;
            menu->indexChanged = SDL_TRUE;
            break;
        }
        case 3:
        {
            menu->state->menuState = MS_Options;
            menu->activeIndex = 0;
            menu->indexChanged = SDL_TRUE;
            break;
        }
        case 4:
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
}

void MenuUpdateHostLobby(Menu *menu, MapList *mapList, Map *map)
{
    //Determine menu options
    int optionLength = mapList->nMaps + 1;
    char options[optionLength][100];
    for (int i = 0; i < optionLength; i++)
        for (int j = 0; j < 100; j++)
            options[i][j] = 0;

    for (int i = 0, j = 0; j < mapList->nMaps; i++, j++)
    {
        MapListEntry *entry = &mapList->allMaps[j];
        sprintf(options[i], "%s [Players: %d]", entry->name, (int)entry->maxPlayers);
    }
    strcpy(options[optionLength - 1], "Back");

    menu->activeIndex = (menu->activeIndex > optionLength - 1) ? 0 : menu->activeIndex;
    menu->activeIndex = (menu->activeIndex < 0) ? optionLength - 1 : menu->activeIndex;

    if (InputIsKeyPressed(SDL_SCANCODE_E) || InputIsKeyPressed(SDL_SCANCODE_RETURN))
    {
        if (menu->activeIndex == optionLength - 1)
        {
            menu->indexChanged = SDL_TRUE;
            menu->activeIndex = 0;
            menu->state->menuState = MS_MainMenu;
        }
        else
        {
            MapListEntry *entry = &mapList->allMaps[menu->activeIndex];
            LoadedFile lfile = LoadedFileCreate(entry->filename);

            ClientTCPSend(PT_CreateSession, lfile.contents, lfile.size);
            menu->indexChanged = SDL_TRUE;
            menu->activeIndex = 0;
            menu->state->menuState = MS_WaitingForLobby;
        }
    }
    else if (menu->indexChanged && menu->activeIndex != optionLength - 1)
    {
        if (map->contents != NULL)
        {
            MapDestroy(map);
        }
        MapListEntry *entry = &mapList->allMaps[menu->activeIndex];
        JSON *mapdata = JSONCreate(entry->filename);
        *map = MapCreate(mapdata);
        JSONDestroy(mapdata);
    }

    MenuDraw(menu, options, optionLength);
}

void MenuUpdateJoinLobby(Menu *menu)
{
    //Determine menu options
    int optionLength = 1;
    char options[1][100] = {
        {"Back"}};

    menu->activeIndex = (menu->activeIndex > optionLength - 1) ? 0 : menu->activeIndex;
    menu->activeIndex = (menu->activeIndex < 0) ? optionLength - 1 : menu->activeIndex;

    if (InputIsKeyPressed(SDL_SCANCODE_E) || InputIsKeyPressed(SDL_SCANCODE_RETURN))
    {
        menu->indexChanged = SDL_TRUE;
        switch (menu->activeIndex)
        {
        case 0:
        {
            menu->activeIndex = 0;
            menu->state->menuState = MS_MainMenu;
            break;
        }
        }
    }
    MenuDraw(menu, options, optionLength);
}

void MenuUpdateWaitingForLobby(Menu *menu)
{
    //Determine menu options
    int optionLength = 1;
    char options[1][100] = {
        {"Waiting for lobby"}};

    // State is changed by ClientManager upon receiving confirmation from server
    MenuDraw(menu, options, optionLength);
}

void MenuUpdateOptions(Menu *menu)
{
    //Determine menu options
    int optionLength = 5;
    char options[5][100] = {
        {"Toggle fullscreen"},
        {"Set resolution"},
        {"Toggle vSync"},
        {"SET FPS"},
        {"Back"}};

    menu->activeIndex = (menu->activeIndex > optionLength - 1) ? 0 : menu->activeIndex;
    menu->activeIndex = (menu->activeIndex < 0) ? optionLength - 1 : menu->activeIndex;

    if (InputIsKeyPressed(SDL_SCANCODE_E) || InputIsKeyPressed(SDL_SCANCODE_RETURN))
    {
        menu->indexChanged = SDL_TRUE;
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
            }
            break;
        case 1: //set resolution
        {
            menu->state->menuState = MS_Resolution;
        }
        break;
        case 2: //Vsync // do we use OpenGL?
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
            menu->state->menuState = MS_FPS;
            break;
        }
        case 4:
        {
            menu->activeIndex = 0;
            menu->state->menuState = MS_MainMenu;
            break;
        }
        }
    }
    MenuDraw(menu, options, optionLength);
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
        menu->indexChanged = SDL_TRUE;
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
            menu->activeIndex = 0;
            menu->state->menuState = MS_Options;
            break;
        }
        }
    }
    MenuDraw(menu, options, optionLength);
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
        menu->indexChanged = SDL_TRUE;
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
            menu->activeIndex = 0;
            menu->state->menuState = MS_Options;
            break;
        }
        }
    }
    MenuDraw(menu, options, optionLength);
}

void MenuUpdateCustomMap(Menu *menu, MapList *mapList, Map *map)
{
    //Determine menu options
    int optionLength = mapList->nMaps + 1;
    char options[optionLength][100];
    for (int i = 0; i < optionLength; i++)
        for (int j = 0; j < 100; j++)
            options[i][j] = 0;

    for (int i = 0, j = 0; j < mapList->nMaps; i++, j++)
    {
        MapListEntry *entry = &mapList->allMaps[j];
        char buffer[100] = {0};
        sprintf(buffer, "%s [Diff: %d, MaxPl: %d]", entry->name, entry->difficulty, entry->maxPlayers);
        strcpy(options[i], buffer);
    }
    strcpy(options[optionLength - 1], "Back");

    menu->activeIndex = (menu->activeIndex > optionLength - 1) ? 0 : menu->activeIndex;
    menu->activeIndex = (menu->activeIndex < 0) ? optionLength - 1 : menu->activeIndex;

    if (InputIsKeyPressed(SDL_SCANCODE_E) || InputIsKeyPressed(SDL_SCANCODE_RETURN))
    {
        if (menu->activeIndex == optionLength - 1)
        {

            menu->indexChanged = SDL_TRUE;
            menu->activeIndex = 0;
            menu->state->menuState = MS_MainMenu;
        }
        else
        {
            menu->state->menuState = MS_None;
            menu->state->gameState = GS_Playing;
        }
    }
    else if (menu->indexChanged && menu->activeIndex != optionLength - 1)
    {
        if (map->contents != NULL)
        {
            MapDestroy(map);
        }
        MapListEntry *entry = &mapList->allMaps[menu->activeIndex];
        JSON *mapdata = JSONCreate(entry->filename);
        *map = MapCreate(mapdata);
        JSONDestroy(mapdata);
    }

    MenuDraw(menu, options, optionLength);
}

void MenuDraw(Menu *menu, char options[][100], int optionLength)
{
    //Get windows size
    menu->mainMenuDbl.dst.w = menu->gfx->window->width;
    menu->mainMenuDbl.dst.h = menu->gfx->window->height;

    //Draw background
    if (menu->state->menuState != MS_CustomMap &&
        menu->state->menuState != MS_Splash &&
        menu->state->menuState != MS_HostLobby &&
        menu->state->menuState != MS_JoinLobby)
    {
        GraphicsDraw(menu->gfx, menu->mainMenuDbl);
    }

    switch (menu->state->menuState)
    {
    case MS_Options:
    case MS_MainMenu:
    case MS_JoinLobby:
    case MS_HostLobby:
    case MS_Resolution:
    case MS_FPS:
    case MS_CustomMap:
    {
        //Update 3d color
        SDL_Color vitalsColor[10] = {
            {menu->loopSwing, 159, 227},
            {menu->loopSwing, 139, 207},
            {menu->loopSwing, 119, 187},
            {menu->loopSwing, 99, 167},
            {menu->loopSwing, 79, 147},
            {menu->loopSwing, 59, 127},
            {menu->loopSwing, 39, 107},
            {menu->loopSwing, 19, 87},
            {255 - menu->loopSwing, 180, 184},
            {255 - menu->loopSwing, 180, 184}};

        //Get font spacing
        //*1.5 factor for space between lines
        int spacing = 1.5 * FontGetHeight(FontGetDynamicSizing(menu->font));

        //Draw menu options
        for (size_t i = 0; i < optionLength; i++)
        {
            //Calculate scrolling menu
            int screenCenterY = menu->gfx->window->height / 2;
            int yPos = screenCenterY - (spacing * menu->activeIndex) + spacing * i;

            if (i == menu->activeIndex)
            {
                FontDraw3DCustom(menu->font, FontGetDynamicSizing(menu->font), options[i], menu->gfx->window->width / 2, yPos, FAL_C, 0, cos(menu->loopCount) * 1.5, sin(menu->loopCount), 10, vitalsColor);
            }
            else
            {
                FontDraw3D(menu->font, FontGetDynamicSizing(menu->font), options[i], menu->gfx->window->width / 2, yPos, FAL_C, 0, 1, F3D_TL, 10, vitalsColor);
            }
        }
        break;
    }
    default:
        break;
    }
}

void MenuDestroy(Menu *menu)
{
    LoadingBarDestroy(menu->loadingBar);
    SDL_free(menu);
}