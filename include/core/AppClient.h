#ifndef C_GAME_H
#define C_GAME_H
#include "Graphics.h"
#include "Clock.h"
#include "Font.h"
#include "Gui.h"
#include "Camera.h"
#include "Input.h"
#include "Sound.h"
#include "../net/NetworkMgr.h"
#include "../net/Client.h"

typedef struct AppClient AppClient;

AppClient *AppClientCreate(Clock *clock, SDL_bool *running, Input *input, Client *client);
void AppClientDestroy(AppClient *app);

void AppClientRun(AppClient *app);

void AppClientUpdate(AppClient *app);
void AppClientDraw(AppClient *app);

#endif