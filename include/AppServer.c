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
    UDPServerStart();
    app->isRunning = isRunning;
    app->clock = clock;
    app->displayTimer = 0.0f;
    return app;
}

void AppServerDestroy(AppServer *app)
{
    UDPServerStop();
}

void AppServerGo(AppServer *app)
{
    AppServerUpdate(app);
    AppServerShowPlayerList(app);
}

void AppServerUpdate(AppServer *app)
{
    // LOCK VECTOR
    SDL_LockMutex(udpServer.inBufferMutex);
    for (size_t i = 0; i < udpServer.inBuffer->size; i++)
    {
        ParsedUDPPacket nextPacket = UDPSERVER_INBUFFER[i];

        switch (nextPacket.type)
        {
        case UDPType_Text:
            AppServerHandleTextPacket(nextPacket);
            break;
        case UDPType_PlayerID:
            AppServerHandlePlayerIDPacket(nextPacket);
            break;
        case UDPType_Entity:
            AppServerHandleEntityPacket(nextPacket);
            break;
        case UDPType_CompressedEntity:
            AppServerHandleCompressedEntityPacket(nextPacket);
            break;
        case UDPType_IPaddress:
            AppServerHandleIPaddressPacket(nextPacket);
            break;
        default:
            break;
        }
    }
    for (size_t i = 0; i < udpServer.inBuffer->size; i++)
    {
        ParsedUDPPacketDestroy(&UDPSERVER_INBUFFER[i]);
    }
    VectorClear(udpServer.inBuffer);
    SDL_UnlockMutex(udpServer.inBufferMutex);
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
    for (int i = 0; i < udpServer.players->size; i++)
        printf("%d:(host:port): %d:%d\n", UDPSERVER_PLAYERS[i].id, UDPSERVER_PLAYERS[i].ip.host, UDPSERVER_PLAYERS[i].ip.port);
    printf("Ammount of players %u\n", (unsigned int)udpServer.players->size);
    printf("---- End of player list ----\n\n");
}

void AppServerHandleTextPacket(ParsedUDPPacket packet)
{
    if (!SDL_memcmp(packet.data, "alive", 5))
    {
        for (int i = 0; i < udpServer.players->size; i++)
        {
            if (UDPSERVER_PLAYERS[i].ip.host == packet.sender.host &&
                UDPSERVER_PLAYERS[i].ip.port == packet.sender.port)
            {
                int ID = UDPServerGetID(packet.sender);
                UDPSERVER_PLAYERS[i].id = ID;
                UDPServerSend(UDPType_PlayerID, &ID, sizeof(int), packet.sender);
                log_info("Player(id:%d) connected\n", ID);
            }
        }
        return;
    }

    if (!SDL_memcmp(packet.data, "quit", 4))
    {
        UDPServerRemoveClient(packet.sender);
        return;
    }
}

void AppServerHandlePlayerIDPacket(ParsedUDPPacket packet)
{
}

void AppServerHandleEntityPacket(ParsedUDPPacket packet)
{
    Entity incomingEntity = *(Entity *)packet.data;

    SDL_bool exist = SDL_FALSE;
    for (int i = 0; i < udpServer.players->size; i++)
    {
        if (UDPSERVER_PLAYERS[i].id == incomingEntity.id) //entity exists
        {
            exist = SDL_TRUE;
            UDPServerBroadcastExclusive(UDPType_Entity, &incomingEntity, sizeof(Entity), packet.sender);
            break;
        }
    }
    if (!exist)
    {
#ifdef SERVERMANAGER_DEBUG
        log_warn("Received an entity that did not correspond to any player");
#endif
    }
}
void AppServerHandleCompressedEntityPacket(ParsedUDPPacket packet)
{
    CompressedEntity incomingCEntity = *(CompressedEntity *)packet.data;

    SDL_bool exist = SDL_FALSE;
    for (int i = 0; i < udpServer.players->size; i++)
    {
        if (UDPSERVER_PLAYERS[i].id == incomingCEntity.id) //entity exists
        {
            exist = SDL_TRUE;
            UDPServerBroadcastExclusive(UDPType_CompressedEntity, &incomingCEntity, sizeof(CompressedEntity), packet.sender);
            break;
        }
    }
    if (!exist)
    {
#ifdef SERVERMANAGER_DEBUG
        log_warn("Received an entity that did not correspond to any player");
#endif
    }
}
void AppServerHandleIPaddressPacket(ParsedUDPPacket packet)
{
}