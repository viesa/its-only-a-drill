#ifndef SERVER_h
#define SERVER_h

#include "Dependencies.h"
#include "Vector.h"
#include "Entity.h"
#include "State.h"
#include "Packager.h"
#include "NetPlayer.h"
#include "Session.h"

#define SERVER_INBUFFER ServerGetInBuffer()
#define SERVER_PLAYERS ServerGetPlayers()
#define SERVER_IDS ServerGetIDs()
#define SERVER_SESSIONS ServerGetSessions()
#define SERVER_SESSIONBITMAP ServerGetSessionBitmaps()

#define CLIENT_TIMEOUT 3.0f

// Creates a UDP server on a fixed port
void ServerInitialize();
// Destroys the server (udp_close(), unbind, sdlnet close)
void ServerUninitialize();
// Marks server as listening and sends out listener thread
void ServerStartListening();
// Marks server as not listening and collects listener thread
void ServerStopListening();

// Adds deltatime to every client timeout timer
void ServerUpdateTimeoutTimers();
// Sends 'alive' packets to all clients, updating their time-out timer
void ServerPingClients();
// Removes all players from session then deleting the session from session list
void ServerRemoveSession(Session *session);

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
int ServerGenID();
// Marks the id as non-taken
void ServerFreeID(int id);
// Returns lowest free unique session ID
int ServerGetSessionID();
// Marks the session id as non-taken
void ServerFreeSessionID(int id);
///\return: A pointer to matching session, NULL if not found
Session *ServerGetSessionByID(int sessionID);
// Find a netplayer in servers array of netplayers
///\return: A pointer to matching player, NULL if not found
NetPlayer *ServerGetPlayerByID(int id);
// Find a netplayer in servers array of netplayers
///\return: A pointer to matching player, NULL if not found
NetPlayer *ServerGetPlayerByNetPlayer(NetPlayer player);
// Removes a player from given session, notfies players and remove the session if necessary
void ServerRemovePlayerFromSession(Session *session, int playerID);

void ServerClearInBuffer();
// Append a new session to session list
void ServerAddSession(Session *session);

SDL_bool ServerIsListening();
ParsedPacket *ServerGetInBuffer();
size_t ServerGetInBufferSize();
SDL_mutex *ServerGetInBufferMutex();
NetPlayer *ServerGetPlayers();
size_t ServerGetNumPlayers();
SDL_bool *ServerGetIDs();
Session *ServerGetSessions();
size_t ServerGetNumSessions();
SDL_bool *ServerGetSessionBitmaps();
SDL_mutex *ServerGetTrafficMutex();
#endif