#ifndef C_GAME_H
#define C_GAME_H

#include "Dependencies.h"

#include "Clock.h"
#include "FPSManager.h"
#include "Gui.h"
#include "Transition.h"
#include "Camera.h"
#include "Input.h"
#include "Menu.h"
#include "Sound.h"
#include "Music.h"
#include "Cursor.h"
#include "Items.h"
#include "Player.h"
#include "Map.h"
#include "MapList.h"
#include "Weapon.h"
#include "Inventory.h"
#include "Keybinding.h"
#include "Notification.h"
#include "NPCManager.h"

typedef struct AppClient AppClient;

AppClient *AppClientCreate(SDL_bool *isRunning);
void AppClientDestroy(AppClient *app);

void AppClientRun(AppClient *app);

void AppClientUpdate(AppClient *app);
void AppClientDraw(AppClient *app);

#endif