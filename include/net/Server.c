#include "Server.h"

#include <assert.h>

#include "../core/Log.h"
#include "../core/List.h"

Server *ServerCreate()
{
    if (SDLNet_Init() == -1)
    {
        log_error("Could not initialize net: %s", SDLNet_GetError());
        return NULL;
    }

    Server *ret = MALLOC(Server);
    ret->m_port = 0;
    ret->m_clients = ListCreate();
    ret->m_socketSet = SDLNet_AllocSocketSet(100);
    ret->m_packetMgr = PacketMgrCreate();
    ret->m_active = SDL_FALSE;
    ret->m_started = SDL_FALSE;
    ret->m_worker = NULL;
    return ret;
}
void ServerDestroy(Server *server)
{
    ServerStop(server);
    PacketMgrDestroy(&server->m_packetMgr);

    SDLNet_FreeSocketSet(server->m_socketSet);
    SDL_free(server);
    SDLNet_Quit();
}

void ServerSetPort(Server *server, Uint16 port)
{
    server->m_port = port;
}

void ServerStart(Server *server)
{
    if (!server->m_port)
    {
        log_error("Could not start server: Port was not set");
        return;
    }
    if (SDLNet_ResolveHost(&server->m_ip, NULL, server->m_port) == -1)
    {
        log_error("Could not resolve host: %s", SDLNet_GetError());
        return;
    }
    server->m_serverSocket = SDLNet_TCP_Open(&server->m_ip);
    if (!server->m_serverSocket)
    {
        log_error("Failed to open port: %s", SDLNet_GetError());
        return;
    }
    if (SDLNet_TCP_AddSocket(server->m_socketSet, server->m_serverSocket) == -1)
    {
        log_error("Failed to add socket to socket set: %s", SDLNet_GetError());
        return;
    }
    server->m_active = SDL_TRUE;
    server->m_started = SDL_TRUE;
    SDL_CreateThread((SDL_ThreadFunction)ServerMgr, "MGR", server);
}
void ServerStop(Server *server)
{
    if (!server->m_started)
    {
        log_error("Could not stop server: Server was not started");
        return;
    }

    server->m_active = SDL_FALSE;
    server->m_started = SDL_FALSE;
    SDL_WaitThread(server->m_worker, NULL);

    for (Node *node = server->m_clients.front; node; node = node->next)
    {
        SDLNet_TCP_DelSocket(server->m_socketSet, ((Connection *)node->data)->socket);
        ConnectionDestroy((Connection *)node->data);
    }
    ListClear(&server->m_clients);
}

Payload ServerPopFront(Server *server)
{
    return PacketMgrPopFront(&server->m_packetMgr);
}
Payload ServerPopBack(Server *server)
{
    return PacketMgrPopBack(&server->m_packetMgr);
}

void ServerSend(Server *server, Query query, void *data, size_t size, Connection *connection)
{
    PacketMgrSend(&server->m_packetMgr, query, data, size, connection);
}
void ServerBroadcast(Server *server, Query query, void *data, size_t size)
{
    for (Node *node = server->m_clients.front; node; node = node->next)
    {
        ServerSend(server, query, data, size, (Connection *)node->data);
    }
}

void ServerMgr(Server *server)
{
    while (server->m_active)
    {
        int rdy = SDLNet_CheckSockets(server->m_socketSet, 100);
        if (rdy == -1)
        {
            log_error("Failed to check socket in socket set: %s", SDLNet_GetError());
            SDL_Delay(200);
        }
        else if (rdy)
        {
            if (SDLNet_SocketReady(server->m_serverSocket))
                ServerTryAcceptSocket(server);
            else
                for (Node *node = server->m_clients.front; node; node = node->next)
                    if (SDLNet_SocketReady(((Connection *)node->data)->socket))
                        if (!PacketMgrReceivePackage(&server->m_packetMgr, (Connection *)node->data))
                        {
                            ServerDisconnectClient(server, (Connection *)node->data);
                            node = ListErase(&server->m_clients, ListSearch(&server->m_clients, node->data, node->size));
                            if (!node)
                                break;
                            if (!node->prev)
                                break;
                            node = node->prev;
                        }
        }
        PacketMgrSendAllPackages(&server->m_packetMgr);
    }
}

int ServerTryAcceptSocket(Server *server)
{
    TCPsocket new_client = SDLNet_TCP_Accept(server->m_serverSocket);
    if (new_client)
    {
        char token[15] = "TESTTESTTESTTE";
        SDLNet_TCP_AddSocket(server->m_socketSet, new_client);
        Connection connection = ConnectionCreate(new_client, token);
        ListPushBack(&server->m_clients, (void *)&connection, sizeof(Connection));
        return 1;
    }
    else
    {
        return 0;
    }
}

void ServerDisconnectClient(Server *server, Connection *connection)
{
    SDLNet_TCP_DelSocket(server->m_socketSet, connection->socket);
    ConnectionDestroy(connection);
}

void GenerateToken(char *str)
{
    return;
}