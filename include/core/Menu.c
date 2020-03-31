#include "Menu.h"

#include "Log.h"

#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#define PI 3.14159265359

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
    menu->Scale = 1;

    SDL_Rect src = {0, 0, 400, 400};
    SDL_Rect dst = {0, 0, 600, 600};
    menu->mainMenuDbl = DrawableCreate(src, dst, SS_Menu);

    return menu;
}

void MenuUpdate(Menu *menu, Input *input)
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
    if (InputIsKeyPressed(input, SDL_SCANCODE_W) || InputIsKeyPressed(input, SDL_SCANCODE_UP))
    {
        if (menu->activeIndex > 0)
        {
            menu->activeIndex--;
        }
        else
        {
            menu->activeIndex = 3;
        }
    }
    if (InputIsKeyPressed(input, SDL_SCANCODE_S) || InputIsKeyPressed(input, SDL_SCANCODE_DOWN))
    {
        if (menu->activeIndex < 3)
        {
            menu->activeIndex++;
        }
        else
        {
            menu->activeIndex = 0;
        }
    }
    // if (menu->activeIndex > 0 || menu->activeIndex < 3)
    // {
    //     menu->activeIndex += (InputIsKeyPressed(input, SDL_SCANCODE_W) || InputIsKeyPressed(input, SDL_SCANCODE_UP)) - (InputIsKeyPressed(input, SDL_SCANCODE_S) || InputIsKeyPressed(input, SDL_SCANCODE_DOWN));
    // }
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
    int optionLength = 4;
    char options[4][100] = {
        {"Toggle fullscreen"},
        {"Set resolution"},
        {"Toggle vSync"},
        {"Back"}};
    //Get input
    if (InputIsKeyPressed(input, SDL_SCANCODE_W) || InputIsKeyPressed(input, SDL_SCANCODE_UP))
    {
        if (menu->activeIndex > 0)
        {
            menu->activeIndex--;
        }
        else
        {
            menu->activeIndex = 3;
        }
    }
    if (InputIsKeyPressed(input, SDL_SCANCODE_S) || InputIsKeyPressed(input, SDL_SCANCODE_DOWN))
    {
        if (menu->activeIndex < 3)
        {
            menu->activeIndex++;
        }
        else
        {
            menu->activeIndex = 0;
        }
    }
    if (InputIsKeyPressed(input, SDL_SCANCODE_E) || InputIsKeyPressed(input, SDL_SCANCODE_RETURN))
    {
        switch (menu->activeIndex)
        {
        case 0: //toggle fullscreen
            if (menu->gfx->isFullscreen)
            { //Get out of fullscreen
                SDL_SetWindowFullscreen(menu->gfx->m_mainWindow, NULL);
                menu->gfx->gfxWindowHeight -= 60;
                menu->gfx->isFullscreen = 0;
            }
            else
            {
                //go to fullscreen
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
        case 2: //Vsync // not supported with SDL alone // do we use OpenGL?
            break;
        case 3:
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
    int optionLength = 4;
    char options[4][100] = {
        {"Scale based"},
        {"UnUsed"},
        {"Apply"},
        {"Back"}};
    //Get input
    if (InputIsKeyPressed(input, SDL_SCANCODE_W) || InputIsKeyPressed(input, SDL_SCANCODE_UP))
    {
        if (menu->activeIndex > 0)
        {
            menu->activeIndex--;
        }
        else
        {
            menu->activeIndex = 3;
        }
    }
    if (InputIsKeyPressed(input, SDL_SCANCODE_S) || InputIsKeyPressed(input, SDL_SCANCODE_DOWN))
    {
        if (menu->activeIndex < 3)
        {
            menu->activeIndex++;
        }
        else
        {
            menu->activeIndex = 0;
        }
    }
    if (InputIsKeyPressed(input, SDL_SCANCODE_D) || InputIsKeyPressed(input, SDL_SCANCODE_RIGHT))
    {
        switch (menu->activeIndex)
        {
        case 0:
            if (menu->Scale < 3)
            {
                menu->Scale++;
                printf("Menu scale was incressed: %d\n", menu->Scale);
            }
            break;
        }
    }
    if (InputIsKeyPressed(input, SDL_SCANCODE_A) || InputIsKeyPressed(input, SDL_SCANCODE_LEFT))
    {
        switch (menu->activeIndex)
        {
        case 0:
            if (menu->Scale > 1)
            {
                menu->Scale--;
                printf("Menu scale was incressed: %d\n", menu->Scale);
            }
            break;
        }
    }
    if (InputIsKeyPressed(input, SDL_SCANCODE_E) || InputIsKeyPressed(input, SDL_SCANCODE_RETURN))
    {
        switch (menu->activeIndex)
        {
        case 0: // currently used by ad & Left Right
            break;
        case 1:
        {
        }
        break;
        case 2:
            SDL_SetWindowSize(menu->gfx->m_mainWindow, menu->Width * menu->Scale, menu->Height * menu->Scale);
            menu->gfx->gfxWindowWidth = menu->Width * menu->Scale;
            menu->gfx->gfxWindowHeight = menu->Height * menu->Scale;
            break;
        case 3:
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