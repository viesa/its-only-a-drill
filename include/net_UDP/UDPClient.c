#include "UDPClient.h"
#include <stdio.h>
UDPClient UDPClientCreate(const char *ip, Uint16 port)
{
    UDPClient client;
    client.hasInit = SDL_FALSE;
    client.hasPacket = SDL_FALSE;
    client.isActive = SDL_TRUE;
    /* Initialize SDL_net */
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    /* Open a socket on random port */
    if (!(client.sock = SDLNet_UDP_Open(0)))
    {
        fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    /* Resolve server name  */
    if (SDLNet_ResolveHost(&client.serverip, ip, port) == -1)
    {
        fprintf(stderr, "SDLNet_ResolveHost(%s %d): %s\n", ip, port, SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    return client;
}
int UDPClientSend(UDPClient *client, UDPPackageTypes types, void *data, size_t size)
{
    UDPpacket *pack = SDLNet_AllocPacket(size + 2);
    char *payload = UDPPackageCreate(types, data, size);
    SDL_memcpy(pack->data, payload, size + 2);
    pack->address.host = client->serverip.host;
    pack->address.port = client->serverip.port;
    pack->len = size + 2;
    int s = SDLNet_UDP_Send(client->sock, -1, pack);
    UDPPackageDestroy(payload);
    SDLNet_FreePacket(pack);
    return s;
}
int UDPClientListen(UDPClient *client, int maxLen)
{
    client->pack = SDLNet_AllocPacket(maxLen);
    client->pack->address.host = client->serverip.host;
    client->pack->address.port = client->serverip.port;
    int r = SDLNet_UDP_Recv(client->sock, client->pack);
    if (!r)
    {
        client->hasPacket = SDL_FALSE;
        SDLNet_FreePacket(client->pack);
        return 0;
    }
    client->hasPacket = SDL_TRUE;
    return r;
}
void UDPClientDestroy(UDPClient *client)
{
    UDPClientSend(client, UDPTypeText, "quit\0", 5);
    client->pack = NULL;
    SDLNet_FreePacket(client->pack);
    SDLNet_UDP_Close(client->sock);
}