#include "Server.h"

typedef struct Server
{
    UDPsocket udpSocket;
    TCPsocket tcpSocket;
    SDLNet_SocketSet socketSet;
    Vector *players;
    Vector *ids; //0 means taken
    int currentIDIndex;

    Vector *sessions;
    Vector *sessionBitmap;

    Vector *inBuffer;
    SDL_mutex *inBufferMutex;

    SDL_bool isListening;
    SDL_bool isInitialized;

    SDL_Thread *listenThread;

    SDL_mutex *trafficMutex;
} Server;

static Server *server;

void ServerInitialize()
{
    server = MALLOC(Server);
    ALLOC_ERROR_CHECK(server);
    server->socketSet = SDLNet_AllocSocketSet(100);
    server->players = VectorCreate(sizeof(NetPlayer), 100);
    server->players->resizable = SDL_FALSE;
    server->ids = VectorCreate(sizeof(SDL_bool), 100);
    server->ids->resizable = SDL_FALSE;
    for (size_t i = 0; i < server->ids->capacity; i++)
        SERVER_IDS[i] = SDL_FALSE;
    server->currentIDIndex = 0;
    server->inBuffer = VectorCreate(sizeof(ParsedPacket), 100);
    server->inBufferMutex = SDL_CreateMutex();
    server->isInitialized = SDL_FALSE;
    server->isListening = SDL_FALSE;
    server->sessions = VectorCreate(sizeof(Session), 5);
    server->sessionBitmap = VectorCreate(sizeof(SDL_bool), 100);
    for (size_t i = 0; i < server->sessionBitmap->capacity; i++)
    {
        SERVER_SESSIONBITMAP[i] = SDL_FALSE;
    }

    const Uint16 port = 1337;

    IPaddress own;
    if (SDLNet_ResolveHost(&own, NULL, port) == -1)
    {
        log_error("Could not resolve host: %s", SDLNet_GetError());
        return;
    }

    // Open UDP-socket
    if (!(server->udpSocket = SDLNet_UDP_Open(port)))
    {
        log_error("Failed to open port (UDP): %s", SDLNet_GetError());
        return;
    }
    // Add UDP-socket to socket set
    if (SDLNet_UDP_AddSocket(server->socketSet, server->udpSocket) == -1)
    {
        log_error("Failed to add socket to socket set (UDP): %s", SDLNet_GetError());
        return;
    }

    // Open TCP-socket
    if (!(server->tcpSocket = SDLNet_TCP_Open(&own)))
    {
        log_error("Failed to open port (TCP): %s", SDLNet_GetError());
        return;
    }
    // Add TCP-socket to socket set
    if (SDLNet_TCP_AddSocket(server->socketSet, server->tcpSocket) == -1)
    {
        log_error("Failed to add socket to socket set (TCP): %s", SDLNet_GetError());
        return;
    }

    server->isInitialized = SDL_TRUE;
}

void ServerUninitialize()
{
    ServerTCPBroadcast(PT_Disconnect, NULL, 0);
    SDLNet_UDP_DelSocket(server->socketSet, server->udpSocket);
    SDLNet_UDP_Unbind(server->udpSocket, 0);
    SDLNet_UDP_Close(server->udpSocket);
    SDLNet_TCP_DelSocket(server->socketSet, server->tcpSocket);
    SDLNet_TCP_Close(server->tcpSocket);
    for (size_t i = 0; i < server->players->size; i++)
    {
        SDLNet_TCP_DelSocket(server->socketSet, SERVER_PLAYERS[i].socket);
        SDLNet_TCP_Close(SERVER_PLAYERS[i].socket);
    }
    SDLNet_FreeSocketSet(server->socketSet);

    for (size_t i = 0; i < server->inBuffer->size; i++)
    {
        ParsedPacketDestroy(&SERVER_INBUFFER[i]);
    }
    VectorDestroy(server->players);
    VectorDestroy(server->ids);
    VectorDestroy(server->inBuffer);

    for (size_t i = 0; i < server->sessions->size; i++)
    {
        SessionDestroy(&SERVER_SESSIONS[i]);
    }
    VectorDestroy(server->sessions);
    VectorDestroy(server->sessionBitmap);

    SDL_UnlockMutex(server->inBufferMutex);
    SDL_DestroyMutex(server->inBufferMutex);
    server->isInitialized = SDL_FALSE;

    FREE(server);
}

