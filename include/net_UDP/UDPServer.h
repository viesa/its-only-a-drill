#ifndef UDPSERVER_h
#define UDPSERVER_h
#include <SDL2/SDL_net.h>
#define MAX_PLAYERS 5
typedef struct
{
    IPaddress ip;
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
// Sends a message to everyone on that has sent a message to the server capped
void UDPServerBroadcast(UDPServer *server, Uint8 *msg, int size);
void UDPServerSend(UDPServer *server, Uint8 *msg, int length, int port);
int UDPServerListen(UDPServer *server, int len);
void UDPServerDestroy(UDPServer *server);
#endif