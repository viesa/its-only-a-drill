#include "Lobby.h"

struct Lobby
{
    // Vector of LobbyName
    Vector *names;
    SDL_bool isHost;
} lobby;

void LobbyInitialize()
{
    lobby.names = VectorCreate(sizeof(LobbyName), 10);
    lobby.isHost = SDL_FALSE;
}

void LobbyUninitialize()
{
    VectorDestroy(lobby.names);
}

void LobbyClearNames()
{
    VectorClear(lobby.names);
}

void LobbyAddName(char *name)
{
    int toCopy = strlen(name) < MAX_PLAYERNAME_SIZE ? strlen(name) : MAX_PLAYERNAME_SIZE;

    LobbyName _new = {0};
    SDL_memcpy(_new.data, name, toCopy);
    VectorPushBack(lobby.names, &_new);
}

LobbyName *LobbyGetNames()
{
    return (LobbyName *)lobby.names->data;
}

size_t LobbyGetNumNames()
{
    return lobby.names->size;
}

SDL_bool LobbyIsHost()
{
    return lobby.isHost;
}

void LobbySetIsHost(SDL_bool isHost)
{
    lobby.isHost = isHost;
}