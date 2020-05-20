#include "ClientManager.h"

typedef struct ClientManager
{
    Vector *players;
    Vector *shootingLines;
    // A vector of structs, JoinableSession
    Vector *joinList;
    SDL_bool inLobby; // both of these cannot be true at once
    SDL_bool inGame;  // :)
} ClientManager;

static ClientManager *clientManager;

void ClientManagerInitialize()
{
    clientManager = MALLOC(ClientManager);
    ALLOC_ERROR_CHECK(clientManager);

    clientManager->players = VectorCreate(sizeof(EntityIndexP), 100);
    clientManager->shootingLines = VectorCreate(sizeof(ShootData), 10);
    clientManager->joinList = VectorCreate(sizeof(JoinableSession), 5);
    clientManager->inLobby = SDL_FALSE;
    clientManager->inGame = SDL_FALSE;
    LobbyInitialize();
    InstanceInitialize();
}

void ClientManagerUninitialize()
{
    InstanceUninitialize();
    LobbyUninitialize();
    for (int i = 0; i < clientManager->players->size; i++)
        EntityManagerRemove(CLIENTMANAGER_PLAYERS[i]);
    VectorDestroy(clientManager->joinList);
    VectorDestroy(clientManager->shootingLines);
    VectorDestroy(clientManager->players);
    FREE(clientManager);
}

void ClientManagerHandleAllPackets()
{
    ClientManagerDisconnectFromTimeoutServer();

    SDL_LockMutex(ClientGetInBufferMutex());
    for (size_t i = 0; i < ClientGetInBufferSize(); i++)
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
        case PT_CloseSession:
            ClientManagerHandleCloseSessionPacket(nextPacket);
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
        case PT_StartRound:
            ClientManagerHandleStartRoundPacket(nextPacket);
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
        case PT_PlayerDead:
            ClientManagerHandlePlayerDeadPacket(nextPacket);
            break;
        case PT_PlayerShoot:
            ClientManagerHandlePlayerShootPacket(nextPacket);
            break;
        case PT_CloseAllSessions:
            ClientManagerHandleCloseAllSessionsPacket(nextPacket);
            break;
        case PT_NewRound:
            ClientManagerHandleNewRoundPacket(nextPacket);
            break;
        case PT_RoundFinished:
            ClientManagerHandleRoundFinishedPacket(nextPacket);
            break;
        case PT_MatchFinished:
            ClientManagerHandleMatchFinishedPacket(nextPacket);
            break;
        case PT_Countdown:
            ClientManagerHandleCountdownPacket(nextPacket);
            break;
        case PT_FetchScoreboard:
            ClientManagerHandleFetchScoreboardPacket(nextPacket);
            break;
        default:
            break;
        }
    }
    ClientClearInBuffer();
    SDL_UnlockMutex(ClientGetInBufferMutex());
}

void ClientManagerDrawConnectedPlayers()
{
    for (int i = 0; i < clientManager->players->size; i++)
    {
        EntityDrawIndex(CLIENTMANAGER_PLAYERS[i]);
    }
}

void ClientManagerDrawBufferedShootingLines()
{
    float shootLen = 35.0f;
    for (int i = 0; i < clientManager->shootingLines->size; i++)
    {
        ShootData *line = &((ShootData *)clientManager->shootingLines->data)[i];
        Vec2 delta = Vec2MulL(line->dir, shootLen);
        Vec2 startToEnd = Vec2Sub(line->end, line->start);
        float step = ClockGetDeltaTime() * 5000.0f;

        if (step + shootLen < Vec2Len(startToEnd))
        {
            line->start = Vec2Add(line->start, Vec2MulL(line->dir, step));
            CameraDrawLine(line->start.x, line->start.y, line->start.x + delta.x, line->start.y + delta.y, (SDL_Color){200, 200, 200, 255});
        }
        else
        {
            delta = startToEnd;
            CameraDrawLine(line->start.x, line->start.y, line->start.x + delta.x, line->start.y + delta.y, (SDL_Color){200, 200, 200, 255});
            VectorErase(clientManager->shootingLines, i);
            i--;
        }
    }
}

