#ifndef UDPCLIENT_H
#define UDPCLIENT_H
#include <SDL2/SDL_net.h>
typedef struct UDPClient
{
    UDPpacket *pack;
    IPaddress serverip;
    UDPsocket sock;
} UDPClient;
UDPClient UDPClientCreate(const char *ip, Uint16 port);
int UDPClientSend(UDPClient *client, void *data, size_t size);
int UDPClientListen(UDPClient *client, int maxLen);
void UDPClientDestroy(UDPClient *client);
#endif