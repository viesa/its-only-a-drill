#ifndef NETPLAYER_H
#define NETPLAYER_H

#include "../core/Dependencies.h"

typedef struct NetPlayer
{
    TCPsocket socket;
    IPaddress ip;
    int id;
} NetPlayer;

NetPlayer NetPlayerCreate(TCPsocket socket, IPaddress ip, int id);

#endif