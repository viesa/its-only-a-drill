#include "ClientManager.h"

void ClientManagerInitialize()
{
    clientManager.players = VectorCreate(sizeof(EntityIndexP), 100);
    clientManager.joinList = VectorCreate(sizeof(JoinableSession), 5);
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
    SDL_LockMutex(client.inBufferMutex);
    for (size_t i = 0; i < client.inBuffer->size; i++)
    {
        ParsedPacket nextPacket = CLIENT_INBUFFER[i];

        switch (nextPacket.type)
        {
        case PT_Text:
            ClientManagerHandleTextPacket(nextPacket);
            break;
        case PT_Connect:
            ClientManagerHandleConnectPacket(nextPacket);
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
        case PT_LeaveSession:
            ClientManagerHandleLeaveSessionPacket(nextPacket);
            break;
        case PT_FullSession:
            ClientManagerHandleFullSessionPacket(nextPacket);
            break;
        case PT_FetchSessions:
            ClientManagerHandleFetchSessionsPacket(nextPacket);
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

void ClientManagerHandleTextPacket(ParsedPacket packet)
{
}

void ClientManagerHandleConnectPacket(ParsedPacket packet)
{
    int ID = *(int *)packet.data;
    ENTITY_ARRAY[*client.player->entity].id = ID;
    client.receivedPlayerID = SDL_TRUE;
}

void ClientManagerHandleDisconnectPacket(ParsedPacket packet)
{
    // Server quit
}

void ClientManagerHandleNewPlayerPacket(ParsedPacket packet)
{
    int id = *(int *)packet.data;

    EntityIndexP newEntity = EntityManagerAdd(ET_Player, Vec2Create(0.0f, 0.0f));
    ENTITY_ARRAY[*newEntity].id = id;
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

    for (int i = 1; i < ENTITY_ARRAY_SIZE; i++)
    {
        if (ENTITY_ARRAY[i].id == cEntity.id) //entity exists
        {
            EntityAddCompressed(&ENTITY_ARRAY[i], &cEntity);
            return;
        }
    }
}

void ClientManagerHandleCreateSessionPacket(ParsedPacket packet)
{
    int sessionID = *(int *)packet.data;

    // If session was not created (error), return to main menu
    if (sessionID == -1)
    {
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
        MapGenerateNew(mapData);
        JSONDestroy(mapData);
        MenuStateSet(MS_Lobby);
    }
}

void ClientManagerHandleFullSessionPacket(ParsedPacket packet)
{
    if (MenuStateGet() == MS_WaitingForLobby)
    {
        MenuStateSet(MS_JoinLobby);
    }
}

void ClientManagerHandleLeaveSessionPacket(ParsedPacket packet)
{
}

void ClientManagerHandleFetchSessionsPacket(ParsedPacket packet)
{
    // If no session exists, add spcecial element to joinList
    if (!packet.size)
    {
        JoinableSession empty;
        strcpy(empty.name, "No session avaiable");
        empty.maxPlayers = -1;
        empty.currentPlayers = -1;
        empty.sessionID = -1;
        VectorPushBack(clientManager.joinList, &empty);
    }

    JoinableSession *fetched = (JoinableSession *)packet.data;
    int nSessions = packet.size / sizeof(JoinableSession);

    // Clear joinList of old joinable sessions
    VectorClear(clientManager.joinList);

    // Add all joinable sessions to joinList
    for (int i = 0; i < nSessions; i++)
    {
        VectorPushBack(clientManager.joinList, &fetched[i]);
    }
}

EntityIndexP *ClientManagerGetPlayersArray()
{
    return (EntityIndexP *)clientManager.players->data;
}

JoinableSession *ClientManagerGetJoinListArray()
{
    return (JoinableSession *)clientManager.joinList->data;
}