#ifndef MENU_H
#define MENU_H

#include "Font.h"
#include "Input.h"
#include "Clock.h"
#include "State.h"
#include "../MapList.h"
#include "../Map.h"

typedef struct Menu
{
    Graphics *gfx;
    Font *font;
    Drawable mainMenuDbl;
    State *state;
    float loopCount;
    int loopSwing;
    int swingDir;
    int activeIndex;
    int Width;
    int Height;

} Menu;

// Creates menu
Menu *MenuCreate(Graphics *gfx, Font *font, State *state);
void MenuUpdate(Menu *menu, Input *input, FpsManger *FPSContorls, MapList *mapList, Map *map);
void MenuUpdateMainMenu(Menu *menu, Input *input, Map *map);
void MenuUpdateOptions(Menu *menu, Input *input);
void MenuUpdateResolution(Menu *menu, Input *input);
void MenuUpdateFPS(Menu *menu, Input *input, FpsManger *FPSContorls);
void MenuUpdateCustomMap(Menu *menu, Input *input, MapList *mapList, Map *map);

void MenuDraw(Menu *menu, char options[][100], int optionLength);

void MenuDestroy(Menu *menu);

#endif