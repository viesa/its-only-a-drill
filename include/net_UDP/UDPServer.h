#ifndef UDPSERVER_h
#define UDPSERVER_h

#include "../core/Dependencies.h"
#include "../core/Vector.h"
#include "../core/Entity.h"
#include "UDPPackager.h"

#define UDPSERVER_INBUFFER UDPServerGetInBufferArray()
#define UDPSERVER_PLAYERS UDPServerGetPlayerArray()
#define UDPSERVER_IDS UDPServerGetIDArray()

typedef struct UDPPlayer
{
    IPaddress ip;
    int id;
} UDPPlayer;

struct
{
    UDPsocket socket;
    Vector *players;
    Vector *ids; //0 means taken

    Vector *inBuffer;
    SDL_mutex *inBufferMutex;

    SDL_bool isActive;
    SDL_bool isInitialized;

    SDL_Thread *listenThread;
} udpServer;

// Creates a UDP server on a fixed port
void UDPServerInitialize();
// Destroys the server (udp_close(), unbind, sdlnet close)
void UDPServerUninitialize();
// Marks server as active and sends out threads
void UDPServerStart();
// Marks server as inactive and collects threads
void UDPServerStop();
// Sends a message to everyone on that has sent a message to the server during the session
void UDPServerBroadcast(UDPPacketType type, void *data, int size);
// Sends a message to everyone except a specified ip
void UDPServerBroadcastExclusive(UDPPacketType type, void *data, int size, IPaddress exclusive);
// Sends a message to a specified ip address
void UDPServerSend(UDPPacketType type, void *data, int size, IPaddress ip);
// Final step before leaving the server
void UDPServerOut(UDPpacket *packet);
// Thread function to listen to clients and receive packets, parse them and add them to inBuffer
void UDPServerListenToClients();
// Deletes the client from player-list and notifies all clients
void UDPServerRemoveClient();
// Returns lowest free unique ID
int UDPServerGetID(IPaddress ip);
// Marks the id as non-taken
void UDPServerFreeID(int id);

ParsedUDPPacket *UDPServerGetInBufferArray();
UDPPlayer *UDPServerGetPlayerArray();
SDL_bool *UDPServerGetIDArray();

#endif