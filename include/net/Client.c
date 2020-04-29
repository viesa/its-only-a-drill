#include "Client.h"

void ClientInitialize()
{
    client.socketSet = SDLNet_AllocSocketSet(2);
    client.player = NULL;
    client.isInitialized = SDL_FALSE;
    client.isActive = SDL_FALSE;
    client.inBuffer = VectorCreate(sizeof(ParsedPacket), 100);
    client.inBufferMutex = SDL_CreateMutex();

#ifdef LOCAL_SERVER
    const char *ip = "127.0.0.1";
    Uint16 port = 1337;
#else
    const char *ip = "85.226.160.126"; //drill.pierrelf.com port 1337
    Uint16 port = 1337;
#endif

    IPaddress serverIP;
    // Resolve host to retrieve SDL IPaddress pointing to server
    if (SDLNet_ResolveHost(&serverIP, ip, port) == -1)
    {
        log_error("Failed to resolve host: (%s:%d): %s", ip, port, SDLNet_GetError());
        return;
    }

    // Open TCP-socket
    TCPsocket tcpSocket;
    if (!(tcpSocket = SDLNet_TCP_Open(&serverIP)))
    {
        log_error("Failed to open port (TCP) (%s:%d)): %s", ip, port, SDLNet_GetError());
        // This will be dealt with later...
        //return;
    }
    client.server = NetPlayerCreate(tcpSocket, 0);
    // Add TCP-socket to socket set
    if (SDLNet_TCP_AddSocket(client.socketSet, client.server.socket) == -1)
    {
        log_error("Failed to add socket to socket set (TCP): %s", SDLNet_GetError());
        return;
    }

    client.isInitialized = SDL_TRUE;
}

void ClientUninitialize()
{
    ClientTCPSend(PT_Disconnect, NULL, 0);
    SDLNet_UDP_DelSocket(client.socketSet, client.udpSocket);
    SDLNet_UDP_Close(client.udpSocket);
    SDLNet_TCP_DelSocket(client.socketSet, client.server.socket);
    SDLNet_TCP_Close(client.server.socket);
    SDLNet_FreeSocketSet(client.socketSet);

    for (size_t i = 0; i < client.inBuffer->size; i++)
    {
        ParsedPacketDestroy(&CLIENT_INBUFFER[i]);
    }
    VectorDestroy(client.inBuffer);
    SDL_UnlockMutex(client.inBufferMutex);
    SDL_DestroyMutex(client.inBufferMutex);
    client.isInitialized = SDL_FALSE;
}

void ClientSetPlayer(Player *player)
{
    client.player = player;
}

void ClientStart()
{
    assert("Attempting to start client without client initialization" && client.isInitialized);
    assert("Attempting to start client without player set" && client.player != NULL);

    client.isActive = SDL_TRUE;
    client.listenThread = SDL_CreateThread((SDL_ThreadFunction)ClientListenToServer, "Server Listen Thread", NULL);
    ClientTCPSend(PT_Connect, "MyName", 9);
}

void ClientStop()
{
    client.isActive = SDL_FALSE;
    SDL_WaitThread(client.listenThread, NULL);
}

int ClientUDPSend(PacketType type, void *data, size_t size)
{
    if (!client.server.socket)
        return 0;

    assert("Attempting to send UDP-packet without client initialization" && client.isInitialized);
    assert("Attempting to send UDP-packet without player set" && client.player != NULL);

    UDPpacket *outgoing = UDPPacketCreate(type, ENTITY_ARRAY[*client.player->entity].id, data, size);
    outgoing->address.host = client.server.ip->host;
    outgoing->address.port = client.server.ip->port;

    int result = ClientUDPOut(outgoing);

    UDPPacketDestroy(outgoing);

    return result;
}

int ClientUDPOut(UDPpacket *packet)
{
    if (!SDLNet_UDP_Send(client.udpSocket, -1, packet))
    {
#ifdef CLIENT_DEBUG
        log_warn("Failed to send UDP-packet to server: %s", SDLNet_GetError());
#endif
        return 0;
    }
    PacketPrintInformation(PacketDecodeType(packet->data),
                           PacketDecodeID(packet->data),
                           packet->data,
                           packet->len,
                           packet->address,
                           "UDP", "OUTGOING");
    return 1;
}

int ClientTCPSend(PacketType type, void *data, size_t size)
{
    if (!client.server.socket)
        return 0;
    assert("Attempting to send TCP-packet without client initialization" && client.isInitialized);
    assert("Attempting to send TCP-packet without player set" && client.player != NULL);

    if (!client.server.socket)
    {
#ifdef CLIENT_DEBUG
        log_warn("Attempted to send a package to server without being connected to it");
#endif
        return 0;
    }
    int id = ENTITY_ARRAY[*client.player->entity].id;
    TCPpacket *outgoing = TCPPacketCreate(type, id, data, size);
    outgoing->address = client.server.socket;

    int result = ClientTCPOut(outgoing);

    TCPPacketDestroy(outgoing);

    return result;
}

