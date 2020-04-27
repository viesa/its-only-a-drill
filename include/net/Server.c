#include "Server.h"

void ServerInitialize()
{
    server.players = VectorCreate(sizeof(NetPlayer), 100);
    server.players->resizable = SDL_FALSE;
    server.ids = VectorCreate(sizeof(SDL_bool), 100);
    server.ids->resizable = SDL_FALSE;
    for (size_t i = 0; i < server.ids->capacity; i++)
        SERVER_IDS[i] = SDL_FALSE;
    server.inBuffer = VectorCreate(sizeof(ParsedPacket), 100);
    server.inBufferMutex = SDL_CreateMutex();
    server.isInitialized = SDL_FALSE;
    server.isActive = SDL_FALSE;

    /* Open a socket */
    if (!(server.socket = SDLNet_UDP_Open(1337)))
    {
        log_error("SDLNet_UDP_Open: %s", SDLNet_GetError());
        return;
    }

    server.isInitialized = SDL_TRUE;
}

void ServerUninitialize()
{
    ServerBroadcast(PT_Text, "serverQuit", 11);
    for (size_t i = 0; i < server.inBuffer->size; i++)
    {
        ParsedPacketDestroy(&SERVER_INBUFFER[i]);
    }
    VectorDestroy(server.players);
    VectorDestroy(server.ids);
    VectorDestroy(server.inBuffer);
    SDL_UnlockMutex(server.inBufferMutex);
    SDL_DestroyMutex(server.inBufferMutex);
    SDLNet_UDP_Unbind(server.socket, 0);
    SDLNet_UDP_Close(server.socket);
    server.isInitialized = SDL_FALSE;
}

void ServerStart()
{
    assert("Attempting to start server without initialization" && server.isInitialized);

    server.isActive = SDL_TRUE;
    server.listenThread = SDL_CreateThread((SDL_ThreadFunction)ServerListenToClients, "Server Listen Thread", NULL);
}

void ServerStop()
{
    server.isActive = SDL_FALSE;
    SDL_WaitThread(server.listenThread, NULL);
}

void ServerBroadcast(PacketType type, void *data, int size)
{
    UDPpacket *packet = UDPPacketCreate(type, data, size);
    for (int i = 0; i < server.players->size; i++)
    {
        packet->address = SERVER_PLAYERS[i].ip;
        ServerOut(packet);
    }
    UDPPacketDestroy(packet);
}

void ServerBroadcastExclusive(PacketType type, void *data, int size, IPaddress exclusive)
{
    UDPpacket *packet = UDPPacketCreate(type, data, size);
    for (int i = 0; i < server.players->size; i++)
    {
        if (SERVER_PLAYERS[i].ip.port == exclusive.port && SERVER_PLAYERS[i].ip.host == exclusive.host)
            continue;

        packet->address = SERVER_PLAYERS[i].ip;
        ServerOut(packet);
    }
    UDPPacketDestroy(packet);
}
void ServerSend(PacketType type, void *data, int size, IPaddress ip)
{
    UDPpacket *packet = UDPPacketCreate(type, data, size);
    packet->address = ip;
    ServerOut(packet);
    UDPPacketDestroy(packet);
}

void ServerOut(UDPpacket *packet)
{
    assert("Attempting to send packet without server initialization" && server.isInitialized);

#ifdef SERVER_DEBUG_RAWINOUT
    printf(" -- PACKET OUTGOING TO (%d | %d)\n", packet->address.host, packet->address.port);
    printf(" Type: %d Raw Message: ", UDPPacketDecode(packet->data));
    for (int i = 1; i < packet->len; i++)
    {
        printf("%c", ((char *)packet->data)[i]);
    }
    printf("\n -- END OF PACKET\n");
#endif

    if (!SDLNet_UDP_Send(server.socket, -1, packet))
    {
#ifdef SERVER_DEBUG
        log_warn("Failed to send UDP-packet from server: %s", SDLNet_GetError());
#endif
    }

#ifdef SERVER_DEBUG_STRICT
    switch (UDPPacketDecode(packet->data))
    {
    case PT_Text:
        UDPPacketRemoveType(packet);
        printf("PARSED OUT(({type}) {message}, {host}:{port})\n(CHAR) ");
        for (int i = 0; i < packet->len; i++)
        {
            printf("%c", packet->data[i]);
        }
        printf(", %d:%d\n", packet->address.host, packet->address.port);
        break;
    case PT_PlayerID:
        UDPPacketRemoveType(packet);
        printf("PARSED OUT(({type}) {message}, {host}:{port})\n(INT) %d, %d:%d\n", *(int *)packet->data, packet->address.host, packet->address.port);
        break;
    case PT_Entity:
        UDPPacketRemoveType(packet);
        Entity *entity = (Entity *)packet->data;
        printf("PARSED OUT(({type}) {message}, {host}:{port})\n(ENTITY) x:%f, y:%f, id:%d, %d:%d\n", entity->position.x, entity->position.y, entity->id, packet->address.host, packet->address.port);
        break;
    default:
        UDPPacketRemoveType(packet);
        printf("PARSED OUT(({type}) {message}, {host}:{port})\n(DEFAULT) ");
        for (int i = 0; i < packet->len; i++)
        {
            printf("%c", packet->data[i]);
        }
        printf(", %d:%d\n ", packet->address.host, packet->address.port);
        break;
    }
#endif
}

