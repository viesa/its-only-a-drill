#ifndef APPSERVER_H
#define APPSERVER_H

#include "Dependencies.h"
#include "Clock.h"
#include "../net/Server.h"

typedef struct AppServer AppServer;

AppServer *AppServerCreate(SDL_bool *isRunning, Clock *clock);
void AppServerDestroy(AppServer *app);

// Main loop
void AppServerGo(AppServer *app);

// Updates the entire network situation for this client
void AppServerUpdate(AppServer *app);

// Displays connected players to the console
void AppServerShowPlayerList(AppServer *app);

// Handles different kind of packets
void AppServerHandleTextPacket(ParsedPacket packet);
void AppServerHandleConnectPacket(ParsedPacket packet);
void AppServerHandleDisconnectPacket(ParsedPacket packet);
void AppServerHandleNewPlayerPacket(ParsedPacket packet);
void AppServerHandleDelPlayerPacket(ParsedPacket packet);
void AppServerHandleEntityPacket(ParsedPacket packet);
void AppServerHandleCompressedEntityPacket(ParsedPacket packet);

#endif