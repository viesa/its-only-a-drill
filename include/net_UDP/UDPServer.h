#ifndef UDPSERVER_h
#define UDPSERVER_h
#include "UDPPackager.h"
#include <SDL2/SDL_net.h>
#define MAX_PLAYERS 10
#define MAX_MSGLEN 100
typedef struct UDPPlayer
{
    IPaddress ip;
    int id;
} UDPPlayer;
typedef struct UDPServer
{
    UDPPlayer players[MAX_PLAYERS];
    int nrPlayers;
    UDPsocket sock;
    UDPpacket *pack;
} UDPServer;
// Creates a UDP server on a fixed port
UDPServer UDPServerCreate(Uint16 port);
// Sends a message to everyone on that has sent a message to the server during the session
void UDPServerEcho(UDPServer *server, UDPPackageTypes types, void *msg, int size);
// Sends a message to everyone except a specified ip
void UDPServerBroadcast(UDPServer *server, UDPPackageTypes types, IPaddress exclusive, void *msg, int size);
// Sends a message to a specified ip address
void UDPServerSend(UDPServer *server, UDPPackageTypes types, void *msg, int length, IPaddress ip);
// Listens on the port given when creating the server and adding "players" to the list if they send any messages to the server and removes them if they send "quit"
int UDPServerListen(UDPServer *server, int len);
// Destroys the server (udp_close(), unbind, sdlnet close)
void UDPServerDestroy(UDPServer *server);
#endif