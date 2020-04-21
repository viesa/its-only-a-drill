#include "UDPServer.h"
#include <stdio.h>
#define DEGBUG
UDPServer UDPServerCreate(Uint16 port)
{
    UDPServer server;
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        server.players[i].id = 0;
    }
    server.nrPlayers = 0;
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
void UDPServerBroadcast(UDPServer *server, UDPPackageTypes types, IPaddress exclusive, void *data, int size)
{
    UDPpacket *pack = SDLNet_AllocPacket(size + 2);
    char *payload = UDPPackageCreate(types, data, size);
    SDL_memcpy(pack->data, payload, size + 2);
    pack->len = size + 2;
    for (int i = 0; i < server->nrPlayers; i++)
    {
        if (server->players[i].ip.port == exclusive.port && server->players[i].ip.host == exclusive.host)
        {
            continue;
        }
        pack->address = server->players[i].ip;
        if (SDLNet_UDP_Send(server->sock, -1, pack))
        {
            printf("RAW OUT(message, host:port): ");
            for (int i = 0; i < size + 2; i++)
            {
                printf("%c", pack->data[i]);
            }
            printf(", %x:%x\n", server->pack->address.host, server->pack->address.port);
        }
    }
    UDPPackageDestroy(payload);
    SDLNet_FreePacket(pack);
}
void UDPServerEcho(UDPServer *server, UDPPackageTypes types, void *data, int size)
{
    UDPpacket *pack;
    if ((pack = SDLNet_AllocPacket(size + 2)))
    {
        char *payload = UDPPackageCreate(types, data, size);
        SDL_memcpy(pack->data, payload, size + 2);
        pack->len = size + 2;
        for (int i = 0; i < server->nrPlayers; i++)
        {
            pack->address = server->players[i].ip;
            if (SDLNet_UDP_Send(server->sock, -1, pack))
            {
                printf("RAW OUT(message, host:port): ");
                for (int i = 0; i < size + 2; i++)
                {
                    printf("%c", pack->data[i]);
                }
                printf(", %x:%x\n", server->pack->address.host, server->pack->address.port);
            }
        }
        UDPPackageDestroy(payload);
        SDLNet_FreePacket(pack);
    }
}
void UDPServerSend(UDPServer *server, UDPPackageTypes types, void *data, int size, IPaddress ip)
{
    UDPpacket *pack = SDLNet_AllocPacket(size + 2);
    char *payload = UDPPackageCreate(types, data, size);
    SDL_memcpy(pack->data, payload, size + 2);
    pack->len = size + 2;

    pack->address = ip;
    if (SDLNet_UDP_Send(server->sock, -1, pack))
    {
#ifdef DEGBUG
        printf("RAW OUT(message, host:port): ");
        for (int i = 0; i < size + 2; i++)
        {
            printf("%c", pack->data[i]);
        }
        printf(", %x:%x\n", server->pack->address.host, server->pack->address.port);
#endif
    }
    UDPPackageDestroy(payload);
    SDLNet_FreePacket(pack);
}
int UDPServerListen(UDPServer *server, int maxLen)
{
    server->pack = SDLNet_AllocPacket(maxLen);
    int exists = 0;
    int r = SDLNet_UDP_Recv(server->sock, server->pack);
    if (!r)
    {
        SDLNet_FreePacket(server->pack);
        return 0;
    }
#ifdef DEGBUG
    system("clear");
#endif
    for (int i = 0; i < server->nrPlayers; i++)
    {
        if (server->players[i].ip.port == server->pack->address.port)
        {
            if (UDPPackageDecode((char *)server->pack->data) == UDPTypeText)
            {
                if (strcmp((char *)server->pack->data, "0quit") == 0)
                {
                    server->nrPlayers--;
                    for (int j = i; j < server->nrPlayers; j++)
                    {
                        server->players[j] = server->players[j + 1];
                    }
                    SDL_memset(&server->players[server->nrPlayers], 0, sizeof(UDPPlayer));
                }
            }
            exists = 1;
        }
    }
    if (!exists)
    {
        if (server->nrPlayers == MAX_PLAYERS || strcmp((char *)server->pack->data, "0quit") == 0)
            ;
        else
        {
            server->players[server->nrPlayers].ip = server->pack->address;
            server->nrPlayers++;
        }
    }
#ifdef DEGBUG
    printf("\nPlayer list\n");
    for (int i = 0; i < server->nrPlayers; i++)
        printf("(host:port): %x:%x\n", server->players[i].ip.host, server->players[i].ip.port);
    printf("length of list %d\n", server->nrPlayers);
    printf("End of player list\n\n");
    printf("RAW IN(message, host:port): ");
    for (int i = 0; i < server->pack->len; i++)
    {
        printf("%c", server->pack->data[i]);
    }
    printf(", %x:%x\n", server->pack->address.host, server->pack->address.port);
#endif
    return r;
}
void UDPServerDestroy(UDPServer *server)
{
    SDLNet_UDP_Unbind(server->sock, 0);
    SDLNet_UDP_Close(server->sock);
    SDLNet_Quit();
}