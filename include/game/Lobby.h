#ifndef LOBBY_H
#define LOBBY_H

#include "Vector.h"

#define LOBBY_NAMES LobbyGetNames()

typedef struct LobbyName
{
    char data[MAX_PLAYERNAME_SIZE];
} LobbyName;

void LobbyInitialize();
void LobbyUninitialize();

// Removes all the stored name in the lobby
void LobbyClearNames();
// Appends a name to the stored lobby names
void LobbyAddName(char *name);

LobbyName *LobbyGetNames();
// Returns the number of stored lobby names
size_t LobbyGetNumNames();
// Returns true if player is host of lobby
SDL_bool LobbyIsHost();
// Set if player is host over lobby
void LobbySetIsHost(SDL_bool isHost);

#endif