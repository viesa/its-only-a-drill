#ifndef SOCKETWRAPPER_H
#define SOCKETWRAPPER_H
#define BUFFER_LENGTH 1024
#include "SDL2/SDL_net.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct SocketClient{
    IPaddress ip;
    TCPsocket sock;
}SocketClient;
SocketClient SocketClientInit(char host[], int port, SDL_bool isServer);
int SocketClientMSG(SocketClient client, char msg[]);
int SocketClientOpenPort(SocketClient *client);
int SocketClientListen(SocketClient server, char buffer[]);
#endif