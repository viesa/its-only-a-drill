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
#include "Music.h"
#include "Audio.h"
#include "Sound.h"
#include "Anim.h"
#include "Transition.h"
#include "Settings.h"
#include "NPCManager.h"

#define FETCH_SESSIONS_INTERVAL (2.0f)
#define FETCH_LOBBY_INTERVAL (0.5f)

// Creates menu
void MenuInitialize(Keybinding *bindings, MapList *mapList);
void MenuUninitialize();

void MenuResetActiveIndex();
void MenuResetFetchSessionsTimer();
void MenuResetFetchLobbyTimer();
void MenuSetStartedInTransistion(SDL_bool started);
void MenuSetStartedOutTransistion(SDL_bool started);

void MenuUpdate(FPSManager *fpsManager, Player *player);
void MenuUpdateSplash();
void MenuUpdateName();
void MenuUpdateMainMenu(Player *player, FPSManager *fps);
void MenuUpdateInGameMenu();
void MenuUpdateJoinLobby();
void MenuUpdateHostLobby();
void MenuUpdateWaitingForLobby();
void MenuUpdateLobby();
void MenuUpdateOptions();
void MenuUpdateResolution();
void MenuUpdateFPS(FPSManager *fpsManager);
void MenuUpdateKeybinding();
void MenuUpdateAudio();

void MenuUpdateCustomMap(Player *player);
void MenuUpdateSkin(Player *player);
void MenuTitleDraw(char title[100]);
void MenuDrawPreviewMap();
void MenuDraw(char options[][100], int optionLength);

#endif