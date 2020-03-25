#ifndef S_GAME_H
#define S_GAME_H
#include "Clock.h"
#include "Input.h"
#include "../net/NetworkMgr.h"
#include "../net/Server.h"

typedef struct AppServer AppServer;

AppServer *AppServerCreate(Clock *clock, SDL_bool *running, Input *input, Server *server);
void AppServerDestroy(AppServer *app);

void AppServerRun(AppServer *app);

void AppServerUpdate(AppServer *app);

#endif