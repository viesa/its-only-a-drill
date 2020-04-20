#include "UDPServer.h"
#include <stdio.h>
#define DEGBUG
UDPServer UDPServerCreate(Uint16 port)
{
    UDPServer server;
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
void UDPServerBroadcast(UDPServer *server, UDPPackageTypes types, char *data, int size)
{
#ifndef Backup
    UDPpacket *pack = SDLNet_AllocPacket(size + 2);
    char *payload = UDPPackageCreate(types, data, size);
    SDL_memcpy(pack->data, payload, size + 2);
    pack->len = size + 2;
    for (int i = 0; i < server->nrPlayers; i++)
    {
        pack->address = server->players[i].ip;
        if (SDLNet_UDP_Send(server->sock, -1, pack))
        {
            //printf("OUT(message): %s\n", pack->data);
            printf("RAW OUT(message, host:port): ");
            for (int i = 0; i < server->pack->len; i++)
            {
                printf("%c", (char)server->pack->data[i]);
            }
            printf(", %x:%x\n", server->pack->address.host, server->pack->address.port);
        }
    }
    UDPPackageDestroy(payload);
#endif
#ifdef Backup
    /* Make space for the packet */
    UDPpacket *pack;
    if (!(pack = SDLNet_AllocPacket(size)))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    memcpy(pack->data, msg, size);
    pack->len = size;
    for (int i = 0; i < server->nrPlayers; i++)
    {
        pack->address = server->players[i].ip;
        if (SDLNet_UDP_Send(server->sock, -1, pack))
        {
            //printf("OUT(message): %s\n", pack->data);
            printf("OUT(message, host:port): %s, %x:%x\n", pack->data, pack->address.host, pack->address.port);
        }
    }
#endif
}
void UDPServerSend(UDPServer *server, UDPPackageTypes types, char *data, int size, Uint16 host, Uint16 port)
{
#ifndef Backup
    UDPpacket *pack = SDLNet_AllocPacket(size + 2);
    char *payload = UDPPackageCreate(types, data, size);
    SDL_memcpy(pack->data, payload, size + 2);
    pack->len = size + 2;
    pack->address.host = host;
    pack->address.port = port;
    if (SDLNet_UDP_Send(server->sock, -1, pack))
    {
        //printf("OUT(message): %s\n", pack->data);
        printf("RAW OUT(message, host:port): ");
        for (int i = 0; i < server->pack->len; i++)
        {
            printf("%c", server->pack->data[i]);
        }
        printf(", %x:%x\n", server->pack->address.host, server->pack->address.port);
    }
    UDPPackageDestroy(payload);
#endif
#ifdef Backup
    /* Make space for the packet */
    UDPpacket *pack;
    if (!(pack = SDLNet_AllocPacket(size)))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    memcpy(pack->data, data, size);
    pack->len = size;
    pack->address.port = port;
    if (SDLNet_UDP_Send(server->sock, -1, pack))
    {
        printf("RAW IN(message, host:port): ");
        for (int i = 0; i < server->pack->len; i++)
        {
            printf("%c", server->pack->data[i]);
        }
        printf(", %x:%x\n", server->pack->address.host, server->pack->address.port);
    }
    SDLNet_FreePacket(pack);
#endif
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
}