#ifndef APPSERVER_H
#define APPSERVER_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include "../net_UDP/UDPServer.h"
#define MAX_PLAYERS 10
typedef struct AppServer
{
    SDL_bool isRunning;
    UDPServer server;
    UDPPlayer players[MAX_PLAYERS];
    int nrPlayers;
} AppServer;
AppServer AppServerCreate();
void AppServerGo(AppServer *app);
void AppServerListenToClients(AppServer *args);
void AppServerCheckPlayerConnection(UDPServer *server, char joinMsg[], char leaveMsg[]);
void AppServerCheckPlayerJoining(UDPServer *m_server, char joinMsg[]);
void AppServerCheckPlayerLeaving(UDPServer *server, char leaveMsg[]);
void AppServerEchoFromSender(UDPServer *server);
void AppServerBroadcastFromSender(UDPServer *server);
void AppServerLogic(AppServer *app);
void AppServerShowPlayerList(UDPServer *server);
void AppServerDestroy(AppServer *app);
#endif