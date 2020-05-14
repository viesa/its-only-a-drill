#include "AppServer.h"

struct AppServer
{
    SDL_bool *isRunning;
    float displayTimer;
    SDL_Thread *cliWorker;
};

AppServer *AppServerCreate(SDL_bool *isRunning)
{
    srand(time(NULL));
    EntityManagerInitialize();
    CLIStateSet(CS_Main);

    AppServer *app = MALLOC(AppServer);
    ServerInitialize();
    ServerStartListening();
    app->isRunning = isRunning;
    app->displayTimer = 0.0f;
    app->cliWorker = SDL_CreateThread((SDL_ThreadFunction)AppServerUpdateCLI, "CLI Update", app);
    return app;
}

void AppServerDestroy(AppServer *app)
{
    ServerTCPBroadcast(PT_CloseAllSessions, NULL, 0);
    ServerStopListening();
    ServerUninitialize();
    EntityManagerUninitalize();
    if (app->cliWorker)
        SDL_WaitThread(app->cliWorker, NULL);
}

void AppServerGo(AppServer *app)
{
    AppServerHandleAllPackets(app);
    AppServerUpdateTimeoutTimers(app);
    AppServerKickTimeoutClients(app);
    AppServerPingClients(app);
}

void AppServerHandleAllPackets(AppServer *app)
{
    SDL_LockMutex(server.inBufferMutex);
    for (size_t i = 0; i < server.inBuffer->size; i++)
    {
        ParsedPacket nextPacket = SERVER_INBUFFER[i];

        switch (nextPacket.type)
        {
        case PT_Text:
            AppServerHandleTextPacket(nextPacket);
            break;
        case PT_AreYouAlive:
            AppServerHandleAreYouAlivePacket(nextPacket);
            break;
        case PT_IAmAlive:
            AppServerHandleIAmAlivePacket(nextPacket);
            break;
        case PT_Connect:
            AppServerHandleConnectPacket(nextPacket);
            break;
        case PT_UDPRespondIP:
            AppServerHandleUDPRespondIPPacket(nextPacket);
            break;
        case PT_Disconnect:
            AppServerHandleDisconnectPacket(nextPacket);
            break;
        case PT_Entity:
            AppServerHandleEntityPacket(nextPacket);
            break;
        case PT_CompressedEntity:
            AppServerHandleCompressedEntityPacket(nextPacket);
            break;
        case PT_CreateSession:
            AppServerHandleCreateSessionPacket(nextPacket);
            break;
        case PT_JoinSession:
            AppServerHandleJoinSessionPacket(nextPacket);
            break;
        case PT_LeaveSession:
            AppServerHandleLeaveSessionPacket(nextPacket);
            break;
        case PT_StartSession:
            AppServerHandleStartSessionPacket(nextPacket);
            break;
        case PT_ChangeSkin:
            AppServerHandleChangeSkinPacket(nextPacket);
            break;
        case PT_FetchSessions:
            AppServerHandleFetchSessionsPacket(nextPacket);
            break;
        case PT_FetchLobby:
            AppServerHandleFetchLobbyPacket(nextPacket);
            break;
        case PT_PlayerHit:
            AppServerHandlePlayerHitPacket(nextPacket);
            break;
        default:
            break;
        }
    }
    for (size_t i = 0; i < server.inBuffer->size; i++)
    {
        ParsedPacketDestroy(&SERVER_INBUFFER[i]);
    }
    VectorClear(server.inBuffer);
    SDL_UnlockMutex(server.inBufferMutex);
}

void AppServerUpdateCLI(AppServer *app)
{
    SDL_SetThreadPriority(SDL_THREAD_PRIORITY_LOW);
    while (server.isListening)
    {
        AppServerDrawCLI(app);

        char input;
        scanf(" %c", &input);
        input = toupper(input);
        switch (CLIStateGet())
        {
        case CS_Main:
        {
            if (input == 'P')
            {
                CLIStateSet(CS_PlayerList);
            }
            if (input == 'S')
            {
                CLIStateSet(CS_SessionList);
            }
            if (input == 'T')
            {
                CLIStateSet(CS_Traffic);
                // Takes the lock so that "(B) Go back" is printed before any traffic
                SDL_LockMutex(server.trafficMutex);
            }
            if (input == 'Q')
            {
                *app->isRunning = SDL_FALSE;
            }
        }
        break;
        case CS_PlayerList:
        {
            if (input == 'R')
            {
            }
            if (input == 'B')
            {
                CLIStateSet(CS_Main);
            }
        }
        break;
        case CS_SessionList:
        {
            if (input == 'R')
            {
            }
            if (input == 'B')
            {
                CLIStateSet(CS_Main);
            }
        }
        break;
        case CS_Traffic:
        {
            if (input == 'B')
            {
                CLIStateSet(CS_Main);
            }
        }
        break;
        default:
            break;
        }
    }
}

