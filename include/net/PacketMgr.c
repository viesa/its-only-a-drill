
#include "PacketMgr.h"

Payload PayloadCreate(Query query, void *data, size_t size)
{
    Payload ret;
    ret.query = query;
    ret.data = SDL_malloc(size);
    SDL_memcpy(ret.data, data, size);
    ret.size = size;
    return ret;
}

void PayloadDestroy(Payload *payload)
{
    SDL_free(payload->data);
}

Connection ConnectionCreate(TCPsocket socket, char token[TOKEN_SIZE])
{
    Connection ret;
    ret.socket = socket;
    strcpy(ret.token, token);
    return ret;
}

void ConnectionDestroy(Connection *connection)
{
    SDLNet_TCP_Close(connection->socket);
}

Packet PacketCreate(Payload payload, Connection *connection)
{
    Packet ret;
    ret.payload = payload;
    ret.connection = connection;
    return ret;
}

void PacketDestroy(Packet *packet)
{
    PayloadDestroy(&packet->payload);
}

void FillWithSpaces(char *str, size_t size)
{
    for (int i = 0; i < size; i++)
        str[i] = ' ';
}

void ReplaceNullWithSpace(char *str, size_t size)
{
    for (int i = 0; i < size; i++)
        if (!str[i])
            str[i] = ' ';
}

FormatPacket FormatPacketCreate(const Packet *packet)
{
    const Payload *payload = &packet->payload;
    Connection *connection = packet->connection;
    FormatPacket ret;
    if (payload->data)
    {
        size_t size = TOKEN_SIZE + HEADER_SIZE + QUERY_SIZE + payload->size;
        ret.data = SDL_malloc(size);
        ret.size = size;
        char tokenFormatted[TOKEN_SIZE];
        char headerFormatted[HEADER_SIZE];
        char queryFormatted[QUERY_SIZE];
        FillWithSpaces(tokenFormatted, TOKEN_SIZE);
        FillWithSpaces(headerFormatted, HEADER_SIZE);
        FillWithSpaces(queryFormatted, QUERY_SIZE);
        SDL_memcpy(tokenFormatted, connection->token, TOKEN_SIZE);
        SDL_itoa(payload->size, headerFormatted, 10);
        SDL_itoa(payload->query, queryFormatted, 10);
        SDL_memcpy(ret.data, headerFormatted, HEADER_SIZE);
        SDL_memcpy(ret.data + HEADER_SIZE, tokenFormatted, TOKEN_SIZE);
        SDL_memcpy(ret.data + HEADER_SIZE + TOKEN_SIZE, queryFormatted, QUERY_SIZE);
        SDL_memcpy(ret.data + HEADER_SIZE + TOKEN_SIZE + QUERY_SIZE, payload->data, payload->size);
        return ret;
    }
    else
    {
        ret.data = NULL;
        ret.size = 0;
        return ret;
    }
}

void FormatPacketDestroy(FormatPacket *formatPacket)
{
    SDL_free(formatPacket->data);
}

PacketMgr PacketMgrCreate()
{
    PacketMgr packetMgr;
    packetMgr.incoming = ListCreate();
    packetMgr.outgoing = ListCreate();
    packetMgr.fallback = PayloadCreate(Empty, NULL, 0);
    packetMgr.sendLock = SDL_CreateMutex();
    packetMgr.recvLock = SDL_CreateMutex();
    return packetMgr;
}
void PacketMgrDestroy(PacketMgr *packetMgr)
{
    for (Node *node = packetMgr->incoming.front; node; node = node->next)
    {
        PacketDestroy((Packet *)node->data);
    }
    for (Node *node = packetMgr->outgoing.front; node; node = node->next)
    {
        PacketDestroy((Packet *)node->data);
    }
    ListDestroy(&packetMgr->incoming);
    ListDestroy(&packetMgr->outgoing);
    SDL_DestroyMutex(packetMgr->sendLock);
    SDL_DestroyMutex(packetMgr->recvLock);
}

