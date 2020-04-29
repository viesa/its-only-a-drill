#include "Session.h"

Session SessionCreate(int id, NetPlayer *host, char *rawmap, size_t size)
{
    Session session;
    session.id = id;
    session.host = host;
    session.playersP = VectorCreate(sizeof(NetPlayer *), 10);
    VectorPushBack(session.playersP, session.host);
    session.rawMap = MALLOC_N(char, size);
    ALLOC_ERROR_CHECK(session.rawMap);
    SDL_memcpy(session.rawMap, rawmap, size);
    JSON *json = JSONCreateFromArray(session.rawMap, size);
    session.map = MapCreate(json);
    return session;
}

void SessionDestroy(Session *session)
{
    MapDestroy(&session->map);
    FREE(session->rawMap);
}

void SessionAddPlayer(Session *session, NetPlayer *player)
{
    VectorPushBack(session->playersP, player);
}

void SessionRemovePlayer(Session *session, NetPlayer *player)
{
    VectorEraseElement(session->playersP, player);
}

NetPlayer **SessionGetPlayers(Session *session)
{
    return (NetPlayer **)session->playersP->data;
}
