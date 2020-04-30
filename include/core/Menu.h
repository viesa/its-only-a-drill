#ifndef MENU_H
#define MENU_H

#include "Dependencies.h"
#include "EntityManager.h"
#include "Font.h"
#include "Input.h"
#include "Clock.h"
#include "FPSManager.h"
#include "Transition.h"
#include "Loadingbar.h"
#include "State.h"
#include "../MapList.h"
#include "../Map.h"
#include "../net/Client.h"

typedef struct Menu
{
    Graphics *gfx;

    Font *font;
    Drawable mainMenuDbl;
    Drawable lobbyDbl;
    LoadingBar *loadingBar;
    State *state;
    float loopCount;
    int loopSwing;
    int swingDir;
    int activeIndex;
    int lastIndex;
    SDL_bool indexChanged;

} Menu;

// Creates menu
Menu *MenuCreate(Graphics *gfx, Font *font, State *state);
void MenuUpdate(Menu *menu, FPSManager *fpsManager, MapList *mapList, Map *map);
void MenuUpdateSplash(Menu *menu, Map *map);
void MenuUpdateName(Menu *menu);
void MenuUpdateMainMenu(Menu *menu, Map *map);
void MenuUpdateJoinLobby(Menu *menu);
void MenuUpdateHostLobby(Menu *menu, MapList *mapList, Map *map);
void MenuUpdateWaitingForLobby(Menu *menu);
void MenuUpdateOptions(Menu *menu);
void MenuUpdateResolution(Menu *menu);
void MenuUpdateFPS(Menu *menu, FPSManager *fpsManager);
void MenuUpdateCustomMap(Menu *menu, MapList *mapList, Map *map);
void MenuDraw(Menu *menu, char options[][100], int optionLength);

void MenuDestroy(Menu *menu);

#endif