void PacketMgrSend(PacketMgr *packetMgr, Query query, void *data, size_t size, Connection *connection)
{
    SDL_LockMutex(packetMgr->sendLock);
    Payload payload = PayloadCreate(query, data, size);
    Packet packet = PacketCreate(payload, connection);
    ListPushBack(&packetMgr->outgoing, &packet, sizeof(packet));
    SDL_UnlockMutex(packetMgr->sendLock);
}

Payload PacketMgrPopFront(PacketMgr *packetMgr)
{
    SDL_LockMutex(packetMgr->recvLock);
    if (packetMgr->incoming.len > 0)
    {
        Payload ret = (*(Packet *)packetMgr->incoming.front->data).payload;
        ListPopFront(&packetMgr->incoming);
        SDL_UnlockMutex(packetMgr->recvLock);
        return ret;
    }
    else
    {
        SDL_UnlockMutex(packetMgr->recvLock);
        return packetMgr->fallback;
    }
}
Payload PacketMgrPopBack(PacketMgr *packetMgr)
{
    SDL_LockMutex(packetMgr->recvLock);
    if (packetMgr->incoming.len > 0)
    {
        Payload ret = (*(Packet *)packetMgr->incoming.back->data).payload;
        ListPopBack(&packetMgr->incoming);
        SDL_UnlockMutex(packetMgr->recvLock);
        return ret;
    }
    else
    {
        SDL_UnlockMutex(packetMgr->recvLock);
        return packetMgr->fallback;
    }
}

void Strip(char *str, size_t size, char stipFrom)
{
    for (int i = 0; i < size; i++)
        if (str[i] == stipFrom)
        {
            str[i] = 0;
            break;
        }
}

int PacketMgrReceivePackage(PacketMgr *packetMgr, Connection *connection)
{
    char header[HEADER_SIZE];
    char token[TOKEN_SIZE];
    char query_str[QUERY_SIZE];
    if (!SDLNet_TCP_Recv(connection->socket, header, HEADER_SIZE))
    {
        //FAILED TO RECEIVE HEADER DID THE CLIENT DISCONNECT?
        return 0;
    }
    size_t dataSize = atoi(header);
    size_t size = TOKEN_SIZE + QUERY_SIZE + dataSize;

    char rest[size];
    if (!SDLNet_TCP_Recv(connection->socket, rest, size))
    {
        //FAILED TO RECEIVE DATA
        return 0;
    }
    void *data = SDL_malloc(dataSize);
    SDL_memcpy(token, rest, TOKEN_SIZE);
    SDL_memcpy(query_str, rest + TOKEN_SIZE, QUERY_SIZE);
    SDL_memcpy(data, rest + TOKEN_SIZE + QUERY_SIZE, dataSize);
    Query query = atoi(query_str);

    SDL_LockMutex(packetMgr->recvLock);
    PacketMgrAddIncoming(packetMgr, PacketCreate(PayloadCreate(query, data, dataSize), connection));
    SDL_UnlockMutex(packetMgr->recvLock);
    SDL_free(data);
    return 1;
}
void PacketMgrSendAllPackages(PacketMgr *packetMgr)
{
    SDL_LockMutex(packetMgr->sendLock);
    for (Node *node = packetMgr->outgoing.front; node; node = node->next)
    {
        Packet *packet = (Packet *)node->data;
        FormatPacket formatPacket = FormatPacketCreate(packet);
        if (SDLNet_TCP_Send(packet->connection->socket, formatPacket.data, formatPacket.size))
            packet->sent = SDL_TRUE;
    }

    for (Node *node = packetMgr->outgoing.front; node; node = node->next)
        if (((Packet *)node->data)->sent)
            ListEraseNode(&packetMgr->outgoing, node);
    SDL_UnlockMutex(packetMgr->sendLock);
}

void PacketMgrAddOutgoing(PacketMgr *packetMgr, const Packet packet)
{
    ListPushBack(&packetMgr->outgoing, (void *)&packet, sizeof(packet));
}
void PacketMgrAddIncoming(PacketMgr *packetMgr, const Packet packet)
{
    ListPushBack(&packetMgr->incoming, (void *)&packet, sizeof(packet));
}