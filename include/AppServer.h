#ifndef APPSERVER_H
#define APPSERVER_H

#include "Dependencies.h"
#include "Clock.h"
#include "EntityManager.h"
#include "State.h"
#include "ServerManager.h"

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

// Adds deltatime to every client timeout timer
void AppServerUpdateTimeoutTimers(AppServer *app);
// Kicks all clients that hasn't replied to 'alive' packets before time-out
void AppServerKickTimeoutClients(AppServer *app);
// Sends 'alive' packets to all clients, updating their time-out timer
void AppServerPingClients(AppServer *app);

// Handles different kind of packets
void AppServerHandleTextPacket(ParsedPacket packet);
void AppServerHandleAreYouAlivePacket(ParsedPacket packet);
void AppServerHandleIAmAlivePacket(ParsedPacket packet);
void AppServerHandleConnectPacket(ParsedPacket packet);
void AppServerHandleUDPRespondIPPacket(ParsedPacket packet);
void AppServerHandleDisconnectPacket(ParsedPacket packet);
void AppServerHandleEntityPacket(ParsedPacket packet);
void AppServerHandleCompressedEntityPacket(ParsedPacket packet);
void AppServerHandleCreateSessionPacket(ParsedPacket packet);
void AppServerHandleJoinSessionPacket(ParsedPacket packet);
void AppServerHandleLeaveSessionPacket(ParsedPacket packet);
void AppServerHandleStartRoundPacket(ParsedPacket packet);
void AppServerHandleChangeSkinPacket(ParsedPacket packet);
void AppServerHandleFetchLobbyPacket(ParsedPacket packet);
void AppServerHandleFetchSessionsPacket(ParsedPacket packet);
void AppServerHandlePlayerHitPacket(ParsedPacket packet);
void AppServerHandlePlayerDeadPacket(ParsedPacket packet);

#endif