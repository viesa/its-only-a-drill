#include "Menu.h"

#include "Log.h"
#include "Library.h"

Menu *MenuCreate(Graphics *gfx, Font *font)
{
    Menu *menu = (Menu *)SDL_malloc(sizeof(Menu));
    menu->gfx = gfx;
    menu->font = font;
    menu->currentState = MS_MainMenu;
    menu->loopCount = 0;
    menu->loopSwing = 87;
    menu->swingDir = 0;
    menu->activeIndex = 0;
    menu->Width = 640;
    menu->Height = 480;

    SDL_Rect src = {0, 0, 1919, 942};
    SDL_Rect dst = {0, 0, gfx->gfxWindowWidth, gfx->gfxWindowHeight};
    menu->mainMenuDbl = DrawableCreate(src, dst, SS_Menu);

    return menu;
}

void MenuUpdate(Menu *menu, Input *input, FpsManger *FPSContorls)
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

    if (menu->currentState != MS_None)
    {
        //Get windows size
        menu->mainMenuDbl.dst.w = menu->gfx->gfxWindowWidth;
        menu->mainMenuDbl.dst.h = menu->gfx->gfxWindowHeight;

        //Draw background
        GraphicsDraw(menu->gfx, menu->mainMenuDbl);
    }

    //Decides what shall be drawn on top
    switch (menu->currentState)
    {
    case MS_MainMenu:
        MenuUpdateMainMenu(menu, input);
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
    default:
        break;
    }
}

