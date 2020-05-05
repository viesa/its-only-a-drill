#ifndef SESSION_H
#define SESSIOn_H

#include "Dependencies.h"
#include "Vector.h"
#include "NetPlayer.h"
#include "Map.h"

typedef struct Session
{
    int id;
    NetPlayer *host;
    // Pointers to players in server
    Vector *playersP;
    char *rawMap;
    size_t rawMapDataSize;
    char *mapName;
    int mapMaxPlayers;
    SDL_bool inGame;
} Session;

Session SessionCreate(int id, NetPlayer *host, char *rawmap, size_t size);
void SessionDestroy(Session *session);

NetPlayer **SessionGetPlayers(Session *session);

#endif
