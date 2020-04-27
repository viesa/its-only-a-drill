#ifndef CLIENT_H
#define CLIENT_H

#include "../core/Dependencies.h"
#include "../core/Vector.h"
#include "Packager.h"

#define CLIENT_INBUFFER ClientGetInBufferArray()

struct
{
    IPaddress serverIP;
    UDPsocket socket;

    Vector *inBuffer;
    SDL_mutex *inBufferMutex;

    SDL_bool isActive;
    SDL_bool isInitialized;
    SDL_bool receivedPlayerID;

    SDL_Thread *listenThread;

} client;

void ClientInitialize();
void ClientUninitialize();

// Marks client as active and sends out threads
void ClientStart();
// Marks client as inactive and collects threads
void ClientStop();
// Sends a packet to the server
int ClientSend(PacketType type, void *data, size_t size);
// Thread function to listen for server-packets, parse them and put them into inBuffer
void ClientListenToServer();

ParsedPacket *ClientGetInBufferArray();

#endif