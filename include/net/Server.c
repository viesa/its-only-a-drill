#include "Server.h"

void ServerInitialize()
{
    server.socketSet = SDLNet_AllocSocketSet(100);
    server.players = VectorCreate(sizeof(NetPlayer), 100);
    server.players->resizable = SDL_FALSE;
    server.ids = VectorCreate(sizeof(SDL_bool), 100);
    server.ids->resizable = SDL_FALSE;
    for (size_t i = 0; i < server.ids->capacity; i++)
        SERVER_IDS[i] = SDL_FALSE;
    server.currentIDIndex = 0;
    server.inBuffer = VectorCreate(sizeof(ParsedPacket), 100);
    server.inBufferMutex = SDL_CreateMutex();
    server.isInitialized = SDL_FALSE;
    server.isActive = SDL_FALSE;

    const Uint16 port = 1337;

    IPaddress own;
    if (SDLNet_ResolveHost(&own, NULL, port) == -1)
    {
        log_error("Could not resolve host: %s", SDLNet_GetError());
        return;
    }

    // Open UDP-socket
    if (!(server.udpSocket = SDLNet_UDP_Open(port)))
    {
        log_error("Failed to open port (UDP): %s", SDLNet_GetError());
        return;
    }
    // Add UDP-socket to socket set
    if (SDLNet_TCP_AddSocket(server.socketSet, server.tcpSocket) == -1)
    {
        log_error("Failed to add socket to socket set (UDP): %s", SDLNet_GetError());
        return;
    }

    // Open TCP-socket
    if (!(server.tcpSocket = SDLNet_TCP_Open(&own)))
    {
        log_error("Failed to open port (TCP): %s", SDLNet_GetError());
        return;
    }
    // Add TCP-socket to socket set
    if (SDLNet_TCP_AddSocket(server.socketSet, server.tcpSocket) == -1)
    {
        log_error("Failed to add socket to socket set (TCP): %s", SDLNet_GetError());
        return;
    }

    server.isInitialized = SDL_TRUE;
}

void ServerUninitialize()
{
    ServerUDPBroadcast(PT_Disconnect, NULL, 0);
    SDLNet_UDP_DelSocket(server.socketSet, server.udpSocket);
    SDLNet_UDP_Unbind(server.udpSocket, 0);
    SDLNet_UDP_Close(server.udpSocket);
    SDLNet_TCP_DelSocket(server.socketSet, server.tcpSocket);
    SDLNet_TCP_Close(server.tcpSocket);
    for (size_t i = 0; i < server.players->size; i++)
    {
        SDLNet_TCP_DelSocket(server.socketSet, SERVER_PLAYERS[i].socket);
        SDLNet_TCP_Close(SERVER_PLAYERS[i].socket);
    }
    SDLNet_FreeSocketSet(server.socketSet);

    for (size_t i = 0; i < server.inBuffer->size; i++)
    {
        ParsedPacketDestroy(&SERVER_INBUFFER[i]);
    }
    VectorDestroy(server.players);
    VectorDestroy(server.ids);
    VectorDestroy(server.inBuffer);
    SDL_UnlockMutex(server.inBufferMutex);
    SDL_DestroyMutex(server.inBufferMutex);
    server.isInitialized = SDL_FALSE;
}

void ServerStart()
{
    assert("Attempting to start server without server initialization" && server.isInitialized);

    server.isActive = SDL_TRUE;
    server.listenThread = SDL_CreateThread((SDL_ThreadFunction)ServerListenToClients, "Server Listen Thread", NULL);
}

void ServerStop()
{
    assert("Attempting to stop server without server initialization" && server.isInitialized);

    server.isActive = SDL_FALSE;
    SDL_WaitThread(server.listenThread, NULL);
}

void ServerUDPBroadcast(PacketType type, void *data, int size)
{
    // Creates a UDP-packet to send. ID of server is 0
    UDPpacket *packet = UDPPacketCreate(type, 0, data, size);
    for (int i = 0; i < server.players->size; i++)
    {
        packet->address = *SERVER_PLAYERS[i].ip;
        ServerUDPOut(packet);
    }
    UDPPacketDestroy(packet);
}

