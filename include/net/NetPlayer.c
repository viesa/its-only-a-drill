#include "NetPlayer.h"

NetPlayer NetPlayerCreate(TCPsocket socket, int id)
{
    NetPlayer netPlayer;
    netPlayer.socket = socket;
    if (socket)
        netPlayer.ip = SDLNet_TCP_GetPeerAddress(socket);
    netPlayer.udpRespondIP = (IPaddress){0u, 0u};
    netPlayer.sessionID = -1;
    netPlayer.id = id;
    SDL_memset(netPlayer.name, 0, MAX_PLAYERNAME_SIZE);
    strcpy(netPlayer.name, "Unnamed");
    netPlayer.spriteSheet = SS_Character_Prisoner;
    netPlayer.timeoutTimer = 0.0f;
    netPlayer.waitingForAliveReply = SDL_FALSE;
    netPlayer.pointBuffer = 0.0f;
    netPlayer.state = NPS_None;
    return netPlayer;
}
