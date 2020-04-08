#include "UDPServer.h"
#define DEGBUG
UDPServer UDPServerCreate(Uint16 port)
{
    UDPServer server;
    /* Initialize SDL_net */
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    /* Open a socket */
    if (!(server.sock = SDLNet_UDP_Open(port)))
    {
        fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    return server;
}
void UDPServerBroadcast(UDPServer *server, Uint8 *msg, int size, int ports[], int nrPorts)
{
    /* Make space for the packet */
    if (!(server->pack = SDLNet_AllocPacket(size)))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    memcpy(server->pack->data, msg, size);
    server->pack->len = size;
    for (int i = 0; i < nrPorts; i++)
    {
        server->pack->address.port = ports[i];
        SDLNet_UDP_Send(server->sock, -1, server->pack);
    }
    SDLNet_FreePacket(server->pack);
}
void UDPServerSend(UDPServer *server, Uint8 *msg, int size, int port)
{
    /* Make space for the packet */
    if (!(server->pack = SDLNet_AllocPacket(size)))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    memcpy(server->pack->data, msg, size);
    server->pack->len = size;
    server->pack->address.port = port;
    if (SDLNet_UDP_Send(server->sock, -1, server->pack))
    {
#ifdef DEGBUG
        printf("Sending Message: %s\n", server->pack->data);
#endif
    }
    SDLNet_FreePacket(server->pack);
}
int UDPServerListen(UDPServer *server, int maxLen)
{
    server->pack = SDLNet_AllocPacket(maxLen);
    int r = SDLNet_UDP_Recv(server->sock, server->pack);
    if (!r)
    {
        SDLNet_FreePacket(server->pack);
        return 0;
    }
#ifdef DEGBUG
    printf("Incomming Message: %s\n", server->pack->data);
#endif
    return r;
}
void UDPServerDestroy(UDPServer *server)
{
    SDLNet_UDP_Unbind(server->sock, 0);
    SDLNet_FreePacket(server->pack);
}