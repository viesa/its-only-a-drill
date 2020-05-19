#include "Client.h"

typedef struct Client
{
    NetPlayer server;
    UDPsocket udpSocket;
    SDLNet_SocketSet socketSet;

    Vector *inBuffer;
    SDL_mutex *inBufferMutex;

    SDL_bool isListening;
    SDL_bool isInitialized;
    SDL_bool receivedPlayerID;

    SDL_Thread *listenThread;

    float connectTimer;
    SDL_mutex *connectMutex;

    char name[MAX_PLAYERNAME_SIZE];
    SDL_bool hasSentName;
} Client;

static Client *client;

void ClientInitialize()
{
    client = MALLOC(Client);
    ALLOC_ERROR_CHECK(client);
    client->server = NetPlayerCreate(NULL, 0);
    client->udpSocket = NULL;
    client->socketSet = SDLNet_AllocSocketSet(2);
    client->isListening = SDL_FALSE;
    client->isInitialized = SDL_FALSE;
    client->receivedPlayerID = SDL_FALSE;
    client->inBuffer = VectorCreate(sizeof(ParsedPacket), 100);
    client->inBufferMutex = SDL_CreateMutex();
    client->connectTimer = 0.0f;
    client->connectMutex = SDL_CreateMutex();

    ClientClearName();
    client->hasSentName = SDL_FALSE;

    // Start with assuming we are offline
    ConStateSet(CON_Offline);

    client->isInitialized = SDL_TRUE;

    ClientTryConnect();
}

void ClientUninitialize()
{
    ClientDisconnect();
    VectorDestroy(client->inBuffer);
    SDL_DestroyMutex(client->inBufferMutex);
    SDLNet_FreeSocketSet(client->socketSet);
    SDL_DestroyMutex(client->connectMutex);
    client->isInitialized = SDL_FALSE;
    client->receivedPlayerID = SDL_FALSE;
    PlayerGetEntity()->id = -1;
    FREE(client);
}

void ClientUpdate()
{
    if (ConStateGet() == CON_Offline)
    {
        const int wait = 5.0f;
        if (client->connectTimer >= wait)
        {
            ClientTryConnect();
            client->connectTimer = 0.0f;
        }
        else
        {
            client->connectTimer += ClockGetDeltaTime();
        }
    }
    ClientUpdateServerTimeoutTimer();
    ClientPingServer();
}

void ClientUpdateServerTimeoutTimer()
{
    if (ConStateGet() == CON_Online && ClientIsWaitingForAliveReply())
    {
        client->server.timeoutTimer += ClockGetDeltaTime();
    }
}

void ClientPingServer()
{
    if (ConStateGet() == CON_Online && !ClientIsWaitingForAliveReply())
    {
        ClientTCPSend(PT_AreYouAlive, NULL, 0);
        client->server.waitingForAliveReply = SDL_TRUE;
    }
}

void ClientTryConnect()
{
    assert("Attempting to connect client without client initialization" && client->isInitialized);
    SDL_Thread *worker = SDL_CreateThread((SDL_ThreadFunction)ClientConnectThreadFn, "Server Connector", NULL);
    SDL_DetachThread(worker);
}

