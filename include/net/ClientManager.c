#include "ClientManager.h"

void ClientManagerInitialize()
{
    clientManager.players = VectorCreate(sizeof(EntityIndexP), 100);
    clientManager.joinList = VectorCreate(sizeof(JoinableSession), 5);
    clientManager.inLobby = SDL_FALSE;
    clientManager.inGame = SDL_FALSE;
}

void ClientManagerUninitialize()
{
    for (int i = 0; i < clientManager.players->size; i++)
        EntityManagerRemove(CLIENTMANAGER_PLAYERS[i]);
    VectorDestroy(clientManager.players);
    VectorDestroy(clientManager.joinList);
}

void ClientManagerUpdate()
{
    ClientManagerUpdateServerTimeoutTimer();
    ClientManagerDisconnectFromTimeoutServer();
    ClientManagerPingServer();

    SDL_LockMutex(client.inBufferMutex);
    for (size_t i = 0; i < client.inBuffer->size; i++)
    {
        ParsedPacket nextPacket = CLIENT_INBUFFER[i];

        switch (nextPacket.type)
        {
        case PT_Text:
            ClientManagerHandleTextPacket(nextPacket);
            break;
        case PT_AreYouAlive:
            ClientManagerHandleAreYouAlivePacket(nextPacket);
            break;
        case PT_IAmAlive:
            ClientManagerHandleIAmAlivePacket(nextPacket);
            break;
        case PT_Connect:
            ClientManagerHandleConnectPacket(nextPacket);
            break;
        case PT_DuplicateName:
            ClientManagerHandleDuplicateNamePacket(nextPacket);
            break;
        case PT_Disconnect:
            ClientManagerHandleDisconnectPacket(nextPacket);
            break;
        case PT_NewPlayer:
            ClientManagerHandleNewPlayerPacket(nextPacket);
            break;
        case PT_DelPlayer:
            ClientManagerHandleDelPlayerPacket(nextPacket);
            break;
        case PT_CompressedEntity:
            ClientManagerHandleCompressedEntityPacket(nextPacket);
            break;
        case PT_CreateSession:
            ClientManagerHandleCreateSessionPacket(nextPacket);
            break;
        case PT_JoinSession:
            ClientManagerHandleJoinSessionPacket(nextPacket);
            break;
        case PT_FullSession:
            ClientManagerHandleFullSessionPacket(nextPacket);
            break;
        case PT_StartSession:
            ClientManagerHandleStartSessionPacket(nextPacket);
            break;
        case PT_HostAssign:
            ClientManagerHandleHostAssignPacket(nextPacket);
            break;
        case PT_FetchSessions:
            ClientManagerHandleFetchSessionsPacket(nextPacket);
            break;
        case PT_FetchLobby:
            ClientManagerHandleFetchLobbyPacket(nextPacket);
            break;
        case PT_PlayerHit:
            ClientManagerHandlePlayerHitPacket(nextPacket);
            break;
        case PT_CloseAllSessions:
            ClientManagerHandleCloseAllSessionsPacket(nextPacket);
            break;
        default:
            break;
        }
    }
    for (size_t i = 0; i < client.inBuffer->size; i++)
    {
        ParsedPacketDestroy(&CLIENT_INBUFFER[i]);
    }
    VectorClear(client.inBuffer);
    SDL_UnlockMutex(client.inBufferMutex);
}

void ClientManagerDrawConnectedPlayers(Camera *camera)
{
    for (int i = 0; i < clientManager.players->size; i++)
    {
        EntityDrawIndex(CLIENTMANAGER_PLAYERS[i], camera);
    }
}

void ClientManagerUpdateServerTimeoutTimer()
{
    if (ConStateGet() == CON_Online && client.server.waitingForAliveReply)
    {
        client.server.timeoutTimer += ClockGetDeltaTime();
    }
}

