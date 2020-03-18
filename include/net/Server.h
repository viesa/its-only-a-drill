#ifndef SERVER_H
#define SERVER_H

#include "PacketMgr.h"

typedef struct Server
{
    IPaddress m_ip;

    TCPsocket m_serverSocket;
    List m_clients;
    SDLNet_SocketSet m_socketSet;

    PacketMgr m_packetMgr;

    SDL_bool m_active;

    SDL_Thread *m_worker;
} Server;

Server *ServerCreate(Uint16 port);
void ServerDestroy(Server *server);

Payload ServerPopFront(Server *server);
Payload ServerPopBack(Server *server);

void ServerSend(Server *server, Query query, void *data, size_t size, Connection *connection);
void ServerBroadcast(Server *server, Query query, void *data, size_t size);

void ServerMgr(Server *server);

int ServerTryAcceptSocket(Server *server);
void ServerDisconnectClient(Server *server, Connection *connection);

void GenerateToken(char *str);

#endif