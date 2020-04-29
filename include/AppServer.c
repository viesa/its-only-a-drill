#include "core/AppServer.h"

struct AppServer
{
    SDL_bool *isRunning;
    Clock *clock;
    float displayTimer;
};

AppServer *AppServerCreate(SDL_bool *isRunning, Clock *clock)
{
    AppServer *app = MALLOC(AppServer);
    ServerStart();
    app->isRunning = isRunning;
    app->clock = clock;
    app->displayTimer = 0.0f;
    return app;
}

void AppServerDestroy(AppServer *app)
{
    ServerStop();
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

        void AppServerHandleTextPacket(ParsedPacket packet);
        void AppServerHandleConnectPacket(ParsedPacket packet);
        void AppServerHandleDisconnectPacket(ParsedPacket packet);
        void AppServerHandleNewPlayerPacket(ParsedPacket packet);
        void AppServerHandleDelPlayerPacket(ParsedPacket packet);
        void AppServerHandleEntityPacket(ParsedPacket packet);
        void AppServerHandleCompressedEntityPacket(ParsedPacket packet);
        switch (nextPacket.type)
        {
        case PT_Text:
            AppServerHandleTextPacket(nextPacket);
            break;
        case PT_Connect:
            AppServerHandleEntityPacket(nextPacket);
            break;
        case PT_Disconnect:
            AppServerHandleDisconnectPacket(nextPacket);
            break;
        case PT_NewPlayer:
            AppServerHandleNewPlayerPacket(nextPacket);
            break;
        case PT_DelPlayer:
            AppServerHandleDelPlayerPacket(nextPacket);
            break;
        case PT_Entity:
            AppServerHandleEntityPacket(nextPacket);
            break;
        case PT_CompressedEntity:
            AppServerHandleCompressedEntityPacket(nextPacket);
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
        app->displayTimer += ClockGetDeltaTime(app->clock);
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
        printf("%d:(host:port): %d:%d\n", SERVER_PLAYERS[i].id, SERVER_PLAYERS[i].ip->host, SERVER_PLAYERS[i].ip->port);
    printf("Ammount of players %u\n", (unsigned int)server.players->size);
    printf("---- End of player list ----\n\n");
}

void AppServerHandleTextPacket(ParsedPacket packet)
{
}

void AppServerHandleConnectPacket(ParsedPacket packet)
{
    //Add name?
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