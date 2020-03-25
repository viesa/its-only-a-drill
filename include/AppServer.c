#include "core/AppServer.h"

struct AppServer
{
    Clock *clock;
    Input *input;
    Server *server;
    NetworkMgr *netMgr;
};

AppServer *AppServerCreate(Clock *clock, SDL_bool *running, Input *input, Server *server)
{
    AppServer *app = (AppServer *)SDL_malloc(sizeof(AppServer));
    app->clock = clock;
    app->input = input;
    app->server = server;
    app->netMgr = NetworkMgrCreate();
    NetworkMgrAddServer(app->netMgr, app->server);

    ServerSetPort(server, 1337);
    ServerStart(server);
    return app;
}
void AppServerDestroy(AppServer *app)
{
    NetworkMgrDestroy(app->netMgr);
    SDL_free(app);
}

void AppServerRun(AppServer *app)
{
    AppServerUpdate(app);
}

void AppServerUpdate(AppServer *app)
{
    NetworkMgrPollAll(app->netMgr);

    SDL_Delay(100);
    ServerBroadcast(app->server, Test, "ALSO A TEST", 12);
}