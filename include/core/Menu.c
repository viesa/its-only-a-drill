#include "Menu.h"

#include "Log.h"
#include "Library.h"

Menu *MenuCreate(Graphics *gfx, EntityManager *entityManager, Font *font, State *state)
{
    Menu *menu = (Menu *)SDL_malloc(sizeof(Menu));
    menu->gfx = gfx;
    menu->entityManager = entityManager;
    menu->font = font;
    menu->state = state;
    menu->loopCount = 0;
    menu->loopSwing = 87;
    menu->swingDir = 0;
    menu->activeIndex = 0;
    menu->lastIndex = 0;
    menu->Width = 640;
    menu->Height = 480;

    SDL_Rect src = {0, 0, 1919, 942};
    SDL_Rect dst = {0, 0, gfx->windowWidth, gfx->windowHeight};
    menu->mainMenuDbl = DrawableCreate(src, dst, SS_Menu);

    return menu;
}

void MenuUpdate(Menu *menu, Input *input, FpsManger *FPSContorls, MapList *mapList, Map *map)
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
    menu->activeIndex += (InputIsKeyPressed(input, SDL_SCANCODE_S) || InputIsKeyPressed(input, SDL_SCANCODE_DOWN)) -
                         (InputIsKeyPressed(input, SDL_SCANCODE_W) || InputIsKeyPressed(input, SDL_SCANCODE_UP));

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
    case MS_MainMenu:
        MenuUpdateMainMenu(menu, input, map);
        break;
    case MS_Options:
        MenuUpdateOptions(menu, input);
        break;
    case MS_Resolution:
        MenuUpdateResolution(menu, input);
        break;
    case MS_FPS:
        MenuUpdateFPS(menu, input, FPSContorls);
        break;
    case MS_CustomMap:
        MenuUpdateCustomMap(menu, input, mapList, map);
        break;
    default:
        break;
    }
}

