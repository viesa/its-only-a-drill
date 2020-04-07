#ifndef UDPSERVER_h
#define UDPSERVER_h
#include <SDL2/SDL_net.h>
typedef struct UDPServer
{
    UDPsocket sock;
    UDPpacket *pack;
} UDPServer;
UDPServer UDPServerCreate(Uint16 port);
void UDPServerBroadcast(UDPServer *server, Uint8 *msg, int size, int ports[], int nrPorts);
void UDPServerSend(UDPServer *server, Uint8 *msg, int length, int port);
int UDPServerListen(UDPServer *server, int len);
void UDPServerDestroy(UDPServer *server);
#endif