#ifndef C_GAME_H
#define C_GAME_H
#include "Clock.h"
#include "Gui.h"
#include "Camera.h"
#include "Input.h"
#include "Menu.h"
#include "Sound.h"
#include "Music.h"
#include "State.h"
#include "../core/Log.h"
#include "../net_UDP/UDPClient.h"
#include "Score.h"

typedef struct AppClient AppClient;

AppClient *AppClientCreate(SDL_bool *isRunning, Clock *clock, Input *input, UDPClient *client, FpsManger *FPSControls);
void AppClientDestroy(AppClient *app);

void AppClientRun(AppClient *app);

void AppClientUpdate(AppClient *app);
void AppClientDraw(AppClient *app);

#endif