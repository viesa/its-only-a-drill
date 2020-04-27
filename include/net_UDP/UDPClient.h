#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include "../core/Dependencies.h"
#include "../core/Vector.h"
#include "UDPPackager.h"

#define UDPCLIENT_INBUFFER UDPClientGetInBufferArray()

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

} udpClient;

void UDPClientInitialize();
void UDPClientUninitialize();

// Marks client as active and sends out threads
void UDPClientStart();
// Marks client as inactive and collects threads
void UDPClientStop();
// Sends a packet to the server
int UDPClientSend(UDPPacketType type, void *data, size_t size);
// Thread function to listen for server-packets, parse them and put them into inBuffer
void UDPClientListenToServer();

ParsedUDPPacket *UDPClientGetInBufferArray();

#endif