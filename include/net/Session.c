#include "Session.h"

Session SessionCreate(int id, NetPlayer *host, char *rawmap, size_t size)
{
    Session badSession;
    badSession.id = -1;

    Session session;
    session.inGame = SDL_FALSE;
    session.id = id;
    session.hostID = host->id;
    session.playerIDs = VectorCreate(sizeof(int), 10);
    VectorPushBack(session.playerIDs, &session.hostID);
    session.rawMap = MALLOC_N(char, size);
    ALLOC_ERROR_CHECK(session.rawMap);
    session.rawMapDataSize = size;
    SDL_memcpy(session.rawMap, rawmap, size);

    JSON *json = JSONCreateFromArray(session.rawMap, size);

    session.mapInfo = MapInfoCreateFromJSON(json);
    if (session.mapInfo.uid == -1)
    {
        badSession.mapInfo = session.mapInfo;
        return badSession;
    }

    JSONDestroy(json);
    return session;
}

void SessionDestroy(Session *session)
{
    if (session->playerIDs)
        VectorDestroy(session->playerIDs);
    FREE(session->rawMap);
    MapInfoDestroy(&session->mapInfo);
}

int *SessionGetPlayerIDs(Session *session)
{
    return (int *)session->playerIDs->data;
}