void ServerUDPBroadcastExclusive(PacketType type, void *data, int size, NetPlayer exclusive)
{
    // Creates a UDP-packet to send. ID of server is 0
    UDPpacket *packet = UDPPacketCreate(type, 0, data, size);
    for (int i = 0; i < server.players->size; i++)
    {
        if (SERVER_PLAYERS[i].entity.id == exclusive.entity.id)
            continue;

        packet->address = *SERVER_PLAYERS[i].ip;
        ServerUDPOut(packet);
    }
    UDPPacketDestroy(packet);
}
void ServerUDPSend(PacketType type, void *data, int size, NetPlayer player)
{
    // Creates a UDP-packet to send. ID of server is 0
    UDPpacket *packet = UDPPacketCreate(type, 0, data, size);
    packet->address = *player.ip;
    ServerUDPOut(packet);
    UDPPacketDestroy(packet);
}

void ServerUDPOut(UDPpacket *packet)
{
    assert("Attempting to send packet without server initialization" && server.isInitialized);

    if (!SDLNet_UDP_Send(server.udpSocket, -1, packet))
    {
#ifdef SERVER_DEBUG
        log_warn("Failed to send UDP-packet to client: %s", SDLNet_GetError());
#endif
    }
    PacketPrintInformation(PacketDecodeType(packet->data),
                           PacketDecodeID(packet->data),
                           packet->data,
                           packet->len,
                           packet->address,
                           "UDP", "OUTGOING");
}

void ServerTCPBroadcast(PacketType type, void *data, int size)
{
    // Creates a TCP-packet to send. ID of server is 0
    TCPpacket *packet = TCPPacketCreate(type, 0, data, size);
    for (int i = 0; i < server.players->size; i++)
    {
        packet->address = SERVER_PLAYERS[i].socket;
        ServerTCPOut(packet);
    }
    TCPPacketDestroy(packet);
}

void ServerTCPBroadcastExclusive(PacketType type, void *data, int size, NetPlayer exclusive)
{
    // Creates a TCP-packet to send. ID of server is 0
    TCPpacket *packet = TCPPacketCreate(type, 0, data, size);
    for (int i = 0; i < server.players->size; i++)
    {
        if (SERVER_PLAYERS[i].entity.id == exclusive.entity.id)
            continue;

        packet->address = SERVER_PLAYERS[i].socket;
        ServerTCPOut(packet);
    }
    TCPPacketDestroy(packet);
}

void ServerTCPSend(PacketType type, void *data, int size, NetPlayer player)
{
    // Creates a TCP-packet to send. ID of server is 0
    TCPpacket *packet = TCPPacketCreate(type, 0, data, size);
    packet->address = player.socket;
    ServerTCPOut(packet);
    TCPPacketDestroy(packet);
}

void ServerTCPOut(TCPpacket *packet)
{
    assert("Attempting to send packet without server initialization" && server.isInitialized);

    if (!SDLNet_TCP_Send(packet->address, packet->data, packet->len))
    {
#ifdef SERVER_DEBUG
        log_warn("Failed to send TCP-packet to client: %s", SDLNet_GetError());
#endif
    }
    PacketPrintInformation(PacketDecodeType(((char *)packet->data) + TCP_HEADER_SIZE),
                           PacketDecodeID(((char *)packet->data) + TCP_HEADER_SIZE),
                           packet->data,
                           packet->len,
                           *SDLNet_TCP_GetPeerAddress(packet->address),
                           "TCP", "OUTGOING");
}

