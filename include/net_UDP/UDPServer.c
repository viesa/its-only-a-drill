#include "UDPServer.h"

void UDPServerInitialize()
{
    udpServer.players = VectorCreate(sizeof(UDPPlayer), 100);
    udpServer.players->resizable = SDL_FALSE;
    udpServer.ids = VectorCreate(sizeof(SDL_bool), 100);
    udpServer.ids->resizable = SDL_FALSE;
    for (size_t i = 0; i < udpServer.ids->capacity; i++)
        UDPSERVER_IDS[i] = SDL_FALSE;
    udpServer.inBuffer = VectorCreate(sizeof(ParsedUDPPacket), 100);
    udpServer.inBufferMutex = SDL_CreateMutex();
    udpServer.isInitialized = SDL_FALSE;
    udpServer.isActive = SDL_FALSE;

    /* Initialize SDL_net */
    if (SDLNet_Init() < 0)
    {
        log_error("SDLNet_Init: %s", SDLNet_GetError());
        return;
    }
    /* Open a socket */
    if (!(udpServer.socket = SDLNet_UDP_Open(1337)))
    {
        log_error("SDLNet_UDP_Open: %s", SDLNet_GetError());
        return;
    }

    udpServer.isInitialized = SDL_TRUE;
}

void UDPServerUninitialize()
{
    UDPServerBroadcast(UDPType_Text, "serverQuit", 11);
    for (size_t i = 0; i < udpServer.inBuffer->size; i++)
    {
        ParsedUDPPacketDestroy(&UDPSERVER_INBUFFER[i]);
    }
    VectorDestroy(udpServer.players);
    VectorDestroy(udpServer.ids);
    VectorDestroy(udpServer.inBuffer);
    SDL_UnlockMutex(udpServer.inBufferMutex);
    SDL_DestroyMutex(udpServer.inBufferMutex);
    SDLNet_UDP_Unbind(udpServer.socket, 0);
    SDLNet_UDP_Close(udpServer.socket);
    SDLNet_Quit();
    udpServer.isInitialized = SDL_FALSE;
}

void UDPServerStart()
{
    assert("Attempting to start server without initialization" && udpServer.isInitialized);

    udpServer.isActive = SDL_TRUE;
    udpServer.listenThread = SDL_CreateThread((SDL_ThreadFunction)UDPServerListenToClients, "Server Listen Thread", NULL);
}

void UDPServerStop()
{
    udpServer.isActive = SDL_FALSE;
    SDL_WaitThread(udpServer.listenThread, NULL);
}

void UDPServerBroadcast(UDPPacketType type, void *data, int size)
{
    UDPpacket *packet = UDPPacketCreate(type, data, size);
    for (int i = 0; i < udpServer.players->size; i++)
    {
        packet->address = UDPSERVER_PLAYERS[i].ip;
        UDPServerOut(packet);
    }
    UDPPacketDestroy(packet);
}

void UDPServerBroadcastExclusive(UDPPacketType type, void *data, int size, IPaddress exclusive)
{
    UDPpacket *packet = UDPPacketCreate(type, data, size);
    for (int i = 0; i < udpServer.players->size; i++)
    {
        if (UDPSERVER_PLAYERS[i].ip.port == exclusive.port && UDPSERVER_PLAYERS[i].ip.host == exclusive.host)
            continue;

        packet->address = UDPSERVER_PLAYERS[i].ip;
        UDPServerOut(packet);
    }
    UDPPacketDestroy(packet);
}
void UDPServerSend(UDPPacketType type, void *data, int size, IPaddress ip)
{
    UDPpacket *packet = UDPPacketCreate(type, data, size);
    packet->address = ip;
    UDPServerOut(packet);
    UDPPacketDestroy(packet);
}

