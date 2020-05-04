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
#include "MapList.h"
#include "Map.h"
#include "ClientManager.h"
#include "Lobby.h"

typedef struct Menu
{
    Graphics *gfx;

    Font *font;
    Drawable mainMenuDbl;
    Drawable lobbyHostDbl;
    Drawable lobbyNormalDbl;
    LoadingBar *loadingBar;
    Keybinding *bindings;
    float loopCount;
    int loopSwing;
    int swingDir;
    int activeIndex;
    int lastIndex;
    int keybindingstate;
    SDL_bool indexChanged;
    float fetchSessionsTimer;
    float fetchLobbyTimer;
} Menu;

// Creates menu
Menu *MenuCreate(Graphics *gfx, Font *font, Keybinding *bindings);
void MenuUpdate(Menu *menu, FPSManager *fpsManager, MapList *mapList);
void MenuUpdateSplash(Menu *menu);
void MenuUpdateName(Menu *menu);
void MenuUpdateMainMenu(Menu *menu);
void MenuUpdateJoinLobby(Menu *menu);
void MenuUpdateHostLobby(Menu *menu, MapList *mapList);
void MenuUpdateWaitingForLobby(Menu *menu);
void MenuUpdateLobby(Menu *menu);
void MenuUpdateOptions(Menu *menu);
void MenuUpdateResolution(Menu *menu);
void MenuUpdateFPS(Menu *menu, FPSManager *fpsManager);
void MenuUpdateKeybinding(Menu *menu);
void MenuUpdateCustomMap(Menu *menu, MapList *mapList);
void MenuDraw(Menu *menu, char options[][100], int optionLength);

void MenuDestroy(Menu *menu);

#endif