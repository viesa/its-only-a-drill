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
        case PT_Connect:
            AppServerHandleConnectPacket(nextPacket);
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
            printf("[ID:%d] %s IP: %d:%d\n", SERVER_PLAYERS[i].id, SERVER_PLAYERS[i].name, SERVER_PLAYERS[i].ip->host, SERVER_PLAYERS[i].ip->port);
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
            printf("[ID:%d] %s [%zu/%d]\n", session->id, session->mapName, session->playersP->size, session->mapMaxPlayers);
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
Session *ServerGetSessionByID(int sessionID)
{
    for (size_t i = 0; i < server.sessions->size; i++)
    {
        if (SERVER_SESSIONS[i].id == sessionID)
        {
            return &SERVER_SESSIONS[i];
        }
    }
    return NULL;
}

void AppServerHandleTextPacket(ParsedPacket packet)
{
}

void AppServerHandleConnectPacket(ParsedPacket packet)
{
    for (size_t i = 0; i < server.players->size; i++)
    {
        if (!strcmp(SERVER_PLAYERS[i].name, (char *)packet.data))
        {
            ServerTCPSend(PT_DuplicateName, NULL, 0, packet.sender);
            return;
        }
    }
    NetPlayer *playerP = ServerNetPlayerToPointer(packet.sender);
    strcpy(playerP->name, packet.data);
}

void AppServerHandleDisconnectPacket(ParsedPacket packet)
{
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
    CompressedEntity incomingCEntity = *(CompressedEntity *)packet.data;

    for (int i = 0; i < server.players->size; i++)
    {
        if (SERVER_PLAYERS[i].entity.id == packet.sender.entity.id)
        {
            ServerUDPBroadcastExclusive(PT_CompressedEntity, &incomingCEntity, sizeof(CompressedEntity), packet.sender);
            return;
        }
    }

    // THIS SHOULD NEVER HAPPEN
#ifdef SERVERMANAGER_DEBUG
    log_warn("Received an compressed entity that did not correspond to any player");
#endif
}

