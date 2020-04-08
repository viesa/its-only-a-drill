#ifndef C_GAME_H
#define C_GAME_H
#include "Clock.h"
#include "Gui.h"
#include "Camera.h"
#include "Input.h"
#include "Menu.h"
#include "Sound.h"
#include "Music.h"
#include "../core/Log.h"
#include "../net/NetworkMgr.h"
#include "../net/Client.h"
#include "Score.h"
#include "Weapons.h"

typedef struct AppClient AppClient;

AppClient *AppClientCreate(SDL_bool *isRunning, Clock *clock, Input *input, Client *client, FpsManger *FPSControls);
void AppClientDestroy(AppClient *app);

void AppClientRun(AppClient *app);

void AppClientUpdate(AppClient *app);
void AppClientDraw(AppClient *app);

#endif