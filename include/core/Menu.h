#ifndef MENU_H
#define MENU_H

#include "EntityManager.h"
#include "Font.h"
#include "Input.h"
#include "Clock.h"
#include "FPSManager.h"
#include "State.h"
#include "../MapList.h"
#include "../Map.h"

typedef struct Menu
{
    Graphics *gfx;
    EntityManager *entityManager;

    Font *font;
    Drawable mainMenuDbl;
    State *state;
    float loopCount;
    int loopSwing;
    int swingDir;
    int activeIndex;
    int lastIndex;
    SDL_bool indexChanged;

} Menu;

// Creates menu
Menu *MenuCreate(Graphics *gfx, EntityManager *entityManager, Font *font, State *state);
void MenuUpdate(Menu *menu, Input *input, FPSManager *fpsManager, MapList *mapList, Map *map);
void MenuUpdateMainMenu(Menu *menu, Input *input, Map *map);
void MenuUpdateOptions(Menu *menu, Input *input);
void MenuUpdateResolution(Menu *menu, Input *input);
void MenuUpdateFPS(Menu *menu, Input *input, FPSManager *fpsManager);
void MenuUpdateCustomMap(Menu *menu, Input *input, MapList *mapList, Map *map);

void MenuDraw(Menu *menu, char options[][100], int optionLength);

void MenuDestroy(Menu *menu);

#endif