#ifndef SERVER_H
#define SERVER_H

#include "PacketMgr.h"

typedef struct Server
{
    IPaddress m_ip;
    Uint16 m_port;

    TCPsocket m_serverSocket;
    List m_clients;
    SDLNet_SocketSet m_socketSet;

    PacketMgr m_packetMgr;

    SDL_bool m_active;
    SDL_bool m_started;

    SDL_Thread *m_worker;
} Server;

Server *ServerCreate();
void ServerDestroy(Server *server);

void ServerSetPort(Server *server, Uint16 port);

void ServerStart(Server *server);
void ServerStop(Server *server);

Payload ServerPopFront(Server *server);
Payload ServerPopBack(Server *server);

void ServerSend(Server *server, Query query, void *data, size_t size, Connection *connection);
void ServerBroadcast(Server *server, Query query, void *data, size_t size);

void ServerMgr(Server *server);

int ServerTryAcceptSocket(Server *server);
void ServerDisconnectClient(Server *server, Connection *connection);

void GenerateToken(char *str);

#endif