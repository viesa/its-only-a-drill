#ifndef SESSION_H
#define SESSIOn_H

#include "Dependencies.h"
#include "Vector.h"
#include "NetPlayer.h"
#include "Map.h"

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
} Session;

Session SessionCreate(int id, NetPlayer *host, char *rawmap, size_t size);
void SessionDestroy(Session *session);

int *SessionGetPlayerIDs(Session *session);

#endif
