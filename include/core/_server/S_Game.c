#include "S_Game.h"

#include "../../../_server/S_Resources.h"

struct S_Game
{
    S_Resources *m_resources;
    Clock *m_clock;
    Input *m_input;
    Server *m_server;
    NetworkMgr *m_netMgr;
    void *m_attributes[GAttrib_Count];
};

S_Game *S_GameCreate(Clock *clock, SDL_bool *running, Input *input, Server *server)
{
    S_Game *ret = (S_Game *)SDL_malloc(sizeof(S_Game));
    ret->m_resources = (S_Resources *)SDL_malloc(sizeof(S_Resources));
    ret->m_clock = clock;
    ret->m_input = input;
    ret->m_server = server;
    ret->m_netMgr = NetworkMgrCreate();
    NetworkMgrAddServer(ret->m_netMgr, ret->m_server);
    ret->m_attributes[GAttrib_Game] = ret;
    ret->m_attributes[GAttrib_Resources] = ret->m_resources;
    ret->m_attributes[GAttrib_Clock] = ret->m_clock;
    ret->m_attributes[GAttrib_Input] = ret->m_input;
    ret->m_attributes[GAttrib_Server] = ret->m_server;
    S_Init(ret->m_attributes);
    return ret;
}
void S_GameDestroy(S_Game *game)
{
    S_Exit(game->m_attributes);
    NetworkMgrDestroy(game->m_netMgr);
    SDL_free(game);
}

void S_GameRun(S_Game *game)
{
    S_GameUpdateComponents(game);
    S_Logic(game->m_attributes);
}

void S_GameUpdateComponents(S_Game *game)
{
    NetworkMgrPollAll(game->m_netMgr);
}