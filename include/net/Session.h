#ifndef SESSION_H
#define SESSIOn_H

#include "../core/Dependencies.h"
#include "../core/Vector.h"
#include "NetPlayer.h"
#include "../Map.h"

typedef struct Session
{
    int id;
    NetPlayer *host;
    // Pointers to players in server
    Vector *playersP;
    char *rawMap;
    char *mapName;
    int mapMaxPlayers;
} Session;

Session SessionCreate(int id, NetPlayer *host, char *rawmap, size_t size);
void SessionDestroy(Session *session);

NetPlayer **SessionGetPlayers(Session *session);

#endif
