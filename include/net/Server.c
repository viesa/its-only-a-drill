#include "Server.h"

#include "../core/Log.h"
#include "../core/List.h"

Server *ServerCreate(Uint16 port)
{
    if (SDLNet_Init() == -1)
    {
        log_error("Could not initialize net: %s", SDLNet_GetError());
        return NULL;
    }

    Server *ret = (Server *)SDL_malloc(sizeof(Server));

    if (SDLNet_ResolveHost(&ret->m_ip, NULL, port) == -1)
    {
        log_error("Could not resolve host: %s", SDLNet_GetError());
        return NULL;
    }
    ret->m_serverSocket = SDLNet_TCP_Open(&ret->m_ip);
    if (ret->m_serverSocket == NULL)
    {
        log_error("Failed to open port: %s", SDLNet_GetError());
        return NULL;
    }

    ret->m_clients = ListCreate();
    ret->m_socketSet = SDLNet_AllocSocketSet(100);
    if (SDLNet_TCP_AddSocket(ret->m_socketSet, ret->m_serverSocket) == -1)
    {
        log_error("Failed to add socket to socket set: %s", SDLNet_GetError());
        return NULL;
    }

    ret->m_packetMgr = PacketMgrCreate();
    ret->m_active = SDL_TRUE;
    ret->m_worker = SDL_CreateThread((SDL_ThreadFunction)ServerMgr, "MGR", ret);

    return ret;
}
void ServerDestroy(Server *server)
{
    server->m_active = SDL_FALSE;
    PacketMgrDestroy(&server->m_packetMgr);
    for (Node *node = server->m_clients.front; node; node = node->next)
    {
        SDLNet_TCP_DelSocket(server->m_socketSet, ((Connection *)node->data)->socket);
        ConnectionDestroy((Connection *)node->data);
    }
    ListDestroy(&server->m_clients);
    SDLNet_FreeSocketSet(server->m_socketSet);
    SDL_WaitThread(server->m_worker, NULL);
    SDL_free(server);
    SDLNet_Quit();
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