void ClientManagerDisconnectFromTimeoutServer()
{
    if (ConStateGet() == CON_Online && client.server.timeoutTimer > SERVER_TIMEOUT)
    {
#ifdef CLIENTMANAGER_DEBUG
        log_info("Disconnected from server Reason: Timeout");
#endif
        ClientDisconnect();
        ClientManagerLeaveSessionLocally();
        Notify("Connection closed", 1.0f, NT_ERROR);
    }
}

void ClientManagerPingServer()
{
    if (ConStateGet() == CON_Online && !client.server.waitingForAliveReply)
    {
        ClientTCPSend(PT_AreYouAlive, NULL, 0);
        client.server.waitingForAliveReply = SDL_TRUE;
    }
}

void ClientManagerLeaveSessionLocally()
{
    clientManager.inLobby = SDL_FALSE;
    clientManager.inGame = SDL_FALSE;
    GameStateSet(GS_Menu);
    MenuState menuState = MenuStateGet();
    if (menuState == MS_None ||
        menuState == MS_JoinLobby ||
        menuState == MS_HostLobby ||
        menuState == MS_WaitingForLobby ||
        menuState == MS_Lobby)
        MenuStateSet(MS_MainMenu);
}

void ClientManagerHandleTextPacket(ParsedPacket packet)
{
    // Maybe we can print it? O.O
}

void ClientManagerHandleAreYouAlivePacket(ParsedPacket packet)
{
    ClientTCPSend(PT_IAmAlive, NULL, 0);
}

void ClientManagerHandleIAmAlivePacket(ParsedPacket packet)
{
    client.server.timeoutTimer = 0.0f;
    client.server.waitingForAliveReply = SDL_FALSE;
}

void ClientManagerHandleConnectPacket(ParsedPacket packet)
{
    int id = *(int *)packet.data;
    PlayerGetEntity(client.player)->id = id;
    client.receivedPlayerID = SDL_TRUE;

    // Sends a empty packet to signal the server which IP to respond with
    ClientUDPSend(PT_None, NULL, 0);
}

void ClientManagerHandleDuplicateNamePacket(ParsedPacket packet)
{
    MenuStateSet(MS_Name);
    SDL_memset(client.name, 0, MAX_PLAYERNAME_SIZE);
    Notify("Username taken", 1.0f, NT_ERROR);
}

void ClientManagerHandleDisconnectPacket(ParsedPacket packet)
{
    ClientDisconnect();
    Notify("Connection closed", 1.0f, NT_ERROR);
}

void ClientManagerHandleNewPlayerPacket(ParsedPacket packet)
{
    Entity *entity = (Entity *)packet.data;

    EntityIndexP newEntity = EntityManagerAddNoConfig();
    ENTITY_ARRAY[*newEntity] = *entity;
    VectorPushBack(clientManager.players, &newEntity);
}

void ClientManagerHandleDelPlayerPacket(ParsedPacket packet)
{
    int id = *(int *)packet.data;

    for (size_t i = 0; i < clientManager.players->size; i++)
    {
        if (ENTITY_ARRAY[*CLIENTMANAGER_PLAYERS[i]].id == id)
        {
            EntityManagerRemove(CLIENTMANAGER_PLAYERS[i]);
            VectorErase(clientManager.players, i);
            break;
        }
    }
}

void ClientManagerHandleEntityPacket(ParsedPacket packet)
{
    Entity incomingEntity = *(Entity *)packet.data;

    for (int i = 1; i < ENTITY_ARRAY_SIZE; i++)
    {
        if (ENTITY_ARRAY[i].id == incomingEntity.id) //entity exists
        {
            ENTITY_ARRAY[i] = incomingEntity;
            return;
        }
    }
}

void ClientManagerHandleCompressedEntityPacket(ParsedPacket packet)
{
    CompressedEntity cEntity = *(CompressedEntity *)packet.data;

    for (int i = 0; i < clientManager.players->size; i++)
    {
        Entity *entity = &ENTITY_ARRAY[*CLIENTMANAGER_PLAYERS[i]];
        if (entity->id == cEntity.id)
        {
            EntityAddCompressed(entity, &cEntity);
            return;
        }
    }
    for (int i = 0; i < MapGetContentSize(); i++)
    {
        Entity *entity = &ENTITY_ARRAY[*MapGetContents()[i]];
        if (entity->id == cEntity.id)
        {
            EntityAddCompressed(entity, &cEntity);
            return;
        }
    }
}