void MenuUpdateMainMenu(Menu *menu, Input *input, Map *map)
{
    //Determine menu options
    int optionLength = 5;
    char options[5][100] = {
        {"Join party"},
        {"Host party"},
        {"Local game"},
        {"Options"},
        {"Exit"}};
    // makes it loop
    menu->activeIndex = (menu->activeIndex > optionLength - 1) ? 0 : menu->activeIndex;
    menu->activeIndex = (menu->activeIndex < 0) ? optionLength - 1 : menu->activeIndex;

    if (InputIsKeyPressed(input, SDL_SCANCODE_E) || InputIsKeyPressed(input, SDL_SCANCODE_RETURN))
    {
        menu->indexChanged = SDL_TRUE;
        switch (menu->activeIndex)
        {
        case 0:
        {
            //Join party
        }
        case 1:
        {
            //Host party
        }
        case 2:
        {
            menu->state->menuState = MS_CustomMap;
            menu->activeIndex = 0;
            break;
        }
        case 3:
        {
            menu->state->menuState = MS_Options;
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

void MenuUpdateOptions(Menu *menu, Input *input)
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

    if (InputIsKeyPressed(input, SDL_SCANCODE_E) || InputIsKeyPressed(input, SDL_SCANCODE_RETURN))
    {
        menu->indexChanged = SDL_TRUE;
        switch (menu->activeIndex)
        {
        case 0: //toggle fullscreen
            if (menu->gfx->isFullscreen)
            { //Get out of fullscreen
                SDL_SetWindowFullscreen(menu->gfx->m_mainWindow, 0);
                menu->gfx->windowHeight -= 60;
                menu->gfx->isFullscreen = 0;
            }
            else
            { //go to fullscreen
                SDL_SetWindowFullscreen(menu->gfx->m_mainWindow, SDL_WINDOW_FULLSCREEN);
                menu->gfx->windowHeight += 60;
                menu->gfx->isFullscreen = 1;
            }
            break;
        case 1: //set resolution
        {
            menu->state->menuState = MS_Resolution;
        }
        break;
        case 2: //Vsync // do we use OpenGL?
        {
            if (menu->gfx->vsync)
            {
                SDL_GL_SetSwapInterval(0);
                menu->gfx->vsync = SDL_FALSE;
            }
            else
            {
                SDL_GL_SetSwapInterval(1);
                menu->gfx->vsync = SDL_TRUE;
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
void MenuUpdateResolution(Menu *menu, Input *input)
{
    //Determine menu options
    int optionLength = 7;
    char options[7][100] = {
        {"640x480"},
        {"1280x720"},
        {"1920x1080"},
        {"1920x1200"},
        {"2560x1440"},
        {"2560x1600"},
        {"Back"}};

    menu->activeIndex = (menu->activeIndex > optionLength - 1) ? 0 : menu->activeIndex;
    menu->activeIndex = (menu->activeIndex < 0) ? optionLength - 1 : menu->activeIndex;

    if (InputIsKeyPressed(input, SDL_SCANCODE_E) || InputIsKeyPressed(input, SDL_SCANCODE_RETURN))
    {
        menu->indexChanged = SDL_TRUE;
        switch (menu->activeIndex)
        {
        case 0:
            menu->Width = 640;
            menu->Height = 480;
            SDL_SetWindowSize(menu->gfx->m_mainWindow, menu->Width, menu->Height);
            menu->gfx->windowWidth = menu->Width;
            menu->gfx->windowHeight = menu->Height;
            break;
        case 1:
            menu->Width = 1280;
            menu->Height = 720;
            SDL_SetWindowSize(menu->gfx->m_mainWindow, menu->Width, menu->Height);
            menu->gfx->windowWidth = menu->Width;
            menu->gfx->windowHeight = menu->Height;
            break;
        case 2:
            menu->Width = 1920;
            menu->Height = 1080;
            SDL_SetWindowSize(menu->gfx->m_mainWindow, menu->Width, menu->Height);
            menu->gfx->windowWidth = menu->Width;
            menu->gfx->windowHeight = menu->Height;
            break;
        case 3:
            menu->Width = 1920;
            menu->Height = 1200;
            SDL_SetWindowSize(menu->gfx->m_mainWindow, menu->Width, menu->Height);
            menu->gfx->windowWidth = menu->Width;
            menu->gfx->windowHeight = menu->Height;
            break;
        case 4:
            menu->Width = 2560;
            menu->Height = 1440;
            SDL_SetWindowSize(menu->gfx->m_mainWindow, menu->Width, menu->Height);
            menu->gfx->windowWidth = menu->Width;
            menu->gfx->windowHeight = menu->Height;
            break;
        case 5:
            menu->Width = 2560;
            menu->Height = 1600;
            SDL_SetWindowSize(menu->gfx->m_mainWindow, menu->Width, menu->Height);
            menu->gfx->windowWidth = menu->Width;
            menu->gfx->windowHeight = menu->Height;
            break;
        case 6:
        {
            menu->activeIndex = 0;
            menu->state->menuState = MS_Options;
            break;
        }
        }
    }
    MenuDraw(menu, options, optionLength);
}
void MenuUpdateFPS(Menu *menu, Input *input, FpsManger *FPSContorls)
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

    if (InputIsKeyPressed(input, SDL_SCANCODE_E) || InputIsKeyPressed(input, SDL_SCANCODE_RETURN))
    {
        menu->indexChanged = SDL_TRUE;
        switch (menu->activeIndex)
        {
        case 0:
        {
            FPSContorls->DesiredFPS = 30;
            break;
        }
        case 1:
        {
            FPSContorls->DesiredFPS = 60;
            break;
        }
        break;
        case 2:
        {
            FPSContorls->DesiredFPS = 90;
            break;
        }
        case 3:
        {
            FPSContorls->DesiredFPS = 120;
            break;
        }
        case 4:
        {
            FPSContorls->DesiredFPS = 999; // they won't know
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

void MenuUpdateCustomMap(Menu *menu, Input *input, MapList *mapList, Map *map)
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

    if (InputIsKeyPressed(input, SDL_SCANCODE_E) || InputIsKeyPressed(input, SDL_SCANCODE_RETURN))
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
        *map = MapCreate(mapdata, menu->entityManager);
        JSONDestroy(mapdata);
    }

    MenuDraw(menu, options, optionLength);
}

void MenuDraw(Menu *menu, char options[][100], int optionLength)
{
    //Get windows size
    menu->mainMenuDbl.dst.w = menu->gfx->windowWidth;
    menu->mainMenuDbl.dst.h = menu->gfx->windowHeight;

    //Draw background
    if (menu->state->menuState == MS_MainMenu)
    {
        GraphicsDraw(menu->gfx, menu->mainMenuDbl);
    }

    switch (menu->state->menuState)
    {
    case MS_Options:
    case MS_MainMenu:
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

        //Draw menu options
        for (size_t i = 0; i < optionLength; i++)
        {
            if (i == menu->activeIndex)
            {
                FontDraw3DCustom(menu->font, TTF_Antilles, options[i], menu->gfx->windowWidth / 2, menu->gfx->windowHeight / 2 - (75 * optionLength / 2) + 75 * i, FAL_C, 0, cos(menu->loopCount) * 1.5, sin(menu->loopCount), 10, vitalsColor);
            }
            else
            {
                FontDraw3D(menu->font, TTF_Antilles, options[i], menu->gfx->windowWidth / 2, menu->gfx->windowHeight / 2 - (75 * optionLength / 2) + 75 * i, FAL_C, 0, 1, F3D_TL, 10, vitalsColor);
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
    SDL_free(menu);
}