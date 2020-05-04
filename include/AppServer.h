#ifndef APPSERVER_H
#define APPSERVER_H

#include "Dependencies.h"
#include "Clock.h"
#include "EntityManager.h"
#include "State.h"
#include "Server.h"
#include "JoinableSesssion.h"

typedef struct AppServer AppServer;

AppServer *AppServerCreate(SDL_bool *isRunning);
void AppServerDestroy(AppServer *app);

// Main loop
void AppServerGo(AppServer *app);

// Handles all incoming packets
void AppServerHandleAllPackets(AppServer *app);

// Thread function to update command line interface, CLI
void AppServerUpdateCLI(AppServer *app);

void AppServerDrawTitle(AppServer *app);
void AppServerDrawCLI(AppServer *app);
void AppServerClearTerminal(AppServer *app);

Session *ServerGetSessionByID(int sessionID);

// Handles different kind of packets
void AppServerHandleTextPacket(ParsedPacket packet);
void AppServerHandleConnectPacket(ParsedPacket packet);
void AppServerHandleDisconnectPacket(ParsedPacket packet);
void AppServerHandleEntityPacket(ParsedPacket packet);
void AppServerHandleCompressedEntityPacket(ParsedPacket packet);
void AppServerHandleCreateSessionPacket(ParsedPacket packet);
void AppServerHandleJoinSessionPacket(ParsedPacket packet);
void AppServerHandleLeaveSessionPacket(ParsedPacket packet);
void AppServerHandleFetchLobbyPacket(ParsedPacket packet);
void AppServerHandleFetchSessionsPacket(ParsedPacket packet);
void AppServerHandlePlayerHitPacket(ParsedPacket packet);
#endif