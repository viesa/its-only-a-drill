#ifndef SERVER_h
#define SERVER_h

#include "../core/Dependencies.h"
#include "../core/Vector.h"
#include "../core/Entity.h"
#include "Packager.h"
#include "NetPlayer.h"

#define SERVER_INBUFFER ServerGetInBufferArray()
#define SERVER_PLAYERS ServerGetPlayerArray()
#define SERVER_IDS ServerGetIDArray()

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
} server;

// Creates a UDP server on a fixed port
void ServerInitialize();
// Destroys the server (udp_close(), unbind, sdlnet close)
void ServerUninitialize();
// Marks server as active and sends out threads
void ServerStart();
// Marks server as inactive and collects threads
void ServerStop();
// Sends a message to everyone on that has sent a message to the server during the session
void ServerBroadcast(PacketType type, void *data, int size);
// Sends a message to everyone except a specified ip
void ServerBroadcastExclusive(PacketType type, void *data, int size, IPaddress exclusive);
// Sends a message to a specified ip address
void ServerSend(PacketType type, void *data, int size, IPaddress ip);
// Final step before leaving the server
void ServerOut(UDPpacket *packet);
// Thread function to listen to clients and receive packets, parse them and add them to inBuffer
void ServerListenToClients();
// Deletes the client from player-list and notifies all clients
void ServerRemoveClient();
// Returns lowest free unique ID
int ServerGetID(IPaddress ip);
// Marks the id as non-taken
void ServerFreeID(int id);

ParsedPacket *ServerGetInBufferArray();
NetPlayer *ServerGetPlayerArray();
SDL_bool *ServerGetIDArray();

#endif