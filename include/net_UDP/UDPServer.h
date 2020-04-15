#ifndef UDPSERVER_h
#define UDPSERVER_h
#include <SDL2/SDL_net.h>
typedef struct
{
    IPaddress ip;
} UDPPlayer;
typedef struct UDPServer
{
    UDPPlayer players[5];
    int nrPlayers;
    UDPsocket sock;
    UDPpacket *pack;
} UDPServer;
UDPServer UDPServerCreate(Uint16 port);
void UDPServerBroadcast(UDPServer *server, Uint8 *msg, int size);
void UDPServerSend(UDPServer *server, Uint8 *msg, int length, int port);
int UDPServerListen(UDPServer *server, int len);
void UDPServerDestroy(UDPServer *server);
#endif