void ClientConnectThreadFn()
{
    int status = SDL_TryLockMutex(client->connectMutex);
    if (status == SDL_MUTEX_TIMEDOUT)
    {
        // Another thread is already trying to connect
        return;
    }
    else if (status != 0)
    {
#ifdef CLIENT_DEBUG
        log_error("Failed to lock connection mutex");
#endif
        return;
    }

    if (ConStateGet() == CON_Online)
    {
        SDL_UnlockMutex(client->connectMutex);
        return;
    }

#ifdef LOCAL_SERVER
    const char *ip = "127.0.0.1";
    Uint16 port = 1337;
#else
    const char *ip = "drill.pierrelf.com"; //drill.pierrelf.com port 1337
    Uint16 port = 1337;
#endif

    IPaddress serverIP;
    // Resolve host to retrieve SDL IPaddress pointing to server
    if (SDLNet_ResolveHost(&serverIP, ip, port) == -1)
    {
#ifdef CLIENT_DEBUG
        log_error("Failed to resolve host: (%s:%d): %s", ip, port, SDLNet_GetError());
#endif
        SDL_UnlockMutex(client->connectMutex);
        Notify("Failed to connect", 2.0f, NT_WARN);
        return;
    }

    // Open UDP-socket on random port
    if (!(client->udpSocket = SDLNet_UDP_Open(0)))
    {
#ifdef CLIENT_DEBUG
        log_error("Failed to open port (UDP) (%s:%d)): %s", ip, port, SDLNet_GetError());
#endif
        SDL_UnlockMutex(client->connectMutex);
        Notify("Failed to connect", 2.0f, NT_WARN);
        return;
    }
    // Open TCP-socket
    TCPsocket tcpSocket;
    if (!(tcpSocket = SDLNet_TCP_Open(&serverIP)))
    {
#ifdef CLIENT_DEBUG
        log_error("Failed to open port (TCP) (%s:%d)): %s", ip, port, SDLNet_GetError());
#endif
        // Clean up
        SDLNet_UDP_Close(client->udpSocket);
        SDL_UnlockMutex(client->connectMutex);
        Notify("Failed to connect", 2.0f, NT_WARN);
        return;
    }
    client->server = NetPlayerCreate(tcpSocket, 0);

    // Add TCP-socket to socket set
    if (SDLNet_UDP_AddSocket(client->socketSet, client->udpSocket) == -1)
    {
#ifdef CLIENT_DEBUG
        log_error("Failed to add socket to socket set (UDP): %s", SDLNet_GetError());
#endif
        // Clean up
        SDLNet_TCP_Close(tcpSocket);
        SDLNet_UDP_Close(client->udpSocket);
        SDL_UnlockMutex(client->connectMutex);
        Notify("Failed to connect", 2.0f, NT_WARN);
        return;
    }
    // Add TCP-socket to socket set
    if (SDLNet_TCP_AddSocket(client->socketSet, client->server.socket) == -1)
    {
#ifdef CLIENT_DEBUG
        log_error("Failed to add socket to socket set (TCP): %s", SDLNet_GetError());
#endif
        // Clean up
        SDLNet_TCP_Close(tcpSocket);
        SDLNet_UDP_Close(client->udpSocket);
        SDLNet_UDP_DelSocket(client->socketSet, client->udpSocket);
        SDL_UnlockMutex(client->connectMutex);
        Notify("Failed to connect", 2.0f, NT_WARN);
        return;
    }

    if (strlen(client->name) != 0)
    {
        ClientTCPSend(PT_Connect, client->name, strlen(client->name));
    }

    ClientStartListening();

    ConStateSet(CON_Online);
    Notify("Connection established", 2.0f, NT_INFO);

    SDL_UnlockMutex(client->connectMutex);
}

int ClientDisconnect()
{
    assert("Attempting to disconnect client without client initialization" && client->isInitialized);

    if (client->isListening)
    {
        ClientStopListening();
    }

    ClientTCPSend(PT_Disconnect, NULL, 0);
    if (client->udpSocket)
    {
        SDLNet_UDP_DelSocket(client->socketSet, client->udpSocket);
        SDLNet_UDP_Close(client->udpSocket);
    }
    if (client->server.socket)
    {
        SDLNet_TCP_DelSocket(client->socketSet, client->server.socket);
        SDLNet_TCP_Close(client->server.socket);
    }

    ClientClearInBuffer();
    SDL_UnlockMutex(client->inBufferMutex);

    ConStateSet(CON_Offline);

    return 1;
}

void ClientStartListening()
{
    assert("Attempting to start client without client initialization" && client->isInitialized);

    client->isListening = SDL_TRUE;
    client->listenThread = SDL_CreateThread((SDL_ThreadFunction)ClientListenToServer, "Server Listen Thread", NULL);
}

void ClientStopListening()
{
    client->isListening = SDL_FALSE;
    SDL_WaitThread(client->listenThread, NULL);
}

int ClientUDPSend(PacketType type, void *data, size_t size)
{
    if (!client->server.socket)
        return 0;

    assert("Attempting to send UDP-packet without client initialization" && client->isInitialized);

    UDPpacket *outgoing = UDPPacketCreate(type, PlayerGetEntity()->id, data, size);
    outgoing->address.host = client->server.ip->host;
    outgoing->address.port = client->server.ip->port;

    int result = ClientUDPOut(outgoing);

    UDPPacketDestroy(outgoing);

    return result;
}