void UDPServerOut(UDPpacket *packet)
{
    assert("Attempting to send packet without server initialization" && udpServer.isInitialized);

#ifdef UDPSERVER_DEBUG_RAWINOUT
    printf(" -- PACKET OUTGOING TO (%d | %d)\n", packet->address.host, packet->address.port);
    printf(" Type: %d Raw Message: ", UDPPacketDecode(packet->data));
    for (int i = 1; i < packet->len; i++)
    {
        printf("%c", ((char *)packet->data)[i]);
    }
    printf("\n -- END OF PACKET\n");
#endif

    if (!SDLNet_UDP_Send(udpServer.socket, -1, packet))
    {
#ifdef UDPSERVER_DEBUG
        log_warn("Failed to send UDP-packet from server: %s", SDLNet_GetError());
#endif
    }

#ifdef UDPSERVER_DEBUG_STRICT
    switch (UDPPacketDecode(packet->data))
    {
    case UDPType_Text:
        UDPPacketRemoveType(packet);
        printf("PARSED OUT(({type}) {message}, {host}:{port})\n(CHAR) ");
        for (int i = 0; i < packet->len; i++)
        {
            printf("%c", packet->data[i]);
        }
        printf(", %d:%d\n", packet->address.host, packet->address.port);
        break;
    case UDPType_PlayerID:
        UDPPacketRemoveType(packet);
        printf("PARSED OUT(({type}) {message}, {host}:{port})\n(INT) %d, %d:%d\n", *(int *)packet->data, packet->address.host, packet->address.port);
        break;
    case UDPType_Entity:
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

void UDPServerListenToClients()
{
    SDL_SetThreadPriority(SDL_THREAD_PRIORITY_LOW);
    while (udpServer.isActive)
    {
        SDL_bool anyReceive = SDL_FALSE;
        UDPpacket *incoming = SDLNet_AllocPacket(MAX_MSGLEN);

        int result = SDLNet_UDP_Recv(udpServer.socket, incoming);
        if (result == -1)
        {
            log_warn("Failed to recieve UDP-packet: %s", SDLNet_GetError());
        }
        if (result)
        {
            SDL_bool exist = SDL_FALSE;

            for (int i = 0; i < udpServer.players->size; i++)
                if (UDPSERVER_PLAYERS[i].ip.host == incoming->address.host &&
                    UDPSERVER_PLAYERS[i].ip.port == incoming->address.port)
                {
                    exist = SDL_TRUE;
                }
            if (!exist && udpServer.players->size != udpServer.players->capacity)
            {
                UDPSERVER_PLAYERS[udpServer.players->size].ip = incoming->address;
                UDPSERVER_PLAYERS[udpServer.players->size].id = -1; // ID will be given as a reply to "alive"-packet
                udpServer.players->size++;
            }

            anyReceive = SDL_TRUE;
            UDPPacketType type = UDPPacketDecode((char *)incoming->data);
            UDPPacketRemoveType(incoming);
            ParsedUDPPacket parsedPacket = ParsedUDPPacketCreate(type, incoming->data, incoming->len, incoming->address);
            SDL_LockMutex(udpServer.inBufferMutex);
            VectorPushBack(udpServer.inBuffer, &parsedPacket);
#ifdef UDPSERVER_DEBUG_RAWINOUT
            printf(" -- PACKET INCOMING FROM (host | port) (%d | %d)\n", parsedPacket.sender.host, parsedPacket.sender.port);
            printf(" Type: %d Raw Message: ", parsedPacket.type);
            for (int i = 0; i < parsedPacket.size; i++)
            {
                printf("%c", ((char *)parsedPacket.data)[i]);
            }
            printf("\n -- END OF PACKET\n");
#endif
#ifdef UDPSERVER_DEBUG_STRICT
            switch (parsedPacket.type)
            {
            case UDPType_Text:
            {
                printf("PARSED OUT(({type}) {message}, {host}:{port})\n(CHAR) ");
                for (int i = 0; i < parsedPacket.size; i++)
                {
                    printf("%c", ((char *)parsedPacket.data)[i]);
                }
                printf(", %d:%d\n", parsedPacket.sender.host, parsedPacket.sender.port);
            }
            break;
            case UDPType_PlayerID:
            {
                printf("PARSED OUT(({type}) {message}, {host}:{port})\n(PlayerID) %d, %d:%d\n",
                       *(int *)parsedPacket.data,
                       parsedPacket.sender.host,
                       parsedPacket.sender.port);
            }
            break;
            case UDPType_Entity:
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
            SDL_UnlockMutex(udpServer.inBufferMutex);
        }
        UDPPacketDestroy(incoming);

        if (!anyReceive)
        {
            SDL_Delay(0);
        }
    }
}

void UDPServerRemoveClient(IPaddress ip)
{
    int index = -1;
    for (int i = 0; i < udpServer.players->size; i++)
    {
        if (UDPSERVER_PLAYERS[i].ip.host == ip.host &&
            UDPSERVER_PLAYERS[i].ip.port == ip.port)
        {
            index = i;
            break;
        }
    }
    if (index == -1)
        return;
    int id = UDPSERVER_PLAYERS[index].id;
    UDPServerFreeID(id);
    VectorErase(udpServer.players, index);

    char quitmsg[10] = {0};
    sprintf(quitmsg, "quit %d", UDPSERVER_PLAYERS[index].id);
    UDPServerBroadcast(UDPType_Text, quitmsg, strlen(quitmsg));

    printf("Player disconnected [id:%d]\n", id);
}

int UDPServerGetID(IPaddress ip)
{

    for (int i = 0; i < udpServer.players->capacity; i++)
    {
        if (!UDPSERVER_IDS[i])
        {
            UDPSERVER_IDS[i] = SDL_TRUE;
            return i + 1;
        }
    }
#ifdef UDPSERVER_DEBUG
    log_warn("No available ID's!");
#endif
    return -1;
}

void UDPServerFreeID(int id)
{
    if (id < 1 || id > udpServer.players->capacity)
        return;
    UDPSERVER_IDS[id - 1] = 0;
}

ParsedUDPPacket *UDPServerGetInBufferArray()
{
    return (ParsedUDPPacket *)udpServer.inBuffer->data;
}
UDPPlayer *UDPServerGetPlayerArray()
{
    return (UDPPlayer *)udpServer.players->data;
}
SDL_bool *UDPServerGetIDArray()
{
    return (SDL_bool *)udpServer.ids->data;
}