void ClientManagerDisconnectFromTimeoutServer()
{
    if (ConStateGet() == CON_Online && ClientGetTimeoutTimer() > SERVER_TIMEOUT)
    {
#ifdef CLIENTMANAGER_DEBUG
        log_info("Disconnected from server Reason: Timeout");
#endif
        ClientDisconnect();
        ClientManagerLeaveSessionLocally();
        Notify("Connection closed", 1.0f, NT_ERROR);
    }
}

void ClientManagerLeaveSessionLocally()
{
    clientManager->inLobby = SDL_FALSE;
    clientManager->inGame = SDL_FALSE;
    GameStateSet(GS_Menu);
    MenuState menuState = MenuStateGet();
    if (menuState == MS_None ||
        menuState == MS_JoinLobby ||
        menuState == MS_HostLobby ||
        menuState == MS_WaitingForLobby ||
        menuState == MS_Lobby)
        MenuStateSet(MS_MainMenu);
}

void ClientManagerClearPlayers()
{
    for (int i = 0; i < clientManager->players->size; i++)
    {
        EntityManagerRemove(CLIENTMANAGER_PLAYERS[i]);
        VectorClear(clientManager->players);
    }
}

void ClientManagerAddShootingLine(ShootData shootData)
{
    VectorPushBack(clientManager->shootingLines, &shootData);
}

SDL_bool ClientManagerIsInGame()
{
    return clientManager->inGame;
}
SDL_bool ClientManagerIsInLobby()
{
    return clientManager->inLobby;
}
size_t ClientManagerGetJoinListSize()
{
    return clientManager->joinList->size;
}

void ClientManagerSetInLobby(SDL_bool inLobby)
{
    clientManager->inLobby = inLobby;
}
void ClientManagerSetInGame(SDL_bool inGame)
{
    clientManager->inGame = inGame;
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
    ClientReceivedAlivePacket();
}

void ClientManagerHandleIsPlayerActivePacket(ParsedPacket packet)
{
    struct PlayerStatus
    {
        int id;
        SDL_bool active;
    } playerStatus = *(struct PlayerStatus *)packet.data;

    if (!playerStatus.active)
    {
        for (int i = 0; i < clientManager->players->size; i++)
        {
            Entity *current = &ENTITY_ARRAY[*CLIENTMANAGER_PLAYERS[i]];
            if (current->id == playerStatus.id)
            {
                EntityManagerRemove(CLIENTMANAGER_PLAYERS[i]);
                VectorEraseElement(clientManager->players, &CLIENTMANAGER_PLAYERS[i]);
                return;
            }
        }
    }
}

void ClientManagerHandleConnectPacket(ParsedPacket packet)
{
    int id = *(int *)packet.data;
    ClientSetPlayerID(id);
    // Sends a empty packet to signal the server which IP to respond with when sending UDP-packets
    ClientUDPSend(PT_None, NULL, 0);
}

void ClientManagerHandleDuplicateNamePacket(ParsedPacket packet)
{
    MenuStateSet(MS_Name);
    ClientClearName();
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
    VectorPushBack(clientManager->players, &newEntity);
}

