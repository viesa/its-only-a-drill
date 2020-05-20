#include "ServerManager.h"

void ServerManagerHandleAllPackets()
{
    SDL_LockMutex(ServerGetInBufferMutex());
    for (size_t i = 0; i < ServerGetInBufferSize(); i++)
    {
        ParsedPacket nextPacket = SERVER_INBUFFER[i];

        switch (nextPacket.type)
        {
        case PT_Text:
            ServerManagerHandleTextPacket(nextPacket);
            break;
        case PT_AreYouAlive:
            ServerManagerHandleAreYouAlivePacket(nextPacket);
            break;
        case PT_IAmAlive:
            ServerManagerHandleIAmAlivePacket(nextPacket);
            break;
        case PT_Connect:
            ServerManagerHandleConnectPacket(nextPacket);
            break;
        case PT_UDPRespondIP:
            ServerManagerHandleUDPRespondIPPacket(nextPacket);
            break;
        case PT_Disconnect:
            ServerManagerHandleDisconnectPacket(nextPacket);
            break;
        case PT_Entity:
            ServerManagerHandleEntityPacket(nextPacket);
            break;
        case PT_CompressedEntity:
            ServerManagerHandleCompressedEntityPacket(nextPacket);
            break;
        case PT_CreateSession:
            ServerManagerHandleCreateSessionPacket(nextPacket);
            break;
        case PT_JoinSession:
            ServerManagerHandleJoinSessionPacket(nextPacket);
            break;
        case PT_LeaveSession:
            ServerManagerHandleLeaveSessionPacket(nextPacket);
            break;
        case PT_StartRound:
            ServerManagerHandleStartRoundPacket(nextPacket);
            break;
        case PT_ChangeSkin:
            ServerManagerHandleChangeSkinPacket(nextPacket);
            break;
        case PT_FetchSessions:
            ServerManagerHandleFetchSessionsPacket(nextPacket);
            break;
        case PT_FetchLobby:
            ServerManagerHandleFetchLobbyPacket(nextPacket);
            break;
        case PT_PlayerHit:
            ServerManagerHandlePlayerHitPacket(nextPacket);
            break;
        case PT_PlayerShoot:
            ServerManagerHandlePlayerShootPacket(nextPacket);
            break;
        case PT_PlayerDead:
            ServerManagerHandlePlayerDeadPacket(nextPacket);
            break;
        case PT_FetchScoreboard:
            ServerManagerHandleFetchScoreboardPacket(nextPacket);
            break;
        default:
            break;
        }
    }
    ServerClearInBuffer();
    SDL_UnlockMutex(ServerGetInBufferMutex());
}

void ServerManagerKickTimeoutClients()
{
    for (int i = 0; i < ServerGetNumPlayers(); i++)
    {
        if (SERVER_PLAYERS[i].timeoutTimer > CLIENT_TIMEOUT)
        {
#ifdef APPSERVER_DEBUG
            log_info("Disconnected player: %s Reason: Timeout", SERVER_PLAYERS[i].name);
#endif
            ServerRemoveClient(SERVER_PLAYERS[i]);
        }
    }
}

