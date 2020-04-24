#include "UDPServer.h"
#define DEGBUG
//#define RAWINOUT
UDPServer UDPServerCreate(Uint16 port)
{
    UDPServer server;
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        server.ids[i] = 0;
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
#ifdef RAWINOUT
            printf("RAW OUT(message, host:port): ");
            for (int i = 0; i < size + 2; i++)
            {
                printf("%c", pack->data[i]);
            }
            printf(", %d:%d\n", pack->address.host, pack->address.port);
#endif
#ifndef RAWINOUT
            char buffer[MAX_MSGLEN];
            SDL_memcpy(buffer, pack->data, pack->len);
            UDPpacket p = *pack;
            p.data = (Uint8 *)buffer;
            switch (UDPPackageDecode((char *)pack->data))
            {
            case UDPTypeText:
                UDPPackageRemoveType(&p);
                printf("PARSED OUT(({type}) {message}, {host}:{port})\n(CHAR) %s, %d:%d\n", (char *)p.data, p.address.host, p.address.port);
                break;
            case UDPTypeint:
                UDPPackageRemoveTypeNULL(&p);
                printf("PARSED OUT(({type}) {message}, {host}:{port})\n(INT) %d, %d:%d\n", *(int *)p.data, p.address.host, p.address.port);
                break;
            case UDPTypeEntity:
                UDPPackageRemoveTypeNULL(&p);
                Entity *e = (Entity *)p.data;
                printf("PARSED OUT(({type}) {message}, {host}:{port})\n(ENTITY) x:%f, y:%f, id:%d, %d:%d\n", e->position.x, e->position.y, e->id, p.address.host, p.address.port);
                break;
            default:
                printf("PARSED OUT(({type}) {message}, {host}:{port})\n(DEFAULT) ");
                for (int i = 0; i < p.len; i++)
                {
                    printf("%c", p.data[i]);
                }
                printf(", %d:%d\n ", p.address.host, p.address.port);
                break;
            }
#endif
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
#ifdef RAWINOUT
                printf("RAW OUT(message, host:port): ");
                for (int i = 0; i < size + 2; i++)
                {
                    printf("%c", pack->data[i]);
                }
                printf(", %d:%d\n", server->pack->address.host, server->pack->address.port);
#endif
#ifndef RAWINOUT
                char buffer[MAX_MSGLEN];
                SDL_memcpy(buffer, server->pack->data, server->pack->len);
                UDPpacket p = *server->pack;
                p.data = (Uint8 *)buffer;
                switch (UDPPackageDecode((char *)server->pack->data))
                {
                case UDPTypeText:
                    UDPPackageRemoveType(&p);
                    printf("PARSED OUT(({type}) {message}, {host}:{port})\n(CHAR) %s, %d:%d\n", (char *)p.data, p.address.host, p.address.port);
                    break;
                case UDPTypeint:
                    UDPPackageRemoveTypeNULL(&p);
                    printf("PARSED OUT(({type}) {message}, {host}:{port})\n(INT) %d, %d:%d\n", *(int *)p.data, p.address.host, p.address.port);
                    break;
                case UDPTypeEntity:
                    UDPPackageRemoveTypeNULL(&p);
                    Entity *e = (Entity *)p.data;
                    printf("PARSED OUT(({type}) {message}, {host}:{port})\n(ENTITY) x:%f, y:%f, id:%d, %d:%d\n", e->position.x, e->position.y, e->id, p.address.host, p.address.port);
                    break;
                default:
                    printf("PARSED OUT(({type}) {message}, {host}:{port})\n(DEFAULT) ");
                    for (int i = 0; i < p.len; i++)
                    {
                        printf("%c", p.data[i]);
                    }
                    printf(", %d:%d\n ", p.address.host, p.address.port);
                    break;
                }
#endif
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
#ifdef RAWINOUT
        printf("RAW OUT(message, host:port): ");
        for (int i = 0; i < size + 2; i++)
        {
            printf("%c", pack->data[i]);
        }
        printf(", %d:%d\n", server->pack->address.host, server->pack->address.port);
#endif
#ifndef RAWINOUT
        char buffer[MAX_MSGLEN];
        SDL_memcpy(buffer, pack->data, server->pack->len);
        UDPpacket p = *pack;
        p.data = (Uint8 *)buffer;
        switch (UDPPackageDecode((char *)pack->data))
        {
        case UDPTypeText:
            UDPPackageRemoveType(&p);
            printf("PARSED OUT(({type}) {message}, {host}:{port})\n(CHAR) %s, %d:%d\n", (char *)p.data, p.address.host, p.address.port);
            break;
        case UDPTypeint:
            UDPPackageRemoveTypeNULL(&p);
            printf("PARSED OUT(({type}) {message}, {host}:{port})\n(INT) %d, %d:%d\n", *(int *)p.data, p.address.host, p.address.port);
            break;
        case UDPTypeEntity:
            UDPPackageRemoveTypeNULL(&p);
            Entity *e = (Entity *)p.data;
            printf("PARSED OUT(({type}) {message}, {host}:{port})\n(ENTITY) x:%f, y:%f, id:%d, %d:%d\n", e->position.x, e->position.y, e->id, p.address.host, p.address.port);
            break;
        default:
            printf("PARSED OUT(({type}) {message}, {host}:{port})\n(DEFAULT) ");
            for (int i = 0; i < p.len; i++)
            {
                printf("%c", p.data[i]);
            }
            printf(", %d:%d\n ", p.address.host, p.address.port);
            break;
        }
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
                    char buffer[20];
                    sprintf(buffer, "quit %d", server->players->id);
                    UDPServerEcho(server, UDPTypeText, buffer, strlen(buffer) + 1);
                    server->nrPlayers--;
                    for (int j = i; j < server->nrPlayers; j++)
                    {
                        server->players[j] = server->players[j + 1];
                    }
                    server->ids[i] = 0;
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
        printf("(host:port): %d:%d\n", server->players[i].ip.host, server->players[i].ip.port);
    printf("length of list %d\n", server->nrPlayers);
    printf("End of player list\n\n");
#ifdef RAWINOUT
    printf("RAW IN(message, host:port): ");
    for (int i = 0; i < server->pack->len; i++)
    {
        printf("%c", server->pack->data[i]);
    }
    printf(", %d:%d\n", server->pack->address.host, server->pack->address.port);
#endif
#ifndef RAWINOUT
    char buffer[MAX_MSGLEN];
    SDL_memcpy(buffer, server->pack->data, server->pack->len);
    UDPpacket p = *server->pack;
    p.data = (Uint8 *)buffer;
    switch (UDPPackageDecode((char *)server->pack->data))
    {
    case UDPTypeText:
        UDPPackageRemoveType(&p);
        printf("PARSED IN(({type}) {message}, {host}:{port})\n(CHAR) %s, %d:%d\n", (char *)p.data, p.address.host, p.address.port);
        break;
    case UDPTypeint:
        UDPPackageRemoveTypeNULL(&p);
        printf("PARSED IN(({type}) {message}, {host}:{port})\n(INT) %d, %d:%d\n", *(int *)p.data, p.address.host, p.address.port);
        break;
    case UDPTypeEntity:
        UDPPackageRemoveTypeNULL(&p);
        Entity *e = (Entity *)p.data;
        printf("PARSED IN(({type}) {message}, {host}:{port})\n(ENTITY) x:%f, y:%f, id:%d, %d:%d\n", e->position.x, e->position.y, e->id, p.address.host, p.address.port);
        break;
    default:
        printf("PARSED IN(({type}) {message}, {host}:{port})\n(DEFAULT) ");
        for (int i = 0; i < p.len; i++)
        {
            printf("%c", p.data[i]);
        }
        printf(", %d:%d\n ", p.address.host, p.address.port);
        break;
    }
#endif
#endif
    return r;
}
void UDPServerDestroy(UDPServer *server)
{
    SDLNet_UDP_Unbind(server->sock, 0);
    SDLNet_UDP_Close(server->sock);
    SDLNet_Quit();
}