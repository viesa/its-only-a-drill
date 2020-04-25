#include "core//AppServer.h"
AppServer AppServerCreate()
{
    AppServer app;
    app.server = UDPServerCreate(1337);
    app.isRunning = SDL_TRUE;
    app.nrPlayers = 0;
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        app.players->id = 0;
    }
    return app;
}
void AppServerGo(AppServer *app)
{
    AppServerListenToClients(app);
    if (app->server.hasPacket)
    {
        AppServerLogic(app);
        AppServerShowPlayerList(&app->server);
        SDLNet_FreePacket(app->server.pack);
        app->server.hasPacket = SDL_FALSE;
    }
}
void AppServerListenToClients(AppServer *app)
{
    if (!app->server.hasPacket)
    {
        UDPServerListen(&app->server, MAX_MSGLEN);
        AppServerCheckPlayerConnection(&app->server, "0alive", "0quit");
    }
}
void AppServerCheckPlayerConnection(UDPServer *server, char joinMsg[], char leaveMsg[])
{
    if (server->hasPacket)
    {
        if (UDPPackageDecode((char *)server->pack->data) == UDPTypeText)
        {
            AppServerCheckPlayerLeaving(server, leaveMsg);
            AppServerCheckPlayerJoining(server, joinMsg);
        }
    }
}
void AppServerCheckPlayerJoining(UDPServer *m_server, char joinMsg[])
{
    char buffer[MAX_MSGLEN];
    SDL_memcpy(buffer, m_server->pack->data, m_server->pack->len);
    UDPpacket p = *m_server->pack;
    p.data = (Uint8 *)buffer;
    if (!strcmp((char *)p.data, joinMsg))
    {
        for (int i = 0; i < m_server->nrPlayers; i++)
        {
            if (m_server->ids[i] == 0)
            {
                m_server->ids[i] = i + 1;
                m_server->players[i].id = i + 1;
                UDPServerSend(m_server, UDPTypeint, &m_server->players[i].id, sizeof(int), p.address);
            }
        }
    }
}
void AppServerCheckPlayerLeaving(UDPServer *server, char leaveMsg[])
{
    SDL_bool exists = SDL_FALSE;
    for (int i = 0; i < server->nrPlayers; i++)
    {
        if (server->players[i].ip.port == server->pack->address.port)
        {
            if (UDPPackageDecode((char *)server->pack->data) == UDPTypeText)
            {
                if (strcmp((char *)server->pack->data, leaveMsg) == 0)
                {
                    char quitmsg[10];
                    sprintf(quitmsg, "quit %d", server->players[i].id);
                    UDPServerEcho(server, UDPTypeText, quitmsg, strlen(quitmsg) + 1);
                    server->nrPlayers--;
                    for (int j = i; j < server->nrPlayers; j++)
                    {
                        server->players[j] = server->players[j + 1];
                    }
                    server->ids[i] = 0;
                    SDL_memset(&server->players[server->nrPlayers], 0, sizeof(UDPPlayer));
                }
            }
            exists = SDL_TRUE;
        }
    }
    if (!exists)
    {
        if (server->nrPlayers == MAX_PLAYERS || strcmp((char *)server->pack->data, leaveMsg) == 0)
            ;
        else
        {
            server->players[server->nrPlayers].ip = server->pack->address;
            server->nrPlayers++;
        }
    }
}
void AppServerEchoFromSender(UDPServer *server)
{
    if (server->hasPacket)
    {
        for (int i = 0; i < server->nrPlayers; i++)
        {
            server->pack->address = server->players[i].ip;
            SDLNet_UDP_Send(server->sock, -1, server->pack);
        }
    }
}
void AppServerBroadcastFromSender(UDPServer *server)
{
    if (server->hasPacket)
    {
        for (int i = 0; i < server->nrPlayers; i++)
        {
            if (server->players[i].ip.host != server->pack->address.host && server->players[i].ip.port != server->pack->address.port)
            {
                server->pack->address = server->players[i].ip;
                SDLNet_UDP_Send(server->sock, -1, server->pack);
            }
        }
    }
}
void AppServerLogic(AppServer *app)
{
    UDPPackageTypes type = UDPPackageDecode((char *)app->server.pack->data);
    switch (type)
    {
    case UDPTypeText:
        UDPPackageRemoveType(app->server.pack);
        break;
    case UDPTypeint:
        UDPPackageRemoveTypeNULL(app->server.pack);
        break;
    case UDPTypeEntity:
        AppServerBroadcastFromSender(&app->server);
        break;
    case UDPTypeCompressedEntity:
        AppServerBroadcastFromSender(&app->server);
        break;
    case UDPTypeIPaddress:
        UDPPackageRemoveTypeNULL(app->server.pack);
        break;
    default:
        break;
    }
}
void AppServerShowPlayers(UDPServer *server)
{
    printf("\nPlayer list\n");
    for (int i = 0; i < server->nrPlayers; i++)
        printf("(host:port): %d:%d\n", server->players[i].ip.host, server->players[i].ip.port);
    printf("length of list %d\n", server->nrPlayers);
}
void AppServerDestroy(AppServer *app)
{
    UDPServerDestroy(&app->server);
}