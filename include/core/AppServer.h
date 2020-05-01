#ifndef APPSERVER_H
#define APPSERVER_H

#include "Dependencies.h"
#include "Clock.h"
#include "EntityManager.h"
#include "../net/Server.h"
#include "../net/JoinableSesssion.h"

typedef struct AppServer AppServer;

AppServer *AppServerCreate(SDL_bool *isRunning);
void AppServerDestroy(AppServer *app);

// Main loop
void AppServerGo(AppServer *app);

// Handles all incoming packets and updates command line GUI
void AppServerUpdate(AppServer *app);

// Displays connected players to the console
void AppServerShowPlayerList(AppServer *app);

// Handles different kind of packets
void AppServerHandleTextPacket(ParsedPacket packet);
void AppServerHandleConnectPacket(ParsedPacket packet);
void AppServerHandleDisconnectPacket(ParsedPacket packet);
void AppServerHandleEntityPacket(ParsedPacket packet);
void AppServerHandleCompressedEntityPacket(ParsedPacket packet);
void AppServerHandleCreateSessionPacket(ParsedPacket packet);
void AppServerHandleJoinSessionPacket(ParsedPacket packet);
void AppServerHandleLeaveSessionPacket(ParsedPacket packet);
void AppServerHandleFetchSessionsPacket(ParsedPacket packet);

#endif