void ServerManagerAdvanceSessionsWithOnePlayerAlive()
{
    int nSessions = ServerGetNumSessions();
    int nRemovedSesssions = 0;
    for (int i = 0; i < nSessions - nRemovedSesssions; i++)
    {
        int nAlivePlayers = 0;
        Session *session = &SERVER_SESSIONS[i];
        if (session->inGame)
        {
            NetPlayer *playerLeft = NULL;
            for (int j = 0; j < session->playerIDs->size; j++)
            {
                NetPlayer *player = ServerGetPlayerByID(SessionGetPlayerIDs(session)[j]);
                if (!player)
                {
                    ServerRemovePlayerFromSession(session, SessionGetPlayerIDs(session)[j]);
                    break;
                }
                if (player->state == NPS_Alive)
                {
                    nAlivePlayers++;
                    playerLeft = player;
                }
                if (nAlivePlayers > 1)
                    break;
            }
            if (nAlivePlayers <= 1 && !session->quittingMatch && !session->startingNewRound)
            {
                // Award the winning player with points for winning, if he is alive
                if (playerLeft)
                    playerLeft->pointBuffer += (float)(session->playerIDs->size * 10);

                if (++session->currentRound >= session->nRounds || session->playerIDs->size <= 1)
                {
                    // Match finished
                    session->finishedMatchCountdown = 5.0f;
                    session->quittingMatch = SDL_TRUE;
                    ServerTCPBroadcastSession(PT_MatchFinished, session, NULL, 0);
                }
                else
                {
                    // Round finished
                    session->finishedRoundCountdown = 5.0f;
                    session->startingNewRound = SDL_TRUE;
                    ServerTCPBroadcastSession(PT_RoundFinished, session, NULL, 0);
                }
            }

            if (session->quittingMatch)
            {
                session->finishedMatchCountdown -= ClockGetDeltaTime();
                ServerUDPBroadcastSession(PT_Countdown, session, &session->finishedMatchCountdown, sizeof(float));
                if (session->finishedMatchCountdown <= 0.0f)
                {
                    ServerTCPBroadcastSession(PT_CloseSession, session, NULL, 0);
                    ServerRemoveSession(session);
                    i--;
                    nRemovedSesssions++;
                    session->quittingMatch = SDL_FALSE;
                }
            }
            else if (session->startingNewRound)
            {
                session->finishedRoundCountdown -= ClockGetDeltaTime();
                ServerUDPBroadcastSession(PT_Countdown, session, &session->finishedRoundCountdown, sizeof(float));
                if (session->finishedRoundCountdown <= 0.0f)
                {
                    ServerTCPBroadcastSession(PT_NewRound, session, &session->currentRound, sizeof(int));
                    ServerManagerStartSession(session);
                    session->startingNewRound = SDL_FALSE;
                }
            }
        }
    }
}

void ServerManagerStartSession(Session *session)
{
    int *playerIDs = SessionGetPlayerIDs(session);
    // Notify client that the session started, and provide client with his entity
    for (int i = 0; i < session->playerIDs->size; i++)
    {
        NetPlayer *to = ServerGetPlayerByID(playerIDs[i]);
        to->state = NPS_Alive;
        Entity *entity = &to->entity;
        if (i < session->mapInfo.playerSpawns->size)
        {
            *entity = EntityCreate(MapInfoGetPlayerSpawns(&session->mapInfo)[i].position, ET_Player, to->id);
            for (int i = 0; i < MAX_DRAWABLES; i++)
            {
                entity->drawables[i].spriteSheet = to->spriteSheet;
                entity->drawables[i].dst.w = (int)(33.0f * 1.5f);
                entity->drawables[i].dst.h = (int)(33.0f * 1.5f);
            }
        }
        else
        {
            *entity = EntityCreate(Vec2Create(0.0f, 0.0f), ET_Player, to->id);
            for (int i = 0; i < MAX_DRAWABLES; i++)
                entity->drawables[i].spriteSheet = to->spriteSheet;
        }
        ServerTCPSend(PT_StartRound, entity, sizeof(Entity), *to);
    }
    // Notify the client about every other players, and provide client with their entites
    for (int i = 0; i < session->playerIDs->size; i++)
    {
        NetPlayer *to = ServerGetPlayerByID(playerIDs[i]);
        for (int j = 0; j < session->playerIDs->size; j++)
        {
            if (playerIDs[j] != to->id)
            {
                Entity *outgoing = &ServerGetPlayerByID(playerIDs[j])->entity;
                ServerTCPSend(PT_NewPlayer, outgoing, sizeof(Entity), *to);
            }
        }
    }
    session->inGame = SDL_TRUE;
    session->nDeadPlayers = 0;
}

