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
    Map map;
} Session;

Session SessionCreate(int id, NetPlayer *host, char *rawmap, size_t size);
void SessionDestroy(Session *session);

void SessionAddPlayer(Session *session, NetPlayer *player);
void SessionRemovePlayer(Session *session, NetPlayer *player);

NetPlayer **SessionGetPlayers(Session *session);

#endif
