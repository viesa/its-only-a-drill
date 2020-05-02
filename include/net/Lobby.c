#include "Lobby.h"

void LobbyInitialize()
{
    lobby.sessionID = -1;
    lobby.names = VectorCreate(sizeof(LobbyName), 10);
    lobby.isHost = SDL_FALSE;
}

void LobbyUninitialize()
{
    VectorDestroy(lobby.names);
}

LobbyName *LobbyGetNamesArray()
{
    return (LobbyName *)lobby.names->data;
}