void MenuUpdateMainMenu(Menu *menu, Input *input)
{
    //Determine menu options
    int optionLength = 4;
    char options[4][100] = {
        {"Continue game"},
        {"Start new game"},
        {"Options"},
        {"Exit"}};

    //Get input
    menu->activeIndex += (InputIsKeyPressed(input, SDL_SCANCODE_S) || InputIsKeyPressed(input, SDL_SCANCODE_DOWN)) -
                         (InputIsKeyPressed(input, SDL_SCANCODE_W) || InputIsKeyPressed(input, SDL_SCANCODE_UP));
    menu->activeIndex = (menu->activeIndex > optionLength - 1) ? 0 : menu->activeIndex;
    menu->activeIndex = (menu->activeIndex < 0) ? optionLength - 1 : menu->activeIndex;

    if (InputIsKeyPressed(input, SDL_SCANCODE_E) || InputIsKeyPressed(input, SDL_SCANCODE_RETURN))
    {
        switch (menu->activeIndex)
        {
        case 0:
            menu->currentState = MS_None;
            break;
        case 2:
            menu->currentState = MS_Options;
            break;
        case 3:
        {
            SDL_Event quit;
            quit.type = SDL_QUIT;
            SDL_PushEvent(&quit);
        }
        break;
        }
    }

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
            FontDraw3DCustom(menu->font, TTF_Antilles, options[i], menu->gfx->gfxWindowWidth / 2, menu->gfx->gfxWindowHeight / 2 - (75 * optionLength / 2) + 75 * i, FAL_C, 0, cos(menu->loopCount) * 1.5, sin(menu->loopCount), 10, vitalsColor);
        }
        else
        {
            FontDraw3D(menu->font, TTF_Antilles, options[i], menu->gfx->gfxWindowWidth / 2, menu->gfx->gfxWindowHeight / 2 - (75 * optionLength / 2) + 75 * i, FAL_C, 0, 1, F3D_TL, 10, vitalsColor);
        }
    }
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
    //Get input
    menu->activeIndex += (InputIsKeyPressed(input, SDL_SCANCODE_S) || InputIsKeyPressed(input, SDL_SCANCODE_DOWN)) -
                         (InputIsKeyPressed(input, SDL_SCANCODE_W) || InputIsKeyPressed(input, SDL_SCANCODE_UP));
    menu->activeIndex = (menu->activeIndex > optionLength - 1) ? 0 : menu->activeIndex;
    menu->activeIndex = (menu->activeIndex < 0) ? optionLength - 1 : menu->activeIndex;

    if (InputIsKeyPressed(input, SDL_SCANCODE_E) || InputIsKeyPressed(input, SDL_SCANCODE_RETURN))
    {
        switch (menu->activeIndex)
        {
        case 0: //toggle fullscreen
            if (menu->gfx->isFullscreen)
            { //Get out of fullscreen
                SDL_SetWindowFullscreen(menu->gfx->m_mainWindow, 0);
                menu->gfx->gfxWindowHeight -= 60;
                menu->gfx->isFullscreen = 0;
            }
            else
            { //go to fullscreen
                SDL_SetWindowFullscreen(menu->gfx->m_mainWindow, SDL_WINDOW_FULLSCREEN);
                menu->gfx->gfxWindowHeight += 60;
                menu->gfx->isFullscreen = 1;
            }
            break;
        case 1: //set resolution
        {
            menu->currentState = MS_Resolution;
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
            menu->currentState = MS_FPS;
            break;
        }
        case 4:
        {
            menu->currentState = MS_MainMenu;
            break;
        }
        }
    }

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
            FontDraw3DCustom(menu->font, TTF_Antilles, options[i], menu->gfx->gfxWindowWidth / 2, menu->gfx->gfxWindowHeight / 2 - (75 * optionLength / 2) + 75 * i, FAL_C, 0, cos(menu->loopCount) * 1.5, sin(menu->loopCount), 10, vitalsColor);
        }
        else
        {
            FontDraw3D(menu->font, TTF_Antilles, options[i], menu->gfx->gfxWindowWidth / 2, menu->gfx->gfxWindowHeight / 2 - (75 * optionLength / 2) + 75 * i, FAL_C, 0, 1, F3D_TL, 10, vitalsColor);
        }
    }
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
    //Get input
    menu->activeIndex += (InputIsKeyPressed(input, SDL_SCANCODE_S) || InputIsKeyPressed(input, SDL_SCANCODE_DOWN)) -
                         (InputIsKeyPressed(input, SDL_SCANCODE_W) || InputIsKeyPressed(input, SDL_SCANCODE_UP));
    menu->activeIndex = (menu->activeIndex > optionLength - 1) ? 0 : menu->activeIndex;
    menu->activeIndex = (menu->activeIndex < 0) ? optionLength - 1 : menu->activeIndex;

    if (InputIsKeyPressed(input, SDL_SCANCODE_E) || InputIsKeyPressed(input, SDL_SCANCODE_RETURN))
    {
        switch (menu->activeIndex)
        {
        case 0:
            menu->Width = 640;
            menu->Height = 480;
            SDL_SetWindowSize(menu->gfx->m_mainWindow, menu->Width, menu->Height);
            menu->gfx->gfxWindowWidth = menu->Width;
            menu->gfx->gfxWindowHeight = menu->Height;
            break;
        case 1:
            menu->Width = 1280;
            menu->Height = 720;
            SDL_SetWindowSize(menu->gfx->m_mainWindow, menu->Width, menu->Height);
            menu->gfx->gfxWindowWidth = menu->Width;
            menu->gfx->gfxWindowHeight = menu->Height;
            break;
        case 2:
            menu->Width = 1920;
            menu->Height = 1080;
            SDL_SetWindowSize(menu->gfx->m_mainWindow, menu->Width, menu->Height);
            menu->gfx->gfxWindowWidth = menu->Width;
            menu->gfx->gfxWindowHeight = menu->Height;
            break;
        case 3:
            menu->Width = 1920;
            menu->Height = 1200;
            SDL_SetWindowSize(menu->gfx->m_mainWindow, menu->Width, menu->Height);
            menu->gfx->gfxWindowWidth = menu->Width;
            menu->gfx->gfxWindowHeight = menu->Height;
            break;
        case 4:
            menu->Width = 2560;
            menu->Height = 1440;
            SDL_SetWindowSize(menu->gfx->m_mainWindow, menu->Width, menu->Height);
            menu->gfx->gfxWindowWidth = menu->Width;
            menu->gfx->gfxWindowHeight = menu->Height;
            break;
        case 5:
            menu->Width = 2560;
            menu->Height = 1600;
            SDL_SetWindowSize(menu->gfx->m_mainWindow, menu->Width, menu->Height);
            menu->gfx->gfxWindowWidth = menu->Width;
            menu->gfx->gfxWindowHeight = menu->Height;
            break;
        case 6:
        {
            menu->currentState = MS_Options;
            break;
        }
        }
    }

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
            FontDraw3DCustom(menu->font, TTF_Antilles, options[i], menu->gfx->gfxWindowWidth / 2, menu->gfx->gfxWindowHeight / 2 - (75 * optionLength / 2) + 75 * i, FAL_C, 0, cos(menu->loopCount) * 1.5, sin(menu->loopCount), 10, vitalsColor);
        }
        else
        {
            FontDraw3D(menu->font, TTF_Antilles, options[i], menu->gfx->gfxWindowWidth / 2, menu->gfx->gfxWindowHeight / 2 - (75 * optionLength / 2) + 75 * i, FAL_C, 0, 1, F3D_TL, 10, vitalsColor);
        }
    }
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
    //Get input
    menu->activeIndex += (InputIsKeyPressed(input, SDL_SCANCODE_S) || InputIsKeyPressed(input, SDL_SCANCODE_DOWN)) -
                         (InputIsKeyPressed(input, SDL_SCANCODE_W) || InputIsKeyPressed(input, SDL_SCANCODE_UP));
    menu->activeIndex = (menu->activeIndex > optionLength - 1) ? 0 : menu->activeIndex;
    menu->activeIndex = (menu->activeIndex < 0) ? optionLength - 1 : menu->activeIndex;

    if (InputIsKeyPressed(input, SDL_SCANCODE_E) || InputIsKeyPressed(input, SDL_SCANCODE_RETURN))
    {
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
            menu->currentState = MS_Options;
            break;
        }
        }
    }

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
            FontDraw3DCustom(menu->font, TTF_Antilles, options[i], menu->gfx->gfxWindowWidth / 2, menu->gfx->gfxWindowHeight / 2 - (75 * optionLength / 2) + 75 * i, FAL_C, 0, cos(menu->loopCount) * 1.5, sin(menu->loopCount), 10, vitalsColor);
        }
        else
        {
            FontDraw3D(menu->font, TTF_Antilles, options[i], menu->gfx->gfxWindowWidth / 2, menu->gfx->gfxWindowHeight / 2 - (75 * optionLength / 2) + 75 * i, FAL_C, 0, 1, F3D_TL, 10, vitalsColor);
        }
    }
}
void MenuDestroy(Menu *menu)
{
    SDL_free(menu);
}