void ServerListenToClients()
{
    assert("Attempting to start listener thread on server without server initialization" && server.isInitialized);

    SDL_SetThreadPriority(SDL_THREAD_PRIORITY_LOW);
    while (server.isActive)
    {
        int nReadySockets = SDLNet_CheckSockets(server.socketSet, 20);
        if (!nReadySockets)
        {
            // If no sockets are ready, swap thread
            SDL_Delay(0);
        }
        else if (nReadySockets)
        {
            // Check if the socket ready was TCP-socket -> New connection incoming
            if (SDLNet_SocketReady(server.tcpSocket))
            {
                ServerTryAcceptTCPSocket(server);
                if (--nReadySockets == 0)
                    continue;
            }
            // Check if the socket ready was UDP-socket
            if (SDLNet_SocketReady(server.udpSocket))
            {
                ServerTryReceiveUDPPacket();
                if (--nReadySockets == 0)
                    continue;
            }
            // Check if the socket ready was a client-socket
            for (size_t i = 0; i < server.players->size; i++)
            {
                if (SDLNet_SocketReady(SERVER_PLAYERS[i].socket))
                {
                    ServerTryReceiveTCPPacket(SERVER_PLAYERS[i]);
                    if (--nReadySockets == 0)
                        break;
                }
            }
        }
        else if (nReadySockets == -1)
        {
#ifdef SERVER_DEBUG
            log_error("Failed to check socket in socket set: %s", SDLNet_GetError());
#endif
        }
    }
}

int ServerTryReceiveUDPPacket()
{
    UDPpacket *incoming = SDLNet_AllocPacket(MAX_MSGLEN);

    int result = SDLNet_UDP_Recv(server.udpSocket, incoming);
    if (result == -1)
    {
#ifdef SERVER_DEBUG
        log_warn("Failed to recieve UDP-packet: %s", SDLNet_GetError());
#endif
    }
    else if (result)
    {
        if (incoming->len < NET_TYPE_SIZE + NET_ID_SIZE)
        {
#ifdef SERVER_DEBUG
            log_info("Received a UDP-packet that was too small, throwing away...");
#endif
            UDPPacketDestroy(incoming);
            result = 0;
        }
        else
        {
            // Parse the data of the packet
            PacketType type = PacketDecodeType(incoming->data);
            int id = PacketDecodeID(incoming->data);
            UDPPacketRemoveTypeAndID(incoming);
            if (id < 0)
            {
#ifdef SERVER_DEBUG
                log_info("Received a TCP-packet with invalid ID, throwing away...");
#endif
                UDPPacketDestroy(incoming);
                return 0;
            }

            NetPlayer *sender = NULL;
            for (int i = 0; i < server.players->size; i++)
            {
                if (SERVER_PLAYERS[i].ip->host == incoming->address.host &&
                    SERVER_PLAYERS[i].entity.id == id)
                {
                    sender = &SERVER_PLAYERS[i];
                }
            }
            if (sender != NULL)
            {
                ParsedPacket parsedPacket = ParsedPacketCreate(type, incoming->data, incoming->len, *sender);
                SDL_LockMutex(server.inBufferMutex);
                VectorPushBack(server.inBuffer, &parsedPacket);
                PacketPrintInformation(parsedPacket.type,
                                       parsedPacket.sender.entity.id,
                                       parsedPacket.data,
                                       parsedPacket.size,
                                       *parsedPacket.sender.ip,
                                       "UDP", "INCOMING");
                SDL_UnlockMutex(server.inBufferMutex);
            }
            else
            {
#ifdef SERVER_DEBUG
                log_info("Received a UDP-packet with no associated TCP-connection, throwing away...");
#endif
                UDPPacketDestroy(incoming);
                return 0;
            }
        }
    }
    UDPPacketDestroy(incoming);

    return result;
}

int ServerTryAcceptTCPSocket()
{
    if (server.players->size >= server.players->capacity - 1)
    {
        return 0;
    }

    TCPsocket newClient = SDLNet_TCP_Accept(server.tcpSocket);
    if (newClient)
    {
        SDLNet_TCP_AddSocket(server.socketSet, newClient);
        NetPlayer newPlayer = NetPlayerCreate(newClient, ServerGetID());
        ServerTCPSend(PT_Connect, &newPlayer.id, sizeof(int), newPlayer);
        VectorPushBack(server.players, &newPlayer);
#ifdef SERVER_DEBUG
        log_info("New Player connection (id:%d) (IP | PORT) (%d | %p)", newPlayer.entity.id, newPlayer.ip->host, newPlayer.ip->port);
#endif
        return 1;
    }
    else
    {
#ifdef SERVER_DEBUG
        log_warn("Server could not accept the new TCP-connection: %s", SDLNet_GetError());
#endif
        return 0;
    }
}

