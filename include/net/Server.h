#ifndef SERVER_h
#define SERVER_h

#include "../core/Dependencies.h"
#include "../core/Vector.h"
#include "../core/Entity.h"
#include "../core/State.h"
#include "Packager.h"
#include "NetPlayer.h"
#include "Session.h"

#define SERVER_INBUFFER ServerGetInBufferArray()
#define SERVER_PLAYERS ServerGetPlayerArray()
#define SERVER_IDS ServerGetIDArray()
#define SERVER_SESSIONS ServerGetSessionArray()
#define SERVER_SESSIONBITMAP ServerGetSessionBitmapArray()

#define TRAFFIC_BUFFER_SIZE 5000

struct
{
    UDPsocket udpSocket;
    TCPsocket tcpSocket;
    SDLNet_SocketSet socketSet;
    Vector *players;
    Vector *ids; //0 means taken
    int currentIDIndex;

    Vector *sessions;
    Vector *sessionBitmap;

    Vector *inBuffer;
    SDL_mutex *inBufferMutex;

    SDL_bool isActive;
    SDL_bool isInitialized;

    SDL_Thread *listenThread;

    SDL_mutex *trafficMutex;
} server;

// Creates a UDP server on a fixed port
void ServerInitialize();
// Destroys the server (udp_close(), unbind, sdlnet close)
void ServerUninitialize();
// Marks server as active and sends out threads
void ServerStart();
// Marks server as inactive and collects threads
void ServerStop();
// Broadcast UDP-packet to connected clients
void ServerUDPBroadcast(PacketType type, void *data, int size);
// Broadcast UDP-packet to connected clients on the given session
void ServerUDPBroadcastSession(PacketType type, Session *session, void *data, int size);
// Broadcast UDP-packet to connected clients, with NetPlayer-exclusion option
void ServerUDPBroadcastExclusive(PacketType type, void *data, int size, NetPlayer exclusive);
// Broadcast UDP-packet to connected clients, with NetPlayer-exclusion option on the given session
void ServerUDPBroadcastExclusiveSession(PacketType type, Session *session, void *data, int size, NetPlayer exclusive);
// Sends UDP-packet to a specified NetPlayer
void ServerUDPSend(PacketType type, void *data, int size, NetPlayer player);
// Final step before UDP-packet leaving the server, every send operation leads to this step
void ServerUDPOut(UDPpacket *packet);
// Broadcast TCP-packet to connected clients
void ServerTCPBroadcast(PacketType type, void *data, int size);
// Broadcast TCP-packet to connected clients on the given session
void ServerTCPBroadcastSession(PacketType type, Session *session, void *data, int size);
// Broadcast TCP-packet to connected clients, with NetPlayer-exclusion option
void ServerTCPBroadcastExclusive(PacketType type, void *data, int size, NetPlayer exclusive);
// Broadcast TCP-packet to connected clients, with NetPlayer-exclusion option on the given session
void ServerTCPBroadcastExclusiveSession(PacketType type, Session *session, void *data, int size, NetPlayer exclusive);
// Sends TCP-packet to a specified NetPlayer
void ServerTCPSend(PacketType type, void *data, int size, NetPlayer player);
// // Final step before TCP-packet leaving the client
// Every other sending operation leads to this step
void ServerTCPOut(TCPpacket *packet);
// Thread function to listen to clients and receive packets, parse them and add them to inBuffer
void ServerListenToClients();
// Try receive UDP-packet and add it to inBuffer
int ServerTryReceiveUDPPacket();
// Try accept a new TCP-socket and add it to socket set
int ServerTryAcceptTCPSocket();
// Try receive TCP-packet and add it to inBuffer
int ServerTryReceiveTCPPacket(NetPlayer player);
// Deletes the client from player-list and notifies all clients
void ServerRemoveClient(NetPlayer player);
// Returns lowest free unique ID
int ServerGetID();
// Marks the id as non-taken
void ServerFreeID(int id);
// Returns lowest free unique session ID
int ServerGetSessionID();
// Marks the session id as non-taken
void ServerFreeSessionID(int id);

ParsedPacket *ServerGetInBufferArray();
NetPlayer *ServerGetPlayerArray();
SDL_bool *ServerGetIDArray();
Session *ServerGetSessionArray();
SDL_bool *ServerGetSessionBitmapArray();
#endif