void ClientManagerHandleCreateSessionPacket(ParsedPacket packet)
{
    int sessionID = *(int *)packet.data;

    if (sessionID == -1)
    {
        // If session failed to be created, return to main menu
        MenuStateSet(MS_MainMenu);
    }
    if (sessionID >= 0)
    {
        // If session was created successfully, attempt to join the session with sessionID
        ClientTCPSend(PT_JoinSession, &sessionID, sizeof(int));
        MenuStateSet(MS_WaitingForLobby);
    }
}

void ClientManagerHandleJoinSessionPacket(ParsedPacket packet)
{
    if (MenuStateGet() == MS_WaitingForLobby)
    {
        char *rawData = (char *)packet.data;
        JSON *mapData = JSONCreateFromArray(rawData, packet.size);
        if (!mapData)
        {
            // Failed to create a JSON-mapdata from incoming data
            MenuStateSet(MS_MainMenu);
            ClientTCPSend(PT_LeaveSession, NULL, 0);
            return;
        }
        int result = MapGenerateNew(mapData);
        if (!result)
        {
            // Failed to create a map from JSON-data
            JSONDestroy(mapData);
            MenuStateSet(MS_MainMenu);
            ClientTCPSend(PT_LeaveSession, NULL, 0);
            return;
        }
        MenuStateSet(MS_Lobby);
        clientManager.inLobby = SDL_TRUE;
    }
}

void ClientManagerHandleFullSessionPacket(ParsedPacket packet)
{
    if (MenuStateGet() == MS_WaitingForLobby)
    {
        MenuStateSet(MS_JoinLobby);
    }
}

void ClientManagerHandleStartSessionPacket(ParsedPacket packet)
{
    if (MenuStateGet() != MS_Lobby)
        return;
    *PlayerGetEntity(client.player) = *(Entity *)packet.data;
    MenuStateSet(MS_None);
    GameStateSet(GS_Playing);
    clientManager.inGame = SDL_TRUE;
    clientManager.inLobby = SDL_FALSE;
    VectorClear(lobby.names);
    lobby.isHost = SDL_FALSE;
}

void ClientManagerHandleHostAssignPacket(ParsedPacket packet)
{
    lobby.isHost = SDL_TRUE;
}

void ClientManagerHandleFetchSessionsPacket(ParsedPacket packet)
{
    // Clear joinList of old joinable sessions
    VectorClear(clientManager.joinList);

    // If no session exists, add spcecial element to joinList
    if (!packet.size)
        return;

    JoinableSession *fetched = (JoinableSession *)packet.data;
    int nSessions = packet.size / sizeof(JoinableSession);

    // Add all joinable sessions to joinList
    for (int i = 0; i < nSessions; i++)
    {
        VectorPushBack(clientManager.joinList, &fetched[i]);
    }
}

void ClientManagerHandleFetchLobbyPacket(ParsedPacket packet)
{
    if (clientManager.inLobby)
    {
        char *allMembers = (char *)packet.data;

        int nLobbyMembers = packet.size / 20;

        VectorClear(lobby.names);
        for (int i = 0; i < nLobbyMembers; i++)
        {
            LobbyName name = {0};
            strcpy(name.data, allMembers + i * 20);
            VectorPushBack(lobby.names, &name);
        }
    }
}

void ClientManagerHandlePlayerHitPacket(ParsedPacket Packet)
{
    int send = *(int *)Packet.data;

    PlayerGetEntity(client.player)->health -= send;
}

void ClientManagerHandleCloseAllSessionsPacket(ParsedPacket packet)
{
    ClientManagerLeaveSessionLocally();
}

EntityIndexP *ClientManagerGetPlayersArray()
{
    return (EntityIndexP *)clientManager.players->data;
}

JoinableSession *ClientManagerGetJoinListArray()
{
    return (JoinableSession *)clientManager.joinList->data;
}