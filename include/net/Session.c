#include "Session.h"

Session SessionCreate(int id, int nRounds, NetPlayer *host, char *rawmap, size_t size)
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
    session.inGame = SDL_FALSE;
    session.nDeadPlayers = 0;
    session.nRounds = nRounds;
    session.currentRound = 0;
    session.startingNewRound = SDL_FALSE;
    session.quittingMatch = SDL_FALSE;
    session.finishedRoundCountdown = 0.0f;
    session.finishedMatchCountdown = 0.0f;

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

void SessionResetForNewRound(Session *session)
{
    // AppServer will take care of "currentRound >= nRounds" (finished game)
    session->currentRound++;
    session->nDeadPlayers = 0;
}