void AppServerDrawTitle(AppServer *app)
{
    printf("   ____                    \n");
    printf("  / __/__ _____  _____ ____\n");
    printf(" _\\ \\/ -_) __/ |/ / -_) __/\n");
    printf("/___/\\__/_/  |___/\\__/_/   \n\n");
}

void AppServerDrawCLI(AppServer *app)
{
    AppServerClearTerminal(app);
    AppServerDrawTitle(app);
    switch (CLIStateGet())
    {
    case CS_Main:
    {
        printf("(P) Show Playerlist\n");
        printf("(S) Show Sessionlist\n");
        printf("(T) Show Traffic\n");
        printf("(Q) Quit\n");
    }
    break;
    case CS_PlayerList:
    {
        printf("Number of players: %zu\n\n", server.players->size);
        for (int i = 0; i < server.players->size; i++)
        {
            NetPlayer *pl = &SERVER_PLAYERS[i];
            printf("[ID:%d] %s IP:%d:%d ", pl->id, pl->name, pl->ip->host, pl->ip->port);
            if (pl->sessionID == -1)
                printf("Session:None ");
            else
                printf("Session:%d ", pl->sessionID);
            printf("Timeout:%.3f\n", pl->timeoutTimer);
        }
        if (server.players->size > 0)
            printf("\n");
        printf("(R) Reload\n");
        printf("(B) Go Back\n");
    }
    break;
    case CS_SessionList:
    {
        printf("Number of sessions: %zu\n\n", server.sessions->size);
        for (int i = 0; i < server.sessions->size; i++)
        {
            Session *session = &SERVER_SESSIONS[i];
            NetPlayer *host = ServerGetPlayerByID(session->hostID);
            char *name = host ? host->name : "Unknown";
            printf("[ID:%d] %s [%zu/%d] Host: %s\n",
                   session->id,
                   session->mapInfo.name,
                   session->playerIDs->size,
                   session->mapInfo.maxPlayers,
                   name);
        }
        if (server.sessions->size > 0)
            printf("\n");
        printf("(R) Reload\n");
        printf("(B) Go Back\n");
    }
    break;
    case CS_Traffic:
    {
        printf("(B) Go Back\n");
        SDL_UnlockMutex(server.trafficMutex);
    }
    break;
    default:
        break;
    }
    printf("\n");
}

void AppServerClearTerminal(AppServer *app)
{
#ifdef __linux__
    system("clear");
#endif
#ifdef __APPLE__
    system("clear");
#endif
#ifdef _WIN32
    system("cls");
#endif
}

void AppServerUpdateTimeoutTimers(AppServer *app)
{
    for (int i = 0; i < server.players->size; i++)
    {
        if (SERVER_PLAYERS[i].waitingForAliveReply)
            SERVER_PLAYERS[i].timeoutTimer += ClockGetDeltaTime();
    }
}