void ServerManagerHandleTextPacket(ParsedPacket packet)
{
    // Print whatever came in
    printf("Received text-packet: %s", (char *)packet.data);
}

void ServerManagerHandleAreYouAlivePacket(ParsedPacket packet)
{
    // If player doesnt actually exist in server player array, discard packet
    NetPlayer *senderP = ServerGetPlayerByID(packet.sender.id);
    if (!senderP)
        return;
    ServerTCPSend(PT_IAmAlive, NULL, 0, packet.sender);
}

void ServerManagerHandleIAmAlivePacket(ParsedPacket packet)
{
    // If player doesnt actually exist in server player array, discard packet
    NetPlayer *senderP = ServerGetPlayerByID(packet.sender.id);
    if (!senderP)
        return;
    senderP->timeoutTimer = 0.0f;
    senderP->waitingForAliveReply = SDL_FALSE;
}

void ServerManagerHandleIsPlayerActivePacket(ParsedPacket packet)
{
    // If player doesnt actually exist in server player array, discard packet
    NetPlayer *senderP = ServerGetPlayerByID(packet.sender.id);
    if (!senderP)
        return;
    if (senderP->sessionID < 0)
        return;
    Session *session = ServerGetSessionByID(senderP->sessionID);
    if (!session)
        return;

    SDL_bool active = SDL_TRUE;
    SDL_bool inactive = SDL_FALSE;

    int queryID = *(int *)packet.data;
    NetPlayer *queryPlayer = ServerGetPlayerByID(queryID);
    if (!queryPlayer)
    {
        ServerTCPSend(PT_IsPlayerActive, &inactive, sizeof(SDL_bool), packet.sender);
    }
    if (queryPlayer->sessionID == senderP->sessionID)
    {
        ServerTCPSend(PT_IsPlayerActive, &active, sizeof(SDL_bool), packet.sender);
    }
    else
    {
        ServerTCPSend(PT_IsPlayerActive, &inactive, sizeof(SDL_bool), packet.sender);
    }
}

void ServerManagerHandleConnectPacket(ParsedPacket packet)
{
    // If player doesnt actually exist in server player array, discard packet
    NetPlayer *senderP = ServerGetPlayerByID(packet.sender.id);
    if (!senderP)
        return;
    // If player tried to change name to same name, do nothing (keep old)
    if (!strcmp(senderP->name, (char *)packet.data))
        return;

    for (size_t i = 0; i < ServerGetNumPlayers(); i++)
    {
        if (!strcmp(SERVER_PLAYERS[i].name, (char *)packet.data))
        {
            ServerTCPSend(PT_DuplicateName, NULL, 0, packet.sender);
            return;
        }
    }
    NetPlayer *playerP = ServerGetPlayerByID(packet.sender.id);
    strcpy(playerP->name, packet.data);
}

void ServerManagerHandleUDPRespondIPPacket(ParsedPacket packet)
{
    // If player doesnt actually exist in server player array, discard packet
    NetPlayer *senderP = ServerGetPlayerByID(packet.sender.id);
    if (!senderP)
        return;
    IPaddress ip = *(IPaddress *)packet.data;
    senderP->udpRespondIP = ip;
}

void ServerManagerHandleDisconnectPacket(ParsedPacket packet)
{
    // Try remove the client
    ServerRemoveClient(packet.sender);
}

void ServerManagerHandleNewPlayerPacket(ParsedPacket packet)
{
}

void ServerManagerHandleDelPlayerPacket(ParsedPacket packet)
{
}

void ServerManagerHandleEntityPacket(ParsedPacket packet)
{
    Entity incomingEntity = *(Entity *)packet.data;

    for (int i = 0; i < ServerGetNumPlayers(); i++)
    {
        if (SERVER_PLAYERS[i].entity.id == packet.sender.entity.id)
        {
            ServerTCPBroadcastExclusive(PT_Entity, &incomingEntity, sizeof(Entity), packet.sender);
            return;
        }
    }

    // THIS SHOULD NEVER HAPPEN
#ifdef SERVERMANAGER_DEBUG
    log_warn("Received an compressed entity that did not correspond to any player");
#endif
}