int ClientUDPOut(UDPpacket *packet)
{
    if (!SDLNet_UDP_Send(client->udpSocket, -1, packet))
    {
#ifdef CLIENT_DEBUG
        log_warn("Failed to send UDP-packet to server: %s", SDLNet_GetError());
#endif
        return 0;
    }
#ifdef CLIENT_IO
    PacketPrintInformation(PacketDecodeType(packet->data),
                           PacketDecodeID(packet->data),
                           packet->data,
                           packet->len,
                           packet->address,
                           "UDP", "OUTGOING");
#endif
    return 1;
}

int ClientTCPSend(PacketType type, void *data, size_t size)
{
    if (!client->server.socket)
    {
#ifdef CLIENT_DEBUG
        log_warn("Attempted to send a package to server without being connected to it");
#endif
        return 0;
    }
    assert("Attempting to send TCP-packet without client initialization" && client->isInitialized);

    int id = PlayerGetEntity()->id;
    TCPpacket *outgoing = TCPPacketCreate(type, id, data, size);
    outgoing->address = client->server.socket;

    int result = ClientTCPOut(outgoing);

    TCPPacketDestroy(outgoing);

    return result;
}

int ClientTCPOut(TCPpacket *packet)
{
    assert("Attempting to send packet without client initialization" && client->isInitialized);

    int totalSizeToSend = (int)packet->len;
    int totalSent = 0;

    while (totalSent < totalSizeToSend)
    {
        // Attempts to send as much as possible
        int newSend = SDLNet_TCP_Send(packet->address, packet->data + totalSent, totalSizeToSend - totalSent);
        // If nothing was sent, or negative, connection was dissrupted
        if (newSend <= 0)
        {
#ifdef CLIENT_DEBUG
            log_warn("Failed to send TCP-packet to server: %s", SDLNet_GetError());
#endif
            return 0;
        }
        totalSent += newSend;
    }

#ifdef CLIENT_IO
    PacketPrintInformation(PacketDecodeType(((char *)packet->data) + TCP_HEADER_SIZE),
                           PacketDecodeID(((char *)packet->data) + TCP_HEADER_SIZE),
                           packet->data,
                           packet->len,
                           *SDLNet_TCP_GetPeerAddress(packet->address),
                           "TCP", "OUTGOING");
#endif
    return 1;
}

void ClientListenToServer()
{
    assert("Attempting to start client listener without client initialization" && client->isInitialized);

    SDL_SetThreadPriority(SDL_THREAD_PRIORITY_LOW);
    while (client->isListening)
    {
        int nReadySockets = SDLNet_CheckSockets(client->socketSet, 20);
        if (!nReadySockets)
        {
            // If no sockets are ready, swap thread
            SDL_Delay(0);
        }
        else if (nReadySockets)
        {
            // Check if the socket ready was TCP-socket
            if (SDLNet_SocketReady(client->server.socket))
            {
                ClientTryReceiveTCPPacket();
                if (--nReadySockets == 0)
                    continue;
            }
            // Check if the socket ready was UDP-socket
            if (SDLNet_SocketReady(client->udpSocket))
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
    incoming->address.host = client->server.ip->host;
    incoming->address.port = client->server.ip->port;

    int result = SDLNet_UDP_Recv(client->udpSocket, incoming);
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

            ParsedPacket parsedPacket = ParsedPacketCreate(type, incoming->data, incoming->len, client->server);
            SDL_LockMutex(client->inBufferMutex);
            VectorPushBack(client->inBuffer, &parsedPacket);
#ifdef CLIENT_IO
            PacketPrintInformation(parsedPacket.type,
                                   0,
                                   parsedPacket.data,
                                   parsedPacket.size,
                                   *parsedPacket.sender.ip,
                                   "UDP", "INCOMING");
#endif
            SDL_UnlockMutex(client->inBufferMutex);
        }
    }
    UDPPacketDestroy(incoming);
    return result;
}

