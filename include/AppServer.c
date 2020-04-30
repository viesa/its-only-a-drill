#include "core/AppServer.h"

struct AppServer
{
    SDL_bool *isRunning;
    float displayTimer;
};

AppServer *AppServerCreate(SDL_bool *isRunning)
{
    EntityManagerInitialize();

    AppServer *app = MALLOC(AppServer);
    ServerStart();
    app->isRunning = isRunning;
    app->displayTimer = 0.0f;
    return app;
}

void AppServerDestroy(AppServer *app)
{
    ServerStop();
    EntityManagerUninitalize();
}

void AppServerGo(AppServer *app)
{
    AppServerUpdate(app);
    AppServerShowPlayerList(app);
}

void AppServerUpdate(AppServer *app)
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
void AppServerShowPlayerList(AppServer *app)
{
    if (app->displayTimer > 1.0f)
    {
        app->displayTimer -= 1.0f;
    }
    else
    {
        app->displayTimer += ClockGetDeltaTime();
        return;
    }

    // #ifdef __linux__
    //     system("clear");
    // #endif
    // #ifdef __APPLE__
    //     system("clear");
    // #endif
    // #ifdef _WIN32
    //     system("cls");
    // #endif

    printf("\n---- Player List ----\n");
    for (int i = 0; i < server.players->size; i++)
        printf("%s [%d]:(host:port): %d:%d\n", SERVER_PLAYERS[i].name, SERVER_PLAYERS[i].id, SERVER_PLAYERS[i].ip->host, SERVER_PLAYERS[i].ip->port);
    printf("Ammount of players %u\n", (unsigned int)server.players->size);
    printf("---- End of player list ----\n\n");
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

    NetPlayer *playerP = NULL;
    for (size_t i = 0; i < server.players->size; i++)
    {
        if (!SDL_memcmp(&SERVER_PLAYERS[i], &packet.sender, sizeof(NetPlayer)))
        {
            playerP = &SERVER_PLAYERS[i];
            break;
        }
    }

    int sessionID = ServerGetSessionID();

    Session session = SessionCreate(sessionID, playerP, rawMap, packet.size);
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
    NetPlayer *sender = AppServerNetPlayerToPointer(packet.sender);
    for (size_t i = 0; i < server.sessions->size; i++)
    {
        // Find correct session
        if (SERVER_SESSIONS[i].id == sessionID)
        {

            if (SERVER_SESSIONS[i].mapMaxPlayers < SERVER_SESSIONS[i].playersP->size)
            {
                // If there is no host, you are host
                if (SERVER_SESSIONS[i].host == NULL)
                    SERVER_SESSIONS[i].host = sender;
                //Adds new player to session
                VectorPushBack(SERVER_SESSIONS[i].playersP, sender);
                // Notifies the host, that they are host
                ServerTCPSend(PT_HostAssignSession, &SERVER_SESSIONS[i].host->id, sizeof(int), *sender);
            }
            else if (SERVER_SESSIONS[i].mapMaxPlayers > SERVER_SESSIONS[i].playersP->size)
            {
                ServerTCPSend(PT_FullSession, &SERVER_SESSIONS[i].id, sizeof(int), packet.sender);
            }
        }
    }
}
void AppServerHandleLeaveSessionPacket(ParsedPacket packet)
{
    int sessionID = *(int *)packet.data;
    NetPlayer *sender = AppServerNetPlayerToPointer(packet.sender);
    for (size_t i = 0; i < server.sessions->size; i++)
    {
        // Find correct session
        if (SERVER_SESSIONS[i].id == sessionID)
        {
            // Remove leaving player from session
            VectorEraseElement(SERVER_SESSIONS[i].playersP, sender);

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
}
