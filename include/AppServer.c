#include "core/AppServer.h"

struct AppServer
{
    SDL_bool *isRunning;
    float displayTimer;
    SDL_Thread *cliWorker;
};

AppServer *AppServerCreate(SDL_bool *isRunning)
{
    EntityManagerInitialize();
    CLIStateSet(CS_Main);

    AppServer *app = MALLOC(AppServer);
    ServerStart();
    app->isRunning = isRunning;
    app->displayTimer = 0.0f;
    app->cliWorker = SDL_CreateThread((SDL_ThreadFunction)AppServerUpdateCLI, "CLI Update", app);
    return app;
}

void AppServerDestroy(AppServer *app)
{
    ServerStop();
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
        case PT_FetchSessions:
            AppServerHandleFetchSessionsPacket(nextPacket);
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
    while (server.isActive)
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

NetPlayer *AppServerNetPlayerToPointer(NetPlayer player)
{
    for (size_t i = 0; i < server.players->size; i++)
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
void AppServerHandleTextPacket(ParsedPacket packet)
{
}

void AppServerHandleConnectPacket(ParsedPacket packet)
{
    NetPlayer *playerP = AppServerNetPlayerToPointer(packet.sender);
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

    NetPlayer *senderP = AppServerNetPlayerToPointer(packet.sender);
    // If player doesnt acutally exist in server player array, discard packet
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
    NetPlayer *senderP = AppServerNetPlayerToPointer(packet.sender);
    // If player doesnt acutally exist in server player array, discard packet
    if (!senderP)
        return;

    // Check if player is already in another session, if so, disconnect it from the other session
    if (packet.sender.sessionID != -1)
    {
        SDL_bool erased = SDL_FALSE;
        for (size_t i = 0; i < server.sessions->size; i++)
        {
            // Find correct session
            if (SERVER_SESSIONS[i].id == packet.sender.sessionID)
            {
                // Erase client from that session
                VectorEraseElement(SERVER_SESSIONS[i].playersP, senderP);
                erased = SDL_TRUE;
            }
        }
        // Client had invalid session ID assigned to it, somehow...
        if (!erased)
            senderP->sessionID = -1;
    }

    for (size_t i = 0; i < server.sessions->size; i++)
    {
        // Find correct session
        if (SERVER_SESSIONS[i].id == sessionID)
        {
            Session *session = &SERVER_SESSIONS[i];
            // Checks if there is room for player in the new session
            if (session->playersP->size < session->mapMaxPlayers)
            {
                // If there is no host, you are host
                if (session->host == NULL)
                {
                    session->host = senderP;
                    // Notifies client the he is host over the session
                    ServerTCPSend(PT_HostAssignSession, &session->host->id, sizeof(int), packet.sender);
                }
                // Makes sure client isn't already in this session somehow
                if (VectorFind(session->playersP, senderP) == session->playersP->size)
                {
                    // Adds new player to session
                    VectorPushBack(session->playersP, senderP);
                }
                senderP->sessionID = sessionID;
                // Sends the session's map to client
                ServerTCPSend(PT_JoinSession, session->rawMap, session->rawMapDataSize, packet.sender);
            }
            // If there is no room for client, send back FullSesssion packet
            else
            {
                ServerTCPSend(PT_FullSession, &session->id, sizeof(int), packet.sender);
            }
        }
    }
}
void AppServerHandleLeaveSessionPacket(ParsedPacket packet)
{
    int sessionID = *(int *)packet.data;
    NetPlayer *senderP = AppServerNetPlayerToPointer(packet.sender);
    // If player doesnt acutally exist in server player array, discard packet
    if (!senderP)
        return;

    for (size_t i = 0; i < server.sessions->size; i++)
    {
        // Find correct session
        if (SERVER_SESSIONS[i].id == sessionID)
        {
            // Remove leaving player from session
            VectorEraseElement(SERVER_SESSIONS[i].playersP, senderP);

            // If sessions player size is 0, remove session
            if (SERVER_SESSIONS[i].playersP->size == 0)
            {
                VectorEraseElement(server.sessions, &SERVER_SESSIONS[i]);
            }
            // Check if leaving player was host
            else if (SERVER_SESSIONS[i].host->id == packet.sender.id)
            {
                // Randomize new host
                int r = rand() % SERVER_SESSIONS[i].playersP->size;
                SERVER_SESSIONS[i].host = SessionGetPlayers(&SERVER_SESSIONS[i])[r];
                // Notifes new host that they are host
                ServerTCPBroadcastSession(PT_HostAssignSession, &SERVER_SESSIONS[i], &SERVER_SESSIONS[i].host->id, sizeof(int));
            }
        }
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