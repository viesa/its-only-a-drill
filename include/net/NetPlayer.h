#ifndef NETPLAYER_H
#define NETPLAYER_H

#include "Dependencies.h"
#include "Entity.h"

typedef struct NetPlayer
{
    TCPsocket socket;
    // A pointer to the IPaddress of specified NetPlayer's TCP-socket
    IPaddress *ip;
    // An IP address to assign outgoing UDP-packets
    // This IP is set once a UDP-packet is received and the sender data can be extracted
    IPaddress udpRespondIP;
    int id;
    int sessionID;
    // Only used to store the player's entity on server side
    // If you want this NetPlayer's ID, refer to the integer "id" instead
    Entity entity;

    char name[MAX_PLAYERNAME_SIZE];

    float timeoutTimer;
    SDL_bool waitingForAliveReply;
} NetPlayer;

NetPlayer NetPlayerCreate(TCPsocket socket, int id);

#endif