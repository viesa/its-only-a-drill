#ifndef PACKETMGR_H
#define PACKETMGR_H

#include <SDL2/SDL_net.h>

#include "../core/List.h"

#define HEADER_SIZE 10
#define QUERY_SIZE 5
#define TOKEN_SIZE 15
#define BUFFER_SIZE 200

typedef enum Query
{
    Connect = 0,
    Empty,
    Test,
    NQueries
} Query;

typedef struct Payload
{
    Query query;
    void *data;
    size_t size;
} Payload;

typedef struct Connection
{
    TCPsocket socket;
    char token[TOKEN_SIZE];
} Connection;

typedef struct Packet
{
    Payload payload;
    Connection *connection;
    SDL_bool sent;
} Packet;

typedef struct FormatPacket
{
    void *data;
    size_t size;
} FormatPacket;

typedef struct PacketMgr
{
    List incoming;
    List outgoing;
    Payload fallback;
    SDL_mutex *sendLock;
    SDL_mutex *recvLock;
} PacketMgr;

Payload PayloadCreate(Query query, void *data, size_t size);
void PayloadDestroy(Payload *payload);

Connection ConnectionCreate(TCPsocket socket, char token[TOKEN_SIZE]);
void ConnectionDestroy(Connection *connection);

Packet PacketCreate(Payload payload, Connection *connection);
void PacketDestroy(Packet *packet);

FormatPacket FormatPacketCreate(const Packet *packet);
void FormatPacketDestroy(FormatPacket *formatPacket);

PacketMgr PacketMgrCreate();
void PacketMgrDestroy(PacketMgr *packetMgr);

void PacketMgrSend(PacketMgr *packetMgr, Query query, void *data, size_t size, Connection *connection);

Payload PacketMgrPopFront(PacketMgr *packetMgr);
Payload PacketMgrPopBack(PacketMgr *packetMgr);

int PacketMgrReceivePackage(PacketMgr *packetMgr, Connection *connection);
void PacketMgrSendAllPackages(PacketMgr *packetMgr);

void PacketMgrAddOutgoing(PacketMgr *packetMgr, const Packet packet);
void PacketMgrAddIncoming(PacketMgr *packetMgr, const Packet packet);

#endif