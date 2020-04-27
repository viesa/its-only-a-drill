#include "NetPlayer.h"

NetPlayer NetPlayerCreate(TCPsocket socket, IPaddress ip, int id)
{
    NetPlayer netPlayer;
    netPlayer.socket = socket;
    netPlayer.ip = ip;
    netPlayer.id = id;
    return netPlayer;
}