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
    Clock *clock;
    float loopCount;
    int loopSwing;
    int swingDir;
    int activeIndex;
    int lastIndex;
    SDL_bool indexChanged;

} Menu;

// Creates menu
Menu *MenuCreate(Graphics *gfx, Font *font, State *state, Clock *clock);
void MenuUpdate(Menu *menu, Input *input, FPSManager *fpsManager, MapList *mapList, Map *map);
void MenuUpdateSplash(Menu *menu, Input *input, Map *map);
void MenuUpdateMainMenu(Menu *menu, Input *input, Map *map);
void MenuUpdateJoinLobby(Menu *menu, Input *input);
void MenuUpdateHostLobby(Menu *menu, Input *input, MapList *mapList, Map *map);
void MenuUpdateWaitingForLobby(Menu *menu, Input *input);
void MenuUpdateOptions(Menu *menu, Input *input);
void MenuUpdateResolution(Menu *menu, Input *input);
void MenuUpdateFPS(Menu *menu, Input *input, FPSManager *fpsManager);
void MenuUpdateCustomMap(Menu *menu, Input *input, MapList *mapList, Map *map);
void MenuDraw(Menu *menu, char options[][100], int optionLength);

void MenuDestroy(Menu *menu);

#endif