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
#include "State.h"
#include "EntityManager.h"
#include "Cursor.h"
#include "../net_UDP/UDPClient.h"
#include "../net_UDP/UDPManager.h"

typedef struct AppClient AppClient;

AppClient *AppClientCreate(SDL_bool *isRunning, Clock *clock, Input *input, FPSManager *fpsManager);
void AppClientDestroy(AppClient *app);

void AppClientRun(AppClient *app);

void AppClientUpdate(AppClient *app);
void AppClientDraw(AppClient *app);

#endif