int ClientTryReceiveTCPPacket()
{
    char header[TCP_HEADER_SIZE] = {0};
    // First receive the header to know how big the packet is
    int headerReceived = 0;
    while (headerReceived < TCP_HEADER_SIZE)
    {
        // Attempts to receive as much as possible
        int newReceive = SDLNet_TCP_Recv(client->server.socket, header + headerReceived, TCP_HEADER_SIZE - headerReceived);
        // If nothing was received, or negative, connection was dissrupted
        if (newReceive <= 0)
        {
#ifdef CLIENT_DEBUG
            log_warn("Failed to receive TCP-packet from server (IP | PORT) (%d | %d)", client->server.ip->host, client->server.ip->port);
#endif
            return 0;
        }
        headerReceived += newReceive;
    }
    const size_t packetSize = SDL_atoi(header);
    if (packetSize < NET_TYPE_SIZE + NET_ID_SIZE)
    {
#ifdef CLIENT_DEBUG
        log_info("Received a TCP-packet that was too small, throwing away...");
#endif
    }

    // Then receive the full packet, piece by piece if required
    char *buffer = MALLOC_N(char, packetSize);
    SDL_memset(buffer, 0, packetSize);

    int totalSizeToReceive = (int)packetSize;
    int totalReceived = 0;

    // Receive bytes until you received whole packet, or until the connection is dissrupted
    while (totalReceived < totalSizeToReceive)
    {
        // Attempts to receive as much as possible
        int newReceive = SDLNet_TCP_Recv(client->server.socket, buffer + totalReceived, totalSizeToReceive - totalReceived);
        // If nothing was received, or negative, connection was dissrupted
        if (newReceive <= 0)
        {
#ifdef CLIENT_DEBUG
            log_warn("Failed to receive TCP-packet from server (IP | PORT) (%d | %d)", client->server.ip->host, client->server.ip->port);
#endif
            FREE(buffer);
            return 0;
        }
        totalReceived += newReceive;
    }

    // Parse data
    PacketType type = PacketDecodeType(buffer);
    int id = PacketDecodeID(buffer);
    int dataLength = TCPPacketRemoveTypeAndID(buffer, packetSize);
    if (id != 0)
    {
#ifdef SERVER_DEBUG
        log_info("Received a TCP-packet with invalid ID, throwing packet...");
#endif
        FREE(buffer);
        return 0;
    }

    ParsedPacket parsedPacket = ParsedPacketCreate(type, buffer, dataLength, client->server);
    // Add to inBuffer.
    SDL_LockMutex(client->inBufferMutex);
    VectorPushBack(client->inBuffer, &parsedPacket);
#ifdef CLIENT_IO
    PacketPrintInformation(parsedPacket.type,
                           parsedPacket.sender.id,
                           parsedPacket.data,
                           parsedPacket.size,
                           *parsedPacket.sender.ip,
                           "TCP", "INCOMING");
#endif
    SDL_UnlockMutex(client->inBufferMutex);

    FREE(buffer);
    return 1;
}

ParsedPacket *ClientGetInBuffer()
{
    return (ParsedPacket *)client->inBuffer->data;
}

void ClientClearInBuffer()
{
    for (size_t i = 0; i < client->inBuffer->size; i++)
    {
        ParsedPacketDestroy(&CLIENT_INBUFFER[i]);
    }
    VectorClear(client->inBuffer);
}

void ClientReceivedAlivePacket()
{
    client->server.timeoutTimer = 0.0f;
    client->server.waitingForAliveReply = SDL_FALSE;
}

void ClientSetPlayerID(int id)
{
    PlayerGetEntity()->id = id;
    client->receivedPlayerID = SDL_TRUE;
}

void ClientSetPlayerEntity(Entity *entity)
{
    // Shallow copy the entity to the player
    *PlayerGetEntity() = *entity;
}

char *ClientGetName()
{
    return client->name;
}

void ClientSetName(char *name)
{
    ClientClearName();
    int toCopy = strlen(name) < MAX_PLAYERNAME_SIZE ? strlen(name) : MAX_PLAYERNAME_SIZE;
    SDL_memcpy(client->name, name, toCopy);
}

void ClientClearName()
{
    SDL_memset(client->name, 0, MAX_PLAYERNAME_SIZE);
}

size_t ClientGetInBufferArraySize()
{
    return client->inBuffer->size;
}

Vector *ClientGetInBufferVector()
{
    return client->inBuffer;
}

size_t ClientGetInBufferSize()
{
    return client->inBuffer->size;
}

SDL_mutex *ClientGetInBufferMutex()
{
    return client->inBufferMutex;
}

SDL_bool ClientIsWaitingForAliveReply()
{
    return client->server.waitingForAliveReply;
}

float ClientGetTimeoutTimer()
{
    return client->server.timeoutTimer;
}