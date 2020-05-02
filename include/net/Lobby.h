#ifndef LOBBY_H
#define LOBBY_H

#include "../core/Vector.h"

#define LOBBY_NAMES LobbyGetNamesArray()

typedef struct LobbyName
{
    char data[MAX_PLAYERNAME_SIZE];
} LobbyName;

struct Lobby
{
    int sessionID;
    // Vector of LobbyName
    Vector *names;
    SDL_bool isHost;
} lobby;

void LobbyInitialize();
void LobbyUninitialize();

LobbyName *LobbyGetNamesArray();

#endif