int ClientTCPOut(TCPpacket *packet)
{
    int result = 0;
    if (packet->len <= TCP_MAX_SIZE)
    {
        result = ClientTCPSmallOut(packet);
    }
    else
    {
        result = ClientTCPBigOut(packet);
    }

    PacketPrintInformation(PacketDecodeType(((char *)packet->data) + TCP_HEADER_SIZE),
                           PacketDecodeID(((char *)packet->data) + TCP_HEADER_SIZE),
                           packet->data,
                           packet->len,
                           *SDLNet_TCP_GetPeerAddress(packet->address),
                           "TCP", "OUTGOING");
    return result;
}

int ClientTCPSmallOut(TCPpacket *packet)
{
    if (!SDLNet_TCP_Send(packet->address, packet->data, packet->len))
    {
#ifdef CLIENT_DEBUG
        log_warn("Failed to send TCP-packet to server: %s", SDLNet_GetError());
#endif
        return 0;
    }
    return 1;
}

int ClientTCPBigOut(TCPpacket *packet)
{
    int totalSizeToSend = (int)packet->len;
    int nFullPackagesRequired = (int)(totalSizeToSend / TCP_MAX_SIZE);
    int bytesLeft = totalSizeToSend % TCP_MAX_SIZE;

    for (int i = 0; i < nFullPackagesRequired; i++)
    {
        if (!SDLNet_TCP_Send(packet->address, packet->data + i * TCP_MAX_SIZE, TCP_MAX_SIZE))
        {
#ifdef CLIENT_DEBUG
            log_warn("Failed to send TCP-packet to server: %s", SDLNet_GetError());
#endif
            return 0;
        }
    }
    if (!SDLNet_TCP_Send(packet->address, packet->data + nFullPackagesRequired * TCP_MAX_SIZE, bytesLeft))
    {
#ifdef CLIENT_DEBUG
        log_warn("Failed to send TCP-packet to server: %s", SDLNet_GetError());
#endif
        return 0;
    }
    return 1;
}

void ClientListenToServer()
{
    assert("Attempting to start client listener without client initialization" && client.isInitialized);
    assert("Attempting to start client listener without player set" && client.player != NULL);

    SDL_SetThreadPriority(SDL_THREAD_PRIORITY_LOW);
    while (client.isActive)
    {
        int nReadySockets = SDLNet_CheckSockets(client.socketSet, 20);
        if (!nReadySockets)
        {
            // If no sockets are ready, swap thread
            SDL_Delay(0);
        }
        else if (nReadySockets)
        {
            // Check if the socket ready was TCP-socket -> New connection incoming
            if (SDLNet_SocketReady(client.server.socket))
            {
                ClientTryReceiveTCPPacket();
                if (--nReadySockets == 0)
                    continue;
            }
            // Check if the socket ready was UDP-socket
            if (SDLNet_SocketReady(client.udpSocket))
            {
                ClientTryReceiveUDPPacket();
                if (--nReadySockets == 0)
                    continue;
            }
        }
        else if (nReadySockets == -1)
        {
#ifdef CLIENT_DEBUG
            log_error("Failed to check socket in socket set: %s", SDLNet_GetError());
#endif
        }
    }
}

int ClientTryReceiveUDPPacket()
{
    UDPpacket *incoming = SDLNet_AllocPacket(MAX_MSGLEN);
    incoming->address.host = client.server.ip->host;
    incoming->address.port = client.server.ip->port;

    int result = SDLNet_UDP_Recv(client.udpSocket, incoming);
    if (result == -1)
    {
#ifdef CLIENT_DEBUG
        log_warn("Failed to recieve UDP-packet: ", SDLNet_GetError());
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
            PacketType type = PacketDecodeType(incoming->data);
            int id = PacketDecodeID(incoming->data);
            UDPPacketRemoveTypeAndID(incoming);

            if (id != 0)
            {
#ifdef CLIENT_DEBUG
                log_info("Received UDP-packet with wrong ID, throwing away...");
#endif
                UDPPacketDestroy(incoming);
                return 0;
            }

            ParsedPacket parsedPacket = ParsedPacketCreate(type, incoming->data, incoming->len, client.server);
            SDL_LockMutex(client.inBufferMutex);
            VectorPushBack(client.inBuffer, &parsedPacket);
            PacketPrintInformation(parsedPacket.type,
                                   0,
                                   parsedPacket.data,
                                   parsedPacket.size,
                                   *parsedPacket.sender.ip,
                                   "UDP", "INCOMING");
            SDL_UnlockMutex(client.inBufferMutex);
        }
    }
    UDPPacketDestroy(incoming);
    return result;
}