void ClientManagerHandleDelPlayerPacket(ParsedPacket packet)
{
    int id = *(int *)packet.data;

    for (size_t i = 0; i < clientManager->players->size; i++)
    {
        if (ENTITY_ARRAY[*CLIENTMANAGER_PLAYERS[i]].id == id)
        {
            EntityManagerRemove(CLIENTMANAGER_PLAYERS[i]);
            VectorErase(clientManager->players, i);
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

    for (int i = 0; i < clientManager->players->size; i++)
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

void ClientManagerHandleCloseSessionPacket(ParsedPacket packet)
{
    if (ClientManagerIsInGame())
    {
        GameStateSet(GS_Menu);
        MenuStateSet(MS_Summary);
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
        clientManager->inLobby = SDL_TRUE;
    }
}

void ClientManagerHandleFullSessionPacket(ParsedPacket packet)
{
    if (MenuStateGet() == MS_WaitingForLobby)
    {
        MenuStateSet(MS_JoinLobby);
    }
}

void ClientManagerHandleStartRoundPacket(ParsedPacket packet)
{
    if (MenuStateGet() != MS_Lobby && GameStateGet() != GS_RoundFinished)
        return;
    Entity *entity = (Entity *)packet.data;
    ClientSetPlayerEntity(entity);
    PlayerSetSpawnPoint(entity->position);
    MenuStateSet(MS_None);
    GameStateSet(GS_Playing);
    ClientManagerSetInGame(SDL_TRUE);
    ClientManagerSetInLobby(SDL_TRUE);
    LobbyClearNames();
    LobbySetIsHost(SDL_FALSE);
    ClientManagerClearPlayers();
}

void ClientManagerHandleHostAssignPacket(ParsedPacket packet)
{
    LobbySetIsHost(SDL_TRUE);
}

void ClientManagerHandleFetchSessionsPacket(ParsedPacket packet)
{
    // Clear joinList of old joinable sessions
    VectorClear(clientManager->joinList);

    // If no session exists, add spcecial element to joinList
    if (!packet.size)
        return;

    JoinableSession *fetched = (JoinableSession *)packet.data;
    int nSessions = packet.size / sizeof(JoinableSession);

    // Add all joinable sessions to joinList
    for (int i = 0; i < nSessions; i++)
    {
        VectorPushBack(clientManager->joinList, &fetched[i]);
    }
}

void ClientManagerHandleFetchLobbyPacket(ParsedPacket packet)
{
    if (clientManager->inLobby)
    {
        char *allMembers = (char *)packet.data;

        int nLobbyMembers = packet.size / 20;

        LobbyClearNames();
        for (int i = 0; i < nLobbyMembers; i++)
            LobbyAddName(allMembers + i * 20);
    }
}

void ClientManagerHandlePlayerHitPacket(ParsedPacket packet)
{
    HitData hitData = *(HitData *)packet.data;
    Entity *playerEntity = PlayerGetEntity();
    if (playerEntity->id == hitData.id)
    {
        playerEntity->health -= hitData.damage;
        return;
    }
    for (size_t i = 0; i < clientManager->players->size; i++)
    {
        Entity *current = &ENTITY_ARRAY[*CLIENTMANAGER_PLAYERS[i]];
        if (current->id == hitData.id)
        {
            current->health -= hitData.damage;
            return;
        }
    }
}

void ClientManagerHandlePlayerShootPacket(ParsedPacket packet)
{
    ShootData shootData = *(ShootData *)packet.data;
    ClientManagerAddShootingLine(shootData);
}

void ClientManagerHandlePlayerDeadPacket(ParsedPacket packet)
{
    int id = *(int *)packet.data;
    for (int i = 0; i < clientManager->players->size; i++)
    {
        Entity *current = &ENTITY_ARRAY[*CLIENTMANAGER_PLAYERS[i]];
        if (current->id == id)
        {
            current->nDrawables = 1;
            current->isCollider = SDL_FALSE;
            current->drawables[0].dst = (SDL_Rect){0, 0, 70, 70};
        }
    }
}

void ClientManagerHandleNewRoundPacket(ParsedPacket packet)
{
    int round = *(int *)packet.data;
    InstanceSetRound(round);
    PlayerRevive();
}

void ClientManagerHandleCloseAllSessionsPacket(ParsedPacket packet)
{
    ClientManagerLeaveSessionLocally();
}

void ClientManagerHandleMatchFinishedPacket(ParsedPacket packet)
{
    if (clientManager->inGame)
    {
        GameStateSet(GS_MatchFinished);
    }
}

void ClientManagerHandleRoundFinishedPacket(ParsedPacket packet)
{
    if (clientManager->inGame)
    {
        GameStateSet(GS_RoundFinished);
    }
}

void ClientManagerHandleCountdownPacket(ParsedPacket packet)
{
    RoundSetCountdown(*(float *)packet.data);
}

void ClientManagerHandleFetchScoreboardPacket(ParsedPacket packet)
{
    int nScoreboardEntries = packet.size / sizeof(ScoreboardEntry);
    for (int i = 0; i < nScoreboardEntries; i++)
    {
        ScoreboardEntry entry = ((ScoreboardEntry *)packet.data)[i];
        ScoreboardAddPlayer(entry.id, entry.name, entry.score);
    }
}

EntityIndexP *ClientManagerGetPlayersArray()
{
    return (EntityIndexP *)clientManager->players->data;
}

JoinableSession *ClientManagerGetJoinListArray()
{
    return (JoinableSession *)clientManager->joinList->data;
}