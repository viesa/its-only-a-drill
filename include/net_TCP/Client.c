#include "Client.h"

#include "../core/Log.h"

Client *ClientCreate(const char *ip, Uint16 port)
{
    Client *ret = MALLOC(Client);
    ret->m_port = 0;
    ret->m_ip = NULL;
    ret->m_socketSet = SDLNet_AllocSocketSet(1);
    ret->m_packetMgr = PacketMgrCreate();
    ret->m_active = SDL_FALSE;
    ret->m_connected = SDL_FALSE;
    ret->m_worker = NULL;
    return ret;
}
void ClientDestroy(Client *client)
{
    PacketMgrDestroy(&client->m_packetMgr);
    if (client->m_connected)
        ClientStop(client);
    if (client->m_ip != NULL)
        SDL_free(client->m_ip);
    SDL_free(client);
}

void ClientSetNet(Client *client, const char *ip, Uint16 port)
{
    if (client->m_ip)
        SDL_free(client->m_ip);
    client->m_ip = (char *)SDL_malloc(strlen(ip));
    strcpy(client->m_ip, ip);
    client->m_port = port;
}

void ClientSetToken(Client *client, char token[TOKEN_SIZE])
{
    strcpy(client->m_server.token, token);
}

void ClientStart(Client *client)
{
    if (!client->m_ip || !client->m_port)
    {
        log_error("Could not start client: IP or Port was not set");
        return;
    }
    IPaddress serverIP;
    if (SDLNet_ResolveHost(&serverIP, client->m_ip, client->m_port) == -1)
    {
        log_error("Could not resolve host: %s", SDLNet_GetError());
        return;
    }
    client->m_server = ConnectionCreate(SDLNet_TCP_Open(&serverIP), "");
    if (client->m_server.socket == NULL)
    {
        log_error("Failed to open port: %s", SDLNet_GetError());
        return;
    }

    SDLNet_TCP_AddSocket(client->m_socketSet, client->m_server.socket);

    client->m_connected = SDL_TRUE;
    client->m_active = SDL_TRUE;
    client->m_worker = SDL_CreateThread((SDL_ThreadFunction)ClientMgr, "MGR", client);
}
void ClientStop(Client *client)
{
    if (!client->m_connected)
    {
        log_error("Could not stop client: Client was not started");
        return;
    }
    client->m_connected = SDL_FALSE;
    client->m_active = SDL_FALSE;
    SDL_WaitThread(client->m_worker, NULL);

    SDLNet_TCP_DelSocket(client->m_socketSet, client->m_server.socket);
    SDLNet_FreeSocketSet(client->m_socketSet);
    ConnectionDestroy(&client->m_server);
    client->m_server.socket = NULL;
}

Payload ClientPopFront(Client *client)
{
    return PacketMgrPopFront(&client->m_packetMgr);
}
Payload ClientPopBack(Client *client)
{
    return PacketMgrPopBack(&client->m_packetMgr);
}

void ClientSend(Client *client, Query query, void *data, size_t size)
{
    if (client->m_connected)
        PacketMgrSend(&client->m_packetMgr, query, data, size, &client->m_server);
}

void ClientMgr(Client *client)
{
    while (client->m_active)
    {
        if (client->m_connected)
        {
            if (SDLNet_CheckSockets(client->m_socketSet, 100))
                if (SDLNet_SocketReady(client->m_server.socket))
                    if (!PacketMgrReceivePackage(&client->m_packetMgr, &client->m_server))
                    {
                        ClientStop(client);
                    }
            PacketMgrSendAllPackages(&client->m_packetMgr);
        }
    }
}