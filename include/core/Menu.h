#ifndef MENU_H
#define MENU_H

#include "Font.h"
#include "Drawable.h"
#include "Graphics.h"
#include "Input.h"

typedef enum MenuState
{
    MS_Options,
    MS_MainMenu,
    MS_Resolution,
    MS_None
} MenuState;

typedef struct Menu
{
    Graphics *gfx;
    Font *font;
    Drawable mainMenuDbl;
    MenuState currentState;
    float loopCount;
    int loopSwing;
    int swingDir;
    int activeIndex;
    int Width;
    int Height;
    int Scale;

} Menu;

Menu *MenuCreate(Graphics *gfx, Font *font);
void MenuUpdate(Menu *menu, Input *input);
void MenuUpdateMainMenu(Menu *menu, Input *input);
void MenuUpdateOptions(Menu *menu, Input *input);
void MenuUpdateResolution(Menu *menu, Input *input);
void MenuDestroy(Menu *menu);

#endif