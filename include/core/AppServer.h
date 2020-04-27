#ifndef APPSERVER_H
#define APPSERVER_H

#include "Dependencies.h"
#include "Clock.h"
#include "../net_UDP/UDPServer.h"

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
void AppServerHandleTextPacket(ParsedUDPPacket packet);
void AppServerHandlePlayerIDPacket(ParsedUDPPacket packet);
void AppServerHandleEntityPacket(ParsedUDPPacket packet);
void AppServerHandleCompressedEntityPacket(ParsedUDPPacket packet);
void AppServerHandleIPaddressPacket(ParsedUDPPacket packet);
#endif