void AppServerHandleCreateSessionPacket(ParsedPacket packet)
{
    char *rawMap = (char *)packet.data;

    // If player doesnt acutally exist in server player array, discard packet
    NetPlayer *senderP = ServerNetPlayerToPointer(packet.sender);
    if (!senderP)
        return;

    int sessionID = ServerGetSessionID();

    Session session = SessionCreate(sessionID, senderP, rawMap, packet.size);
    // If session ID is -1, something bad happened when it was created
    if (session.id == -1)
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
    // If player doesnt acutally exist in server player array, discard packet
    NetPlayer *senderP = ServerNetPlayerToPointer(packet.sender);
    if (!senderP)
        return;

    // Check if player is already in another session, if so, disconnect it from the other session
    if (senderP->sessionID != -1)
    {
        for (size_t i = 0; i < server.sessions->size; i++)
        {
            // Find correct session
            if (SERVER_SESSIONS[i].id == senderP->sessionID)
            {
                // Erase client from that session
                VectorEraseElement(SERVER_SESSIONS[i].playersP, senderP);
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

    // Checks if there is room for player in the new session
    if (session->playersP->size < session->mapMaxPlayers)
    {
        if (session->host == NULL)
        {
            // If there is no host, you are host
            session->host = senderP;
        }
        if (senderP->id == session->host->id)
        {
            // Notifies client that they are host
            ServerTCPSend(PT_HostAssign, NULL, 0, packet.sender);
        }
        // Makes sure client isn't already in this session somehow
        if (VectorFind(session->playersP, &senderP) == session->playersP->size)
        {
            // Adds new player to session
            VectorPushBack(session->playersP, &senderP);
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
    // If player doesnt acutally exist in server player array, discard packet
    NetPlayer *senderP = ServerNetPlayerToPointer(packet.sender);
    if (!senderP)
        return;
    if (senderP->sessionID == -1)
        return;

    // Make sure player is actually host over the session
    Session *session = ServerGetSessionByID(senderP->sessionID);
    if (senderP != session->host)
        return;

    Vec2 base = Vec2Create(1000.0f, 1000.0f);
    for (int i = 0; i < session->playersP->size; i++)
    {
        NetPlayer *to = SessionGetPlayers(session)[i];
        Entity *entity = &to->entity;
        entity->position = Vec2AddL(base, rand() % 500);
        ServerTCPSend(PT_StartSession, entity, sizeof(Entity), *to);
    }
    session->inGame = SDL_TRUE;
}

void AppServerHandleLeaveSessionPacket(ParsedPacket packet)
{
    // If player doesnt acutally exist in server player array, discard packet
    NetPlayer *senderP = ServerNetPlayerToPointer(packet.sender);
    if (!senderP)
        return;
    if (senderP->sessionID == -1)
        return;

    Session *session = ServerGetSessionByID(senderP->sessionID);

    // Remove leaving player from session
    VectorEraseElement(session->playersP, &senderP);

    // If number of players in session is 0, remove session
    if (session->playersP->size == 0)
    {
        VectorEraseElement(server.sessions, session);
    }
    // Check if leaving player was host
    else if (session->host->id == packet.sender.id)
    {
        // Randomize new host
        int r = rand() % session->playersP->size;
        session->host = SessionGetPlayers(session)[r];
        // Notifes new host that they are host
        ServerTCPSend(PT_HostAssign, NULL, 0, *session->host);
    }

    // Sets the sessionID of player to -1, so that they can join a new session
    senderP->sessionID = -1;
}

void AppServerHandleFetchSessionsPacket(ParsedPacket packet)
{
    int nSessions = server.sessions->size;
    JoinableSession outgoing[nSessions];

    for (int i = 0; i < nSessions; i++)
    {
        Session *current = &SERVER_SESSIONS[i];
        SDL_memset(outgoing[i].name, 0, 20);
        if (strlen(current->mapName) > 16)
        {
            SDL_memcpy(outgoing[i].name, current->mapName, 16);
            strcpy(outgoing[i].name + 16, "...");
        }
        else
        {
            strcpy(outgoing[i].name, current->mapName);
        }
        outgoing[i].maxPlayers = current->mapMaxPlayers;
        outgoing[i].currentPlayers = current->playersP->size;
        outgoing[i].sessionID = current->id;
    }
    ServerTCPSend(PT_FetchSessions, outgoing, sizeof(outgoing), packet.sender);
}

void AppServerHandleFetchLobbyPacket(ParsedPacket packet)
{
    // If player doesnt acutally exist in server player array, discard packet
    NetPlayer *senderP = ServerNetPlayerToPointer(packet.sender);
    if (!senderP)
        return;
    if (senderP->sessionID == -1)
        return;

    Session *session = ServerGetSessionByID(senderP->sessionID);

    const int nSessionMemebers = session->playersP->size;
    const int nameSize = 20;
    const int arrSize = nSessionMemebers * nameSize;
    char allMembers[arrSize];
    SDL_memset(allMembers, 0, arrSize);
    for (int i = 0; i < nSessionMemebers; i++)
    {
        NetPlayer *p = SessionGetPlayers(session)[i];
        char *name = p->name;
        int bytesToCopy = SDL_min(strlen(name), nameSize);
        SDL_memcpy(allMembers + i * nameSize, name, bytesToCopy);
    }

    ServerTCPSend(PT_FetchLobby, allMembers, arrSize, packet.sender);
}

void AppServerHandlePlayerHitPacket(ParsedPacket packet)
{
    // If player doesnt acutally exist in server player array, discard packet
    NetPlayer *senderP = ServerNetPlayerToPointer(packet.sender);
    if (!senderP)
        return;
    if (senderP->sessionID == -1)
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
    NetPlayer **sessionPlayers = SessionGetPlayers(session);

    NetPlayer *target = NULL;
    for (int i = 0; i < session->playersP->size; i++)
    {
        if (sessionPlayers[i]->id == data.id)
        {
            target = sessionPlayers[i];
        }
    }
    if (target)
        ServerTCPSend(PT_PlayerHit, &data.damage, sizeof(int), *target);
}