int ServerTryReceiveTCPPacket(NetPlayer player)
{
    char header[TCP_HEADER_SIZE] = {0};
    // First receive the header to know how big the packet is
    if (SDLNet_TCP_Recv(player.socket, header, TCP_HEADER_SIZE))
    {
        const size_t packetSize = SDL_atoi(header);
        SDL_bool badPacket = SDL_FALSE;
        if (packetSize < NET_TYPE_SIZE + NET_ID_SIZE)
        {
#ifdef SERVER_DEBUG
            log_info("Received a TCP-packet that was too small, throwing away...");
#endif
            badPacket = SDL_TRUE;
        }
        char rest[packetSize];
        SDL_memset(rest, 0, packetSize);
        // Then receive the full packet
        if (SDLNet_TCP_Recv(player.socket, rest, packetSize))
        {
            if (badPacket)
                return 0;
            // Parse data
            PacketType type = PacketDecodeType(rest);
            int id = PacketDecodeID(rest);
            int dataLength = TCPPacketRemoveTypeAndID(rest, packetSize);
            if (id < 0)
            {
#ifdef SERVER_DEBUG
                log_info("Received a TCP-packet with invalid ID, throwin packet...");
#endif
            }
            ParsedPacket parsedPacket = ParsedPacketCreate(type, rest, dataLength, player);
            // Add to inBuffer.
            // This type of packet is always sent to inBuffer as it might be a "greeting"-packet
            SDL_LockMutex(server.inBufferMutex);
            VectorPushBack(server.inBuffer, &parsedPacket);
            PacketPrintInformation(parsedPacket.type,
                                   parsedPacket.sender.entity.id,
                                   parsedPacket.data,
                                   parsedPacket.size,
                                   *parsedPacket.sender.ip,
                                   "TCP", "INCOMING");
            SDL_UnlockMutex(server.inBufferMutex);

            return 1;
        }
        else
        {
#ifdef SERVER_DEBUG
            log_warn("Failed to receive TCP-packet from client (IP | PORT) (%d | %d)", player.ip->host, player.ip->port);
#endif
            return 0;
        }
    }
    else
    {
#ifdef SERVER_DEBUG
        log_warn("Failed to receive TCP-header from client (IP | PORT) (%d | %d)", player.ip->host, player.ip->port);
#endif
        return 0;
    }
}

void ServerRemoveClient(NetPlayer player)
{
    int index = -1;
    for (int i = 0; i < server.players->size; i++)
    {
        if (!SDL_memcmp(&SERVER_PLAYERS[i], &player, sizeof(NetPlayer)))
        {
            index = i;
            break;
        }
    }
    // If NetPlayer was not found in current players,
    // it was already deleted -> Return
    if (index == -1)
        return;

    // Remove client from player-list before broadcasting
    VectorErase(server.players, index);

    ServerFreeID(player.entity.id);
    printf("Player disconnected [id:%d]\n", player.entity.id);
}

int ServerGetID()
{
    for (int i = 0; i != server.players->capacity; i++)
    {
        int finalIndex = server.currentIDIndex + i;
        if (finalIndex == server.players->capacity)
            finalIndex -= server.players->capacity;

        if (!SERVER_IDS[finalIndex])
        {
            SERVER_IDS[finalIndex] = SDL_TRUE;
            server.currentIDIndex = finalIndex + 1;
            if (server.currentIDIndex == server.players->capacity)
                server.currentIDIndex = 0;
            return finalIndex + 1;
        }
    }
#ifdef SERVER_DEBUG
    log_warn("No available ID's!");
#endif
    return -1;
}

void ServerFreeID(int id)
{
    if (id > 0 || id <= server.players->capacity)
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