int ClientTryReceiveTCPPacket()
{

    char header[TCP_HEADER_SIZE] = {0};
    // First receive the header to know how big the packet is
    if (SDLNet_TCP_Recv(client.server.socket, header, TCP_HEADER_SIZE))
    {
        const size_t packetSize = SDL_atoi(header);
        if (packetSize < NET_TYPE_SIZE + NET_ID_SIZE)
        {
#ifdef CLIENT_DEBUG
            log_info("Received a TCP-packet that was too small, throwing away...");
#endif
            return 0;
        }

        // Then receive the full packet, piece by piece if required
        int result = 0;
        if (packetSize < TCP_MAX_SIZE)
        {
            result = ClientReceiveSmallTCPPacket(packetSize);
        }
        else
        {
            result = ClientReceiveBigTCPPacket(packetSize);
        }
        return result;
    }
    else
    {
#ifdef CLIENT_DEBUG
        log_warn("Failed to receive TCP-header from server (IP | PORT) (%d | %d)", client.server.ip->host, client.server.ip->port);
#endif
        return 0;
    }
}

int ClientReceiveSmallTCPPacket(size_t packetSize)
{
    char rest[packetSize];
    SDL_memset(rest, 0, packetSize);
    // Then receive the full packet
    if (SDLNet_TCP_Recv(client.server.socket, (void *)rest, packetSize))
    {
        // Parse data
        PacketType type = PacketDecodeType(rest);
        int id = PacketDecodeID(rest);
        int dataSize = TCPPacketRemoveTypeAndID(rest, packetSize);

        if (id != 0)
        {
#ifdef CLIENT_DEBUG
            log_info("Received TCP-packet with wrong ID, throwing away...");
#endif
            return 0;
        }

        ParsedPacket parsedPacket = ParsedPacketCreate(type, rest, dataSize, client.server);
        // Add to inBuffer.
        SDL_LockMutex(client.inBufferMutex);
        VectorPushBack(client.inBuffer, &parsedPacket);
        PacketPrintInformation(parsedPacket.type,
                               0,
                               parsedPacket.data,
                               parsedPacket.size,
                               *parsedPacket.sender.ip,
                               "TCP", "INCOMING");
        SDL_UnlockMutex(client.inBufferMutex);

        return 1;
    }
    else
    {
#ifdef CLIENT_DEBUG
        log_warn("Failed to receive TCP-packet from server (IP | PORT) (%d | %d)", client.server.ip->host, client.server.ip->port);
#endif
        return 0;
    }
}

int ClientReceiveBigTCPPacket(size_t packetSize)
{
    char *buffer = MALLOC_N(char, packetSize);
    SDL_memset(buffer, 0, packetSize);

    int totalSizeToReceive = (int)packetSize;
    int nFullPackagesRequired = (int)(totalSizeToReceive / TCP_MAX_SIZE);
    int bytesLeft = totalSizeToReceive % TCP_MAX_SIZE;

    for (int i = 0; i < nFullPackagesRequired; i++)
    {
        if (!SDLNet_TCP_Recv(client.server.socket, buffer + i * TCP_MAX_SIZE, TCP_MAX_SIZE))
        {
#ifdef CLIENT_DEBUG
            log_warn("Failed to receive TCP-packet from server (IP | PORT) (%d | %d)", player.ip->host, player.ip->port);
#endif
            FREE(buffer);
            return 0;
        }
    }

    if (!SDLNet_TCP_Recv(client.server.socket, buffer + nFullPackagesRequired * TCP_MAX_SIZE, bytesLeft))
    {
#ifdef CLIENT_DEBUG
        log_warn("Failed to receive TCP-packet from server (IP | PORT) (%d | %d)", player.ip->host, player.ip->port);
#endif
        FREE(buffer);
        return 0;
    }

    // Parse data
    PacketType type = PacketDecodeType(buffer);
    int id = PacketDecodeID(buffer);
    int dataLength = TCPPacketRemoveTypeAndID(buffer, packetSize);
    if (id < 0)
    {
#ifdef CLIENT_DEBUG
        log_info("Received a TCP-packet with invalid ID, throwing packet...");
#endif
        FREE(buffer);
        return 0;
    }

    ParsedPacket parsedPacket = ParsedPacketCreate(type, buffer, dataLength, client.server);
    // Add to inBuffer.
    SDL_LockMutex(client.inBufferMutex);
    VectorPushBack(client.inBuffer, &parsedPacket);
    PacketPrintInformation(parsedPacket.type,
                           parsedPacket.sender.id,
                           parsedPacket.data,
                           parsedPacket.size,
                           *parsedPacket.sender.ip,
                           "TCP", "INCOMING");
    SDL_UnlockMutex(client.inBufferMutex);

    FREE(buffer);
    return 1;
}

ParsedPacket *ClientGetInBufferArray()
{
    return (ParsedPacket *)client.inBuffer->data;
}

size_t ClientGetInBufferArraySize()
{
    return client.inBuffer->size;
}

Vector *ClientGetInBufferVector()
{
    return client.inBuffer;
}

void ClientPlayerIsInitialized()
{
    client.receivedPlayerID = SDL_TRUE;
}