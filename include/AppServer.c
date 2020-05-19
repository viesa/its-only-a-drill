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
    EntityManagerUninitialize();
    if (app->cliWorker)
        SDL_WaitThread(app->cliWorker, NULL);
}

void AppServerGo(AppServer *app)
{
    ServerManagerHandleAllPackets(app);
    ServerUpdateTimeoutTimers(app);
    ServerManagerKickTimeoutClients(app);
    ServerPingClients(app);
    ServerManagerAdvanceSessionsWithOnePlayerAlive();
}

void AppServerUpdateCLI(AppServer *app)
{
    SDL_SetThreadPriority(SDL_THREAD_PRIORITY_LOW);
    while (ServerIsListening())
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
                SDL_LockMutex(ServerGetTrafficMutex());
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
        printf("Number of players: %zu\n\n", ServerGetNumPlayers());
        for (int i = 0; i < ServerGetNumPlayers(); i++)
        {
            NetPlayer *pl = &SERVER_PLAYERS[i];
            printf("[ID:%d] %s IP:%d:%d ", pl->id, pl->name, pl->ip->host, pl->ip->port);
            if (pl->sessionID == -1)
                printf("Session:None ");
            else
                printf("Session:%d ", pl->sessionID);
            printf("Timeout:%.3f\n", pl->timeoutTimer);
        }
        if (ServerGetNumPlayers() > 0)
            printf("\n");
        printf("(R) Reload\n");
        printf("(B) Go Back\n");
    }
    break;
    case CS_SessionList:
    {
        printf("Number of sessions: %zu\n\n", ServerGetNumSessions());
        for (int i = 0; i < ServerGetNumSessions(); i++)
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
        if (ServerGetNumSessions() > 0)
            printf("\n");
        printf("(R) Reload\n");
        printf("(B) Go Back\n");
    }
    break;
    case CS_Traffic:
    {
        printf("(B) Go Back\n");
        SDL_UnlockMutex(ServerGetTrafficMutex());
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