void ServerListenToClients()
{
    SDL_SetThreadPriority(SDL_THREAD_PRIORITY_LOW);
    while (server.isActive)
    {
        SDL_bool anyReceive = SDL_FALSE;
        UDPpacket *incoming = SDLNet_AllocPacket(MAX_MSGLEN);

        int result = SDLNet_UDP_Recv(server.socket, incoming);
        if (result == -1)
        {
            log_warn("Failed to recieve UDP-packet: %s", SDLNet_GetError());
        }
        if (result)
        {
            SDL_bool exist = SDL_FALSE;

            for (int i = 0; i < server.players->size; i++)
                if (SERVER_PLAYERS[i].ip.host == incoming->address.host &&
                    SERVER_PLAYERS[i].ip.port == incoming->address.port)
                {
                    exist = SDL_TRUE;
                }
            if (!exist && server.players->size != server.players->capacity)
            {
                SERVER_PLAYERS[server.players->size].ip = incoming->address;
                SERVER_PLAYERS[server.players->size].id = -1; // ID will be given as a reply to "alive"-packet
                server.players->size++;
            }

            anyReceive = SDL_TRUE;
            PacketType type = UDPPacketDecode((char *)incoming->data);
            UDPPacketRemoveType(incoming);
            ParsedPacket parsedPacket = ParsedPacketCreate(type, incoming->data, incoming->len, incoming->address);
            SDL_LockMutex(server.inBufferMutex);
            VectorPushBack(server.inBuffer, &parsedPacket);
#ifdef SERVER_DEBUG_RAWINOUT
            printf(" -- PACKET INCOMING FROM (host | port) (%d | %d)\n", parsedPacket.sender.host, parsedPacket.sender.port);
            printf(" Type: %d Raw Message: ", parsedPacket.type);
            for (int i = 0; i < parsedPacket.size; i++)
            {
                printf("%c", ((char *)parsedPacket.data)[i]);
            }
            printf("\n -- END OF PACKET\n");
#endif
#ifdef SERVER_DEBUG_STRICT
            switch (parsedPacket.type)
            {
            case PT_Text:
            {
                printf("PARSED OUT(({type}) {message}, {host}:{port})\n(CHAR) ");
                for (int i = 0; i < parsedPacket.size; i++)
                {
                    printf("%c", ((char *)parsedPacket.data)[i]);
                }
                printf(", %d:%d\n", parsedPacket.sender.host, parsedPacket.sender.port);
            }
            break;
            case PT_PlayerID:
            {
                printf("PARSED OUT(({type}) {message}, {host}:{port})\n(PlayerID) %d, %d:%d\n",
                       *(int *)parsedPacket.data,
                       parsedPacket.sender.host,
                       parsedPacket.sender.port);
            }
            break;
            case PT_Entity:
            {
                Entity *entity = (Entity *)parsedPacket.data;
                printf("PARSED OUT(({type}) {message}, {host}:{port})\n(ENTITY) x:%f, y:%f, id:%d, %d:%d\n",
                       entity->position.x,
                       entity->position.y,
                       entity->id,
                       parsedPacket.sender.host,
                       parsedPacket.sender.port);
            }
            break;
            default:
            {
                printf("PARSED OUT(({type}) {message}, {host}:{port})\n(DEFAULT) ");
                for (int i = 0; i < parsedPacket.size; i++)
                {
                    printf("%c", ((char *)parsedPacket.data)[i]);
                }
                printf(", %d:%d\n", parsedPacket.sender.host, parsedPacket.sender.port);
            }
            break;
            }
#endif
            SDL_UnlockMutex(server.inBufferMutex);
        }
        UDPPacketDestroy(incoming);

        if (!anyReceive)
        {
            SDL_Delay(0);
        }
    }
}

void ServerRemoveClient(IPaddress ip)
{
    int index = -1;
    for (int i = 0; i < server.players->size; i++)
    {
        if (SERVER_PLAYERS[i].ip.host == ip.host &&
            SERVER_PLAYERS[i].ip.port == ip.port)
        {
            index = i;
            break;
        }
    }
    if (index == -1)
        return;
    int id = SERVER_PLAYERS[index].id;
    ServerFreeID(id);
    VectorErase(server.players, index);

    char quitmsg[10] = {0};
    sprintf(quitmsg, "quit %d", SERVER_PLAYERS[index].id);
    ServerBroadcast(PT_Text, quitmsg, strlen(quitmsg));

    printf("Player disconnected [id:%d]\n", id);
}

int ServerGetID(IPaddress ip)
{

    for (int i = 0; i < server.players->capacity; i++)
    {
        if (!SERVER_IDS[i])
        {
            SERVER_IDS[i] = SDL_TRUE;
            return i + 1;
        }
    }
#ifdef SERVER_DEBUG
    log_warn("No available ID's!");
#endif
    return -1;
}

void ServerFreeID(int id)
{
    if (id > 1 || id < server.players->capacity)
        SERVER_IDS[id - 1] = 0;
}

ParsedPacket *ServerGetInBufferArray()
{
    return (ParsedPacket *)server.inBuffer->data;
}
NetPlayer *ServerGetPlayerArray()
{
    return (NetPlayer *)server.players->data;
}
SDL_bool *ServerGetIDArray()
{
    return (SDL_bool *)server.ids->data;
}