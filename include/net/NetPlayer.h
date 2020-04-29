#ifndef NETPLAYER_H
#define NETPLAYER_H

#include "../core/Dependencies.h"
#include "../core/Entity.h"

typedef struct NetPlayer
{
    TCPsocket socket;
    // A pointer to the IPaddress of specified NetPlayer's TCP-socket
    IPaddress *ip;
    int id;
    // Only used to store the player's entity on server side
    // If you want this NetPlayer's ID, refer to the integer "id" instead
    Entity entity;
} NetPlayer;

NetPlayer NetPlayerCreate(TCPsocket socket, int id);

#endif