void ServerManagerHandleCompressedEntityPacket(ParsedPacket packet)
{
    // If player doesnt actually exist in server player array, discard packet
    NetPlayer *senderP = ServerGetPlayerByID(packet.sender.id);
    if (!senderP)
        return;
    if (senderP->sessionID < 0)
        return;

    CompressedEntity incomingCEntity = *(CompressedEntity *)packet.data;

    ServerUDPBroadcastExclusiveSession(PT_CompressedEntity, ServerGetSessionByID(senderP->sessionID), &incomingCEntity, sizeof(CompressedEntity), packet.sender);
}

void ServerManagerHandleCreateSessionPacket(ParsedPacket packet)
{
    int numRounds = 0;
    SDL_memcpy(&numRounds, packet.data, sizeof(int));
    char *rawMap = ((char *)packet.data) + sizeof(int);
    size_t rawMapDataSize = packet.size - sizeof(int);

    // If player doesn't actually exist in server player array, discard packet
    NetPlayer *senderP = ServerGetPlayerByID(packet.sender.id);
    if (!senderP)
        return;

    int sessionID = ServerGetSessionID();

    Session session = SessionCreate(sessionID, numRounds, senderP, rawMap, rawMapDataSize);
    // If session ID is -1, something bad happened when it was created
    if (session.id < 0)
    {
        int badSessionID = -1;
        ServerTCPSend(PT_CreateSession, &badSessionID, sizeof(int), packet.sender);
        SessionDestroy(&session);
    }
    else
    {
        ServerAddSession(&session);
        ServerTCPSend(PT_CreateSession, &session.id, sizeof(int), packet.sender);
    }
}

void ServerManagerHandleJoinSessionPacket(ParsedPacket packet)
{
    int sessionID = *(int *)packet.data;
    // Makes sure its a valid session ID, else discard packet
    if (sessionID < 0)
        return;
    // If player doesnt actually exist in server player array, discard packet
    NetPlayer *senderP = ServerGetPlayerByID(packet.sender.id);
    if (!senderP)
        return;

    // Check if player is already in another session, if so, disconnect it from the other session
    if (senderP->sessionID >= 0)
    {
        for (size_t i = 0; i < ServerGetNumSessions(); i++)
        {
            // Find correct session
            if (SERVER_SESSIONS[i].id == senderP->sessionID)
            {
                // Erase client from that session
                ServerRemovePlayerFromSession(&SERVER_SESSIONS[i], senderP->id);
            }
        }
        senderP->sessionID = -1;
    }

    // Find correct session that client wants to join
    Session *session = ServerGetSessionByID(sessionID);
    if (!session)
    {
        // If invalid session was sent, pretend it was full
        ServerTCPSend(PT_FullSession, NULL, 0, packet.sender);
        return;
    }
    if (session->inGame)
    {
        // If session was already started, pretend it was full
        // This could be a new packet, but honestly who cares :)
        ServerTCPSend(PT_FullSession, NULL, 0, packet.sender);
        return;
    }

    // Checks if there is room for player in the new session
    if (session->playerIDs->size < session->mapInfo.maxPlayers)
    {
        if (session->hostID < 0)
        {
            // If there is no host, you are host
            session->hostID = senderP->id;
        }
        if (senderP->id == session->hostID)
        {
            // Notifies client that they are host
            ServerTCPSend(PT_HostAssign, NULL, 0, packet.sender);
        }
        // Makes sure client isn't already in this session somehow
        if (VectorFind(session->playerIDs, &senderP->id) == session->playerIDs->size)
        {
            // Adds new player to session
            VectorPushBack(session->playerIDs, &senderP->id);
        }
        senderP->sessionID = sessionID;
        // Sends the session's map to client
        ServerTCPSend(PT_JoinSession, session->rawMap, session->rawMapDataSize, packet.sender);
    }
    // If there is no room for client, send back FullSesssion packet
    else
    {
        ServerTCPSend(PT_FullSession, NULL, 0, packet.sender);
    }
}

