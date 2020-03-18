#include "Client.h"

Client *ClientCreate(const char *ip, Uint16 port)
{
    if (SDLNet_Init() == -1)
    {
        fprintf(stderr, "Error: Could not initialize net");
    }
    Client *ret = (Client *)SDL_malloc(sizeof(Client));
    ret->m_ip = (char *)SDL_malloc(strlen(ip));
    strcpy(ret->m_ip, ip);
    ret->m_port = port;
    ret->m_socketSet = SDLNet_AllocSocketSet(1);
    ret->m_packetMgr = PacketMgrCreate();
    ret->m_active = SDL_TRUE;
    ret->m_connected = SDL_FALSE;
    return ret;
}
void ClientDestroy(Client *client)
{
    client->m_active = SDL_FALSE;
    PacketMgrDestroy(&client->m_packetMgr);
    ClientDisconnect(client);
    SDL_free(client->m_ip);
    SDL_WaitThread(client->m_worker, NULL);
    SDL_free(client);
}

void ClientConnect(Client *client)
{
    IPaddress serverIP;
    if (SDLNet_ResolveHost(&serverIP, client->m_ip, client->m_port) == -1)
    {
        //Error count not resolve host
        return;
    }
    client->m_server = ConnectionCreate(SDLNet_TCP_Open(&serverIP), "");
    if (client->m_server.socket == NULL)
    {
        //Error Failed to open port
        return;
    }

    SDLNet_TCP_AddSocket(client->m_socketSet, client->m_server.socket);

    client->m_worker = SDL_CreateThread((SDL_ThreadFunction)ClientMgr, "MGR", client);
    client->m_connected = SDL_TRUE;
}
void ClientDisconnect(Client *client)
{
    if (client->m_connected)
    {
        client->m_connected = SDL_FALSE;
        SDLNet_TCP_DelSocket(client->m_socketSet, client->m_server.socket);
        SDLNet_FreeSocketSet(client->m_socketSet);
        ConnectionDestroy(&client->m_server);
        client->m_server.socket = NULL;
    }
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

void ClientSetToken(Client *client, char token[TOKEN_SIZE])
{
    strcpy(client->m_server.token, token);
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
                        ClientDisconnect(client);
                    }
            PacketMgrSendAllPackages(&client->m_packetMgr);
        }
    }
}