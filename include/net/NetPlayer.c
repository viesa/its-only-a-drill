#include "NetPlayer.h"

NetPlayer NetPlayerCreate(TCPsocket socket, int id)
{
    NetPlayer netPlayer;
    netPlayer.socket = socket;
    if (socket)
        netPlayer.ip = SDLNet_TCP_GetPeerAddress(socket);
    netPlayer.sessionID = -1;
    netPlayer.id = id;
    SDL_memset(netPlayer.name, 0, MAX_PLAYERNAME_SIZE);
    strcpy(netPlayer.name, "Unnamed");
    return netPlayer;
}