void ServerManagerHandleStartRoundPacket(ParsedPacket packet)
{
    // If player doesnt actually exist in server player array, discard packet
    NetPlayer *senderP = ServerGetPlayerByID(packet.sender.id);
    if (!senderP)
        return;
    if (senderP->sessionID < 0)
        return;

    // Make sure player is actually host over the session
    Session *session = ServerGetSessionByID(senderP->sessionID);
    if (senderP->id != session->hostID)
        return;

    ServerManagerStartSession(session);
}

void ServerManagerHandleLeaveSessionPacket(ParsedPacket packet)
{
    // If player doesnt actually exist in server player array, discard packet
    NetPlayer *senderP = ServerGetPlayerByID(packet.sender.id);
    if (!senderP)
        return;

    Session *session = ServerGetSessionByID(senderP->sessionID);
    if (!session)
        return;

    ServerRemovePlayerFromSession(session, senderP->id);
}

void ServerManagerHandleChangeSkinPacket(ParsedPacket packet)
{
    // If player doesnt actually exist in server player array, discard packet
    NetPlayer *senderP = ServerGetPlayerByID(packet.sender.id);
    if (!senderP)
        return;

    int int_spriteSheet = *(int *)packet.data;
    if (int_spriteSheet < 1 || int_spriteSheet > SS_Count)
        return;

    SpriteSheet spriteSheet = (SpriteSheet)int_spriteSheet;
    senderP->spriteSheet = spriteSheet;

    for (int i = 0; i < MAX_DRAWABLES; i++)
        senderP->entity.drawables[i].spriteSheet = spriteSheet;
}

void ServerManagerHandleFetchSessionsPacket(ParsedPacket packet)
{
    int nSessions = ServerGetNumSessions();
    // Reserve for possible max ammout of data
    // Actual number is allSesssions - nStartedSessions
    JoinableSession outgoing[nSessions];
    SDL_memset(outgoing, 0, sizeof(outgoing));

    int j = 0;
    for (int i = 0; i < nSessions; i++)
    {
        Session *current = &SERVER_SESSIONS[i];
        // Make sure session is not started
        if (!current->inGame)
        {
            // Make sure host is valid
            NetPlayer *pHost = ServerGetPlayerByID(current->hostID);
            if (pHost)
            {
                SDL_memset(outgoing[j].name, 0, 20);
                SDL_memset(outgoing[j].hostname, 0, 20);
                if (strlen(current->mapInfo.name) > 16)
                {
                    SDL_memcpy(outgoing[j].name, current->mapInfo.name, 16);
                    strcpy(outgoing[j].name + 16, "...");
                }
                else
                {
                    strcpy(outgoing[j].name, current->mapInfo.name);
                }
                strcpy(outgoing[j].hostname, pHost->name);
                outgoing[j].maxPlayers = current->mapInfo.maxPlayers;
                outgoing[j].currentPlayers = current->playerIDs->size;
                outgoing[j].sessionID = current->id;
                j++;
            }
        }
    }
    ServerTCPSend(PT_FetchSessions, outgoing, sizeof(JoinableSession) * j, packet.sender);
}

