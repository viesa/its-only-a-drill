#include "UDPClient.h"
#include <stdio.h>
UDPClient UDPClientCreate(const char *ip, Uint16 port)
{
    UDPClient client;
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
int UDPClientSend(UDPClient *client, void *data, size_t size)
{
    client->pack = SDLNet_AllocPacket(size);
    memcpy(client->pack->data, data, size);
    client->pack->address.host = client->serverip.host;
    client->pack->address.port = client->serverip.port;
    client->pack->len = size;
    int s = SDLNet_UDP_Send(client->sock, -1, client->pack);
    SDLNet_FreePacket(client->pack);
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
        SDLNet_FreePacket(client->pack);
        return 0;
    }
    return r;
}
void UDPClientDestroy(UDPClient *client)
{
    client->pack = NULL;
    SDLNet_UDP_Close(client->sock);
}