void AppServerKickTimeoutClients(AppServer *app)
{
    for (int i = 0; i < server.players->size; i++)
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

void AppServerPingClients(AppServer *app)
{
    for (int i = 0; i < server.players->size; i++)
    {
        if (!SERVER_PLAYERS[i].waitingForAliveReply)
        {
            ServerTCPSend(PT_AreYouAlive, NULL, 0, SERVER_PLAYERS[i]);
            SERVER_PLAYERS[i].waitingForAliveReply = SDL_TRUE;
        }
    }
}

void AppServerHandleTextPacket(ParsedPacket packet)
{
    // Print whatever came in
    printf("Received text-packet: %s", (char *)packet.data);
}

void AppServerHandleAreYouAlivePacket(ParsedPacket packet)
{
    // If player doesnt actually exist in server player array, discard packet
    NetPlayer *senderP = ServerGetPlayerByID(packet.sender.id);
    if (!senderP)
        return;
    ServerTCPSend(PT_IAmAlive, NULL, 0, packet.sender);
}

void AppServerHandleIAmAlivePacket(ParsedPacket packet)
{
    // If player doesnt actually exist in server player array, discard packet
    NetPlayer *senderP = ServerGetPlayerByID(packet.sender.id);
    if (!senderP)
        return;
    senderP->timeoutTimer = 0.0f;
    senderP->waitingForAliveReply = SDL_FALSE;
}

void AppServerHandleConnectPacket(ParsedPacket packet)
{
    // If player doesnt actually exist in server player array, discard packet
    NetPlayer *senderP = ServerGetPlayerByID(packet.sender.id);
    if (!senderP)
        return;
    // If player tried to change name to same name, do nothing (keep old)
    if (!strcmp(senderP->name, (char *)packet.data))
        return;

    for (size_t i = 0; i < server.players->size; i++)
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

void AppServerHandleUDPRespondIPPacket(ParsedPacket packet)
{
    // If player doesnt actually exist in server player array, discard packet
    NetPlayer *senderP = ServerGetPlayerByID(packet.sender.id);
    if (!senderP)
        return;
    IPaddress ip = *(IPaddress *)packet.data;
    senderP->udpRespondIP = ip;
}

void AppServerHandleDisconnectPacket(ParsedPacket packet)
{
    // Try remove the client
    ServerRemoveClient(packet.sender);
}

void AppServerHandleNewPlayerPacket(ParsedPacket packet)
{
}

void AppServerHandleDelPlayerPacket(ParsedPacket packet)
{
}

void AppServerHandleEntityPacket(ParsedPacket packet)
{
    Entity incomingEntity = *(Entity *)packet.data;

    for (int i = 0; i < server.players->size; i++)
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

void AppServerHandleCompressedEntityPacket(ParsedPacket packet)
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

void AppServerHandleCreateSessionPacket(ParsedPacket packet)
{
    char *rawMap = (char *)packet.data;

    // If player doesn't actually exist in server player array, discard packet
    NetPlayer *senderP = ServerGetPlayerByID(packet.sender.id);
    if (!senderP)
        return;

    int sessionID = ServerGetSessionID();

    Session session = SessionCreate(sessionID, senderP, rawMap, packet.size);
    // If session ID is -1, something bad happened when it was created
    if (session.id < 0)
    {
        int badSessionID = -1;
        ServerTCPSend(PT_CreateSession, &badSessionID, sizeof(int), packet.sender);
        SessionDestroy(&session);
    }
    else
    {
        VectorPushBack(server.sessions, &session);
        ServerTCPSend(PT_CreateSession, &session.id, sizeof(int), packet.sender);
    }
}

void AppServerHandleJoinSessionPacket(ParsedPacket packet)
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
        for (size_t i = 0; i < server.sessions->size; i++)
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

void AppServerHandleStartSessionPacket(ParsedPacket packet)
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

    int *playerIDs = SessionGetPlayerIDs(session);
    // Notify client that the session started, and provide client with his entity
    for (int i = 0; i < session->playerIDs->size; i++)
    {
        NetPlayer *to = ServerGetPlayerByID(playerIDs[i]);
        Entity *entity = &to->entity;
        if (i < session->mapInfo.playerSpawns->size)
        {
            entity->position = MapInfoGetPlayerSpawns(&session->mapInfo)[i].position;
        }
        else
        {
            entity->position = Vec2Create(0.0f, 0.0f);
        }
        ServerTCPSend(PT_StartSession, entity, sizeof(Entity), *to);
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
}

void AppServerHandleLeaveSessionPacket(ParsedPacket packet)
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

void AppServerHandleChangeSkinPacket(ParsedPacket packet)
{
    // If player doesnt actually exist in server player array, discard packet
    NetPlayer *senderP = ServerGetPlayerByID(packet.sender.id);
    if (!senderP)
        return;

    int int_spriteSheet = *(int *)packet.data;
    if (int_spriteSheet < 1 || int_spriteSheet > SS_Count)
        return;

    SpriteSheet spriteSheet = (SpriteSheet)int_spriteSheet;

    for (int i = 0; i < MAX_DRAWABLES; i++)
        senderP->entity.drawables[i].spriteSheet = spriteSheet;
}

void AppServerHandleFetchSessionsPacket(ParsedPacket packet)
{
    int nSessions = server.sessions->size;
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

void AppServerHandleFetchLobbyPacket(ParsedPacket packet)
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

void AppServerHandlePlayerHitPacket(ParsedPacket packet)
{
    // If player doesnt actually exist in server player array, discard packet
    NetPlayer *senderP = ServerGetPlayerByID(packet.sender.id);
    if (!senderP)
        return;
    if (senderP->sessionID < 0)
        return;

    struct
    {
        int id;
        int damage;
    } data = {0};

    SDL_memcpy(&data, packet.data, sizeof(int) * 2);

    Session *session = ServerGetSessionByID(senderP->sessionID);
    if (!session)
        return;

    NetPlayer *target = ServerGetPlayerByID(data.id);

    if (target)
        ServerTCPSend(PT_PlayerHit, &data.damage, sizeof(int), *target);
}