void ServerManagerHandleFetchLobbyPacket(ParsedPacket packet)
{
    // If player doesnt actually exist in server player array, discard packet
    NetPlayer *senderP = ServerGetPlayerByID(packet.sender.id);
    if (!senderP)
        return;
    if (senderP->sessionID < 0)
        return;

    Session *session = ServerGetSessionByID(senderP->sessionID);

    const int nSessionMemebers = session->playerIDs->size;
    const int nameSize = 20;
    const int arrSize = nSessionMemebers * nameSize;
    char allMembers[arrSize];
    SDL_memset(allMembers, 0, arrSize);
    for (int i = 0; i < nSessionMemebers; i++)
    {
        NetPlayer *player = ServerGetPlayerByID(SessionGetPlayerIDs(session)[i]);
        char *name = player->name;
        int bytesToCopy = SDL_min(strlen(name), nameSize);
        SDL_memcpy(allMembers + i * nameSize, name, bytesToCopy);
    }

    ServerTCPSend(PT_FetchLobby, allMembers, arrSize, packet.sender);
}

void ServerManagerHandlePlayerHitPacket(ParsedPacket packet)
{
    // If player doesnt actually exist in server player array, discard packet
    NetPlayer *senderP = ServerGetPlayerByID(packet.sender.id);
    if (!senderP)
        return;
    if (senderP->sessionID < 0)
        return;
    Session *session = ServerGetSessionByID(senderP->sessionID);
    if (!session)
        return;

    HitData hitData = *(HitData *)packet.data;

    // Award player with points for hitting a target, relative to the damage dealt
    senderP->pointBuffer += (float)hitData.damage / 40.0f;

    ServerTCPBroadcastSession(PT_PlayerHit, session, &hitData, sizeof(HitData));
}

void ServerManagerHandlePlayerShootPacket(ParsedPacket packet)
{
    // If player doesnt actually exist in server player array, discard packet
    NetPlayer *senderP = ServerGetPlayerByID(packet.sender.id);
    if (!senderP)
        return;
    if (senderP->sessionID < 0)
        return;
    Session *session = ServerGetSessionByID(senderP->sessionID);
    if (!session)
        return;

    ShootData shootData = *(ShootData *)packet.data;
    ServerTCPBroadcastExclusiveSession(PT_PlayerShoot, session, &shootData, sizeof(ShootData), packet.sender);
}

void ServerManagerHandlePlayerDeadPacket(ParsedPacket packet)
{
    // If player doesnt actually exist in server player array, discard packet
    NetPlayer *senderP = ServerGetPlayerByID(packet.sender.id);
    if (!senderP)
        return;
    if (senderP->sessionID < 0)
        return;
    Session *session = ServerGetSessionByID(senderP->sessionID);
    if (!session)
        return;

    if (senderP->state == NPS_Dead)
        return;

    senderP->state = NPS_Dead;

    // Award player with points relative to how many people died before
    senderP->pointBuffer += (float)(session->nDeadPlayers++) * 10.0f;

    ServerTCPBroadcastExclusiveSession(PT_PlayerDead, session, &senderP->id, sizeof(int), packet.sender);
}

void ServerManagerHandleFetchScoreboardPacket(ParsedPacket packet)
{
    // If player doesnt actually exist in server player array, discard packet
    NetPlayer *senderP = ServerGetPlayerByID(packet.sender.id);
    if (!senderP)
        return;
    if (senderP->sessionID < 0)
        return;
    Session *session = ServerGetSessionByID(senderP->sessionID);
    if (!session)
        return;

    // Allocate enough for maximum session players
    ScoreboardEntry allEntries[session->playerIDs->size];

    // Actual added number of players
    int n = 0;
    for (int i = 0; i < session->playerIDs->size; i++)
    {
        ScoreboardEntry entry = {0};
        NetPlayer *player = ServerGetPlayerByID(SessionGetPlayerIDs(session)[i]);
        if (!player)
            continue;
        entry.id = player->id;
        strcpy(entry.name, player->name);
        entry.score = player->pointBuffer;
        allEntries[n++] = entry;
    }

    ServerTCPSend(PT_FetchScoreboard, allEntries, sizeof(ScoreboardEntry) * n, packet.sender);
}