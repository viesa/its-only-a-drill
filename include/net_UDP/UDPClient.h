#ifndef UDPCLIENT_H
#define UDPCLIENT_H
#include <SDL2/SDL_net.h>
#include "UDPPackager.h"
#define MAX_MSGLEN 1024
typedef struct UDPClient
{
    UDPpacket *pack;
    SDL_bool hasPacket;
    SDL_bool isActive;
    IPaddress serverip;
    UDPsocket sock;
    SDL_bool hasInit;
} UDPClient;
// Creates a UDP client and targets it towards ip:port
UDPClient UDPClientCreate(const char *ip, Uint16 port);
// Sends the data with the absolute size to the server that was configured in UDPClientCreate.
// NOTE: this function will free the packet when sending
int UDPClientSend(UDPClient *client, UDPPackageTypes types, void *data, size_t size);
// Listens for packets once from the server and puts them at the UDPpacket data pointer using maxLen so sdl_net is preventing overflow.
int UDPClientListen(UDPClient *client, int maxLen);
// Destroy the client: UDP_close() and free the UDPpacket
void UDPClientDestroy(UDPClient *client);
#endif