void ServerStartListening()
{
    assert("Attempting to start server without server initialization" && server->isInitialized);

    server->isListening = SDL_TRUE;
    server->listenThread = SDL_CreateThread((SDL_ThreadFunction)ServerListenToClients, "Server Listen Thread", NULL);
}

void ServerStopListening()
{
    assert("Attempting to stop server without server initialization" && server->isInitialized);

    server->isListening = SDL_FALSE;
    SDL_WaitThread(server->listenThread, NULL);
}

void ServerUpdateTimeoutTimers()
{
    for (int i = 0; i < ServerGetNumPlayers(); i++)
    {
        if (SERVER_PLAYERS[i].waitingForAliveReply)
            SERVER_PLAYERS[i].timeoutTimer += ClockGetDeltaTime();
    }
}

void ServerPingClients()
{
    for (int i = 0; i < ServerGetNumPlayers(); i++)
    {
        if (!SERVER_PLAYERS[i].waitingForAliveReply)
        {
            ServerTCPSend(PT_AreYouAlive, NULL, 0, SERVER_PLAYERS[i]);
            SERVER_PLAYERS[i].waitingForAliveReply = SDL_TRUE;
        }
    }
}

void ServerRemoveSession(Session *session)
{
    while (session->playerIDs->size > 0)
    {
        // This function also remove session completely when last player is removed
        ServerRemovePlayerFromSession(session, SessionGetPlayerIDs(session)[0]);
    }
}

void ServerUDPBroadcast(PacketType type, void *data, int size)
{
    // Creates a UDP-packet to send. ID of server is 0
    UDPpacket *packet = UDPPacketCreate(type, 0, data, size);
    for (int i = 0; i < server->players->size; i++)
    {
        packet->address = SERVER_PLAYERS[i].udpRespondIP;
        ServerUDPOut(packet);
    }
    UDPPacketDestroy(packet);
}
void ServerUDPBroadcastSession(PacketType type, Session *session, void *data, int size)
{
    // Creates a UDP-packet to send. ID of server is 0
    UDPpacket *packet = UDPPacketCreate(type, 0, data, size);
    int *playerIDs = SessionGetPlayerIDs(session);
    for (int i = 0; i < session->playerIDs->size; i++)
    {
        NetPlayer *to = ServerGetPlayerByID(playerIDs[i]);

        packet->address = to->udpRespondIP;
        ServerUDPOut(packet);
    }
    UDPPacketDestroy(packet);
}
void ServerUDPBroadcastExclusive(PacketType type, void *data, int size, NetPlayer exclusive)
{
    // Creates a UDP-packet to send. ID of server is 0
    UDPpacket *packet = UDPPacketCreate(type, 0, data, size);
    for (int i = 0; i < server->players->size; i++)
    {
        if (SERVER_PLAYERS[i].id == exclusive.id)
            continue;

        packet->address = SERVER_PLAYERS[i].udpRespondIP;
        ServerUDPOut(packet);
    }
    UDPPacketDestroy(packet);
}
void ServerUDPBroadcastExclusiveSession(PacketType type, Session *session, void *data, int size, NetPlayer exclusive)
{
    // Creates a UDP-packet to send. ID of server is 0
    UDPpacket *packet = UDPPacketCreate(type, 0, data, size);
    int *playerIDs = SessionGetPlayerIDs(session);
    for (int i = 0; i < session->playerIDs->size; i++)
    {
        NetPlayer *to = ServerGetPlayerByID(playerIDs[i]);

        if (to->id == exclusive.id)
            continue;

        packet->address = to->udpRespondIP;
        ServerUDPOut(packet);
    }
    UDPPacketDestroy(packet);
}
void ServerUDPSend(PacketType type, void *data, int size, NetPlayer player)
{
    // Creates a UDP-packet to send. ID of server is 0
    UDPpacket *packet = UDPPacketCreate(type, 0, data, size);
    packet->address = player.udpRespondIP;
    ServerUDPOut(packet);
    UDPPacketDestroy(packet);
}

void ServerUDPOut(UDPpacket *packet)
{
    assert("Attempting to send packet without server initialization" && server->isInitialized);

    if (packet->address.host == 0 || packet->address.port == 0)
        return;

    if (!SDLNet_UDP_Send(server->udpSocket, -1, packet))
    {
#ifdef SERVER_DEBUG
        log_warn("Failed to send UDP-packet to client: %s", SDLNet_GetError());
#endif
    }
    if (CLIStateGet() == CS_Traffic)
    {
        SDL_LockMutex(server->trafficMutex);
        PacketPrintInformation(PacketDecodeType(packet->data),
                               PacketDecodeID(packet->data),
                               packet->data,
                               packet->len,
                               packet->address,
                               "UDP", "OUTGOING");
        SDL_UnlockMutex(server->trafficMutex);
    }
}

void ServerTCPBroadcast(PacketType type, void *data, int size)
{
    // Creates a TCP-packet to send. ID of server is 0
    TCPpacket *packet = TCPPacketCreate(type, 0, data, size);
    for (int i = 0; i < server->players->size; i++)
    {
        packet->address = SERVER_PLAYERS[i].socket;
        ServerTCPOut(packet);
    }
    TCPPacketDestroy(packet);
}

void ServerTCPBroadcastSession(PacketType type, Session *session, void *data, int size)
{

    // Creates a TCP-packet to send. ID of server is 0
    TCPpacket *packet = TCPPacketCreate(type, 0, data, size);
    int *playerIDs = SessionGetPlayerIDs(session);
    for (int i = 0; i < session->playerIDs->size; i++)
    {
        NetPlayer *to = ServerGetPlayerByID(playerIDs[i]);

        packet->address = to->socket;
        ServerTCPOut(packet);
    }
    TCPPacketDestroy(packet);
}

void ServerTCPBroadcastExclusive(PacketType type, void *data, int size, NetPlayer exclusive)
{
    // Creates a TCP-packet to send. ID of server is 0
    TCPpacket *packet = TCPPacketCreate(type, 0, data, size);
    for (int i = 0; i < server->players->size; i++)
    {
        if (SERVER_PLAYERS[i].id == exclusive.id)
            continue;

        packet->address = SERVER_PLAYERS[i].socket;
        ServerTCPOut(packet);
    }
    TCPPacketDestroy(packet);
}
void ServerTCPBroadcastExclusiveSession(PacketType type, Session *session, void *data, int size, NetPlayer exclusive)
{
    // Creates a TCP-packet to send. ID of server is 0
    TCPpacket *packet = TCPPacketCreate(type, 0, data, size);
    int *playerIDs = SessionGetPlayerIDs(session);
    for (int i = 0; i < session->playerIDs->size; i++)
    {
        NetPlayer *to = ServerGetPlayerByID(playerIDs[i]);

        if (to->id == exclusive.id)
            continue;

        packet->address = to->socket;
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
    assert("Attempting to send packet without server initialization" && server->isInitialized);

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
            log_warn("Failed to send TCP-packet to client: %s", SDLNet_GetError());
#endif
            return;
        }
        totalSent += newSend;
    }

    if (CLIStateGet() == CS_Traffic)
    {
        SDL_LockMutex(server->trafficMutex);
        PacketPrintInformation(PacketDecodeType(((char *)packet->data) + TCP_HEADER_SIZE),
                               PacketDecodeID(((char *)packet->data) + TCP_HEADER_SIZE),
                               packet->data,
                               packet->len,
                               *SDLNet_TCP_GetPeerAddress(packet->address),
                               "TCP", "OUTGOING");
        SDL_UnlockMutex(server->trafficMutex);
    }
}

void ServerListenToClients()
{
    assert("Attempting to start listener thread on server without server initialization" && server->isInitialized);

    SDL_SetThreadPriority(SDL_THREAD_PRIORITY_LOW);
    while (server->isListening)
    {
        int nReadySockets = SDLNet_CheckSockets(server->socketSet, 20);
        if (!nReadySockets)
        {
            // If no sockets are ready, swap thread
            SDL_Delay(0);
        }
        else if (nReadySockets)
        {
            // Check if the socket ready was TCP-socket -> New connection incoming
            if (SDLNet_SocketReady(server->tcpSocket))
            {
                ServerTryAcceptTCPSocket(server);
                if (--nReadySockets == 0)
                    continue;
            }
            // Check if the socket ready was UDP-socket
            if (SDLNet_SocketReady(server->udpSocket))
            {
                ServerTryReceiveUDPPacket();
                if (--nReadySockets == 0)
                    continue;
            }
            // Check if the socket ready was a client-socket
            for (size_t i = 0; i < server->players->size; i++)
            {
                if (SDLNet_SocketReady(SERVER_PLAYERS[i].socket))
                {
                    ServerTryReceiveTCPPacket(SERVER_PLAYERS[i]);
                    if (--nReadySockets == 0)
                        break;
                }
            }
        }
        else if (nReadySockets < 0)
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

    int result = SDLNet_UDP_Recv(server->udpSocket, incoming);

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

            // Asserts that the senders IP-address has a TCP-connection as well, and that those IDs match
            NetPlayer *sender = NULL;
            for (int i = 0; i < server->players->size; i++)
            {
                if (SERVER_PLAYERS[i].ip->host == incoming->address.host &&
                    SERVER_PLAYERS[i].id == id)
                {
                    sender = &SERVER_PLAYERS[i];
                }
            }
            if (sender != NULL)
            {
                // Update the respond address
                sender->udpRespondIP = incoming->address;

                ParsedPacket parsedPacket = ParsedPacketCreate(type, incoming->data, incoming->len, *sender);
                SDL_LockMutex(server->inBufferMutex);
                VectorPushBack(server->inBuffer, &parsedPacket);

                if (CLIStateGet() == CS_Traffic)
                {
                    SDL_LockMutex(server->trafficMutex);
                    PacketPrintInformation(parsedPacket.type,
                                           parsedPacket.sender.id,
                                           parsedPacket.data,
                                           parsedPacket.size,
                                           *parsedPacket.sender.ip,
                                           "UDP", "INCOMING");
                    SDL_UnlockMutex(server->trafficMutex);
                }
                SDL_UnlockMutex(server->inBufferMutex);
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
    if (server->players->size >= server->players->capacity - 1)
    {
        return 0;
    }

    TCPsocket newClient = SDLNet_TCP_Accept(server->tcpSocket);
    if (newClient)
    {
        SDLNet_TCP_AddSocket(server->socketSet, newClient);
        NetPlayer newPlayer = NetPlayerCreate(newClient, ServerGenID());
        newPlayer.entity = EntityCreate(Vec2Create(0.0f, 0.0f), ET_Player, newPlayer.id);
        // Default skin is prisoner
        for (int i = 0; i < MAX_DRAWABLES; i++)
        {
            newPlayer.entity.drawables[i].spriteSheet = SS_Character_Prisoner;
            newPlayer.entity.drawables[i].dst.w = (int)(33.0f * 1.5f);
            newPlayer.entity.drawables[i].dst.h = (int)(33.0f * 1.5f);
        }
        ServerTCPSend(PT_Connect, &newPlayer.id, sizeof(int), newPlayer);
        VectorPushBack(server->players, &newPlayer);
#ifdef SERVER_DEBUG
        log_info("New Player connection (id:%d) (IP | PORT) (%d | %p)", newPlayer.id, newPlayer.ip->host, newPlayer.ip->port);
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
    int headerReceived = 0;
    while (headerReceived < TCP_HEADER_SIZE)
    {
        // Attempts to receive as much as possible
        int newReceive = SDLNet_TCP_Recv(player.socket, header + headerReceived, TCP_HEADER_SIZE - headerReceived);
        // If nothing was received, or negative, connection was dissrupted
        if (newReceive <= 0)
        {
#ifdef SERVER_DEBUG
            log_warn("Failed to receive TCP-packet from client (IP | PORT) (%d | %d)", player.ip->host, player.ip->port);
#endif
            return 0;
        }
        headerReceived += newReceive;
    }
    const size_t packetSize = SDL_atoi(header);
    if (packetSize < NET_TYPE_SIZE + NET_ID_SIZE)
    {
#ifdef SERVER_DEBUG
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
        int newReceive = SDLNet_TCP_Recv(player.socket, buffer + totalReceived, totalSizeToReceive - totalReceived);
        // If nothing was received, or negative, connection was dissrupted
        if (newReceive <= 0)
        {
#ifdef SERVER_DEBUG
            log_warn("Failed to receive TCP-packet from client (IP | PORT) (%d | %d)", player.ip->host, player.ip->port);
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
    if (id < 0)
    {
#ifdef SERVER_DEBUG
        log_info("Received a TCP-packet with invalid ID, throwing packet...");
#endif
        FREE(buffer);
        return 0;
    }

    ParsedPacket parsedPacket = ParsedPacketCreate(type, buffer, dataLength, player);
    // Add to inBuffer.
    SDL_LockMutex(server->inBufferMutex);
    VectorPushBack(server->inBuffer, &parsedPacket);

    if (CLIStateGet() == CS_Traffic)
    {
        SDL_LockMutex(server->trafficMutex);
        PacketPrintInformation(parsedPacket.type,
                               parsedPacket.sender.id,
                               parsedPacket.data,
                               parsedPacket.size,
                               *parsedPacket.sender.ip,
                               "TCP", "INCOMING");
        SDL_UnlockMutex(server->trafficMutex);
    }
    SDL_UnlockMutex(server->inBufferMutex);

    FREE(buffer);
    return 1;
}

void ServerRemoveClient(NetPlayer player)
{
    // Try to disconnect and clean up to make sure client is not still in memory
    // Even if no correspsonding ID was found
    if (player.socket)
    {
        // Try send disconnect message to client in case it was just a bad connection
        // So that the client can quickly connect again later
        ServerTCPSend(PT_Disconnect, NULL, 0, player);
        // Clean up socket related data to not receive anything more from this client
        SDLNet_TCP_DelSocket(server->socketSet, player.socket);
        SDLNet_TCP_Close(player.socket);
    }

    NetPlayer *senderP = ServerGetPlayerByID(player.id);

    // If NetPlayer was not found in current players,
    // it was already deleted -> Return
    if (!senderP)
    {
        return;
    }

    // Try to remove player from any session
    for (int i = 0; i < server->sessions->size; i++)
    {
        ServerRemovePlayerFromSession(&SERVER_SESSIONS[i], senderP->id);
    }

    // Remove client from player-list
    VectorEraseElement(server->players, senderP);

    // Free up the used ID so it can be assigned to someone else
    ServerFreeID(player.id);
}

int ServerGenID()
{
    for (int i = 0; i != server->players->capacity; i++)
    {
        int finalIndex = server->currentIDIndex + i;
        if (finalIndex == server->players->capacity)
            finalIndex -= server->players->capacity;

        if (!SERVER_IDS[finalIndex])
        {
            SERVER_IDS[finalIndex] = SDL_TRUE;
            server->currentIDIndex = finalIndex + 1;
            if (server->currentIDIndex == server->players->capacity)
                server->currentIDIndex = 0;
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
    if (id > 0 || id <= server->players->capacity)
        SERVER_IDS[id - 1] = SDL_FALSE;
}

int ServerGetSessionID()
{
    for (size_t i = 0; i < server->sessionBitmap->capacity; i++)
    {
        if (!SERVER_SESSIONBITMAP[i])
        {
            SERVER_SESSIONBITMAP[i] = SDL_TRUE;
            return i;
        }
    }
#ifdef SERVER_DEBUG
    log_warn("No available session IDs");
#endif
    return -1;
}

void ServerFreeSessionID(int id)
{
    if (id >= 0 || id < server->sessionBitmap->capacity)
        SERVER_SESSIONBITMAP[id] = SDL_FALSE;
}

Session *ServerGetSessionByID(int sessionID)
{
    for (size_t i = 0; i < server->sessions->size; i++)
    {
        if (SERVER_SESSIONS[i].id == sessionID)
        {
            return &SERVER_SESSIONS[i];
        }
    }
#ifdef APPSERVER_DEBUG
    log_warn("Could not find pointer to Session specified");
#endif
    return NULL;
}

NetPlayer *ServerGetPlayerByID(int id)
{
    for (size_t i = 0; i < server->players->size; i++)
    {
        if (SERVER_PLAYERS[i].id == id)
        {
            return &SERVER_PLAYERS[i];
        }
    }
#ifdef APPSERVER_DEBUG
    log_warn("Could not find pointer to NetPlayer specified");
#endif
    return NULL;
}

NetPlayer *ServerGetPlayerByNetPlayer(NetPlayer player)
{
    for (size_t i = 0; i < server->players->size; i++)
    {
        if (!SDL_memcmp(&SERVER_PLAYERS[i], &player, sizeof(NetPlayer)))
        {
            return &SERVER_PLAYERS[i];
        }
    }
#ifdef APPSERVER_DEBUG
    log_warn("Could not find pointer to NetPlayer specified");
#endif
    return NULL;
}

void ServerRemovePlayerFromSession(Session *session, int playerID)
{
    // Remove leaving player from session
    VectorEraseElement(session->playerIDs, &playerID);

    // If number of players in session is 0, remove session
    if (session->playerIDs->size == 0)
    {
        VectorEraseElement(server->sessions, session);
    }
    // Check if leaving player was host
    else if (session->hostID == playerID)
    {
        // Randomize new host
        int r = rand() % session->playerIDs->size;
        session->hostID = SessionGetPlayerIDs(session)[r];
        // Notifes new host that they are host
        ServerTCPSend(PT_HostAssign, NULL, 0, *ServerGetPlayerByID(session->hostID));
    }

    // Notify everyone else in session that the client left
    if (playerID != -1)
        ServerTCPBroadcastSession(PT_DelPlayer, session, &playerID, sizeof(int));

    NetPlayer *playerP = ServerGetPlayerByID(playerID);
    if (!playerP)
        return;

    // Sets the sessionID of player to -1, so that they can join a new session
    playerP->sessionID = -1;
    // Clears points earned in this session
    playerP->pointBuffer = 0.0f;
    // Clears entity
    playerP->entity = (Entity){0};
}

void ServerClearInBuffer()
{
    for (size_t i = 0; i < server->inBuffer->size; i++)
    {
        ParsedPacketDestroy(&SERVER_INBUFFER[i]);
    }
    VectorClear(server->inBuffer);
}

void ServerAddSession(Session *session)
{
    VectorPushBack(server->sessions, session);
}

SDL_bool ServerIsListening()
{
    return server->isListening;
}
ParsedPacket *ServerGetInBuffer()
{
    return (ParsedPacket *)server->inBuffer->data;
}
size_t ServerGetInBufferSize()
{
    return server->inBuffer->size;
}
SDL_mutex *ServerGetInBufferMutex()
{
    return server->inBufferMutex;
}
NetPlayer *ServerGetPlayers()
{
    return (NetPlayer *)server->players->data;
}
size_t ServerGetNumPlayers()
{
    return server->players->size;
}
SDL_bool *ServerGetIDs()
{
    return (SDL_bool *)server->ids->data;
}
Session *ServerGetSessions()
{
    return (Session *)server->sessions->data;
}
size_t ServerGetNumSessions()
{
    return server->sessions->size;
}
SDL_bool *ServerGetSessionBitmaps()
{
    return (SDL_bool *)server->sessionBitmap->data;
}
SDL_mutex *ServerGetTrafficMutex()
{
    return server->trafficMutex;
}