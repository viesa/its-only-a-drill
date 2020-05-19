#ifndef SESSION_H
#define SESSION_H

#include "Dependencies.h"
#include "Vector.h"
#include "NetPlayer.h"
#include "Map.h"

#define SURVIVING_POINTS (10.0f)

typedef struct Session
{
    int id;
    int hostID;
    // Vector of Player IDs
    Vector *playerIDs;
    char *rawMap;
    size_t rawMapDataSize;
    MapInfo mapInfo;
    SDL_bool inGame;

    // Used to multiply point in player survining longer
    int nDeadPlayers;

    int nRounds;
    int currentRound;

    SDL_bool startingNewRound;
    SDL_bool quittingMatch;
    float finishedRoundCountdown;
    float finishedMatchCountdown;
} Session;

Session SessionCreate(int id, int nRounds, NetPlayer *host, char *rawmap, size_t size);
void SessionDestroy(Session *session);

int *SessionGetPlayerIDs(Session *session);

void SessionStartNewRound(Session *session);

#endif
