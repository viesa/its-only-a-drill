#ifndef CLIENT_H
#define CLIENT_H

#include "PacketMgr.h"

typedef struct Client
{
    char *m_ip;
    Uint16 m_port;

    Connection m_server;
    SDLNet_SocketSet m_socketSet;

    PacketMgr m_packetMgr;

    SDL_bool m_active;
    SDL_bool m_connected;

    SDL_Thread *m_worker;
} Client;

Client *ClientCreate(const char *ip, Uint16 port);
void ClientDestroy(Client *client);

void ClientSetNet(Client *client, const char *ip, Uint16 port);
void ClientSetToken(Client *client, char token[TOKEN_SIZE]);

void ClientStart(Client *client);
void ClientStop(Client *client);

Payload ClientPopFront(Client *client);
Payload ClientPopBack(Client *client);

void ClientSend(Client *client, Query query, void *data, size_t size);

void ClientMgr(Client *client);

#endif