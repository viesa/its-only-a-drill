#ifndef APPSERVER_H
#define APPSERVER_H

#include "Dependencies.h"
#include "Clock.h"
#include "EntityManager.h"
#include "../net/Server.h"
#include "../net/JoinableSesssion.h"

typedef enum CLIState
{
    CS_Main,
    CS_PlayerList,
    CS_SessionList,
    CS_Traffic,
    CS_None,
    CS_Count
} CLIState;

typedef struct AppServer AppServer;

AppServer *AppServerCreate(SDL_bool *isRunning);
void AppServerDestroy(AppServer *app);

// Main loop
void AppServerGo(AppServer *app);

// Handles all incoming packets
void AppServerHandleAllPackets(AppServer *app);

// Thread function to update command line interface, CLI
void AppServerUpdateCLI(AppServer *app);

void AppServerDrawCLI(AppServer *app);

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