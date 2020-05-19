#ifndef CLIENT_H
#define CLIENT_H

#include "Dependencies.h"
#include "Vector.h"
#include "Player.h"
#include "Packager.h"
#include "State.h"
#include "Notification.h"

#define CLIENT_INBUFFER ClientGetInBuffer()

#define SERVER_TIMEOUT 3.0f

///\param player: which player the client should pair up with
void ClientInitialize();
void ClientUninitialize();

void ClientUpdate();

// Adds deltatime to the server timeout timer
void ClientUpdateServerTimeoutTimer();
// Sends 'alive' packets to server, updating its time-out timer
void ClientPingServer();

// Connects client to server
void ClientTryConnect();
// Thread function to connect to server without interupt, used by "ClientTryConnect()"
void ClientConnectThreadFn();
// Disconnects client to server
int ClientDisconnect();
// Marks client as listening and sends out listener thread
void ClientStartListening();
// Marks client as not listening and collects listener thread
void ClientStopListening();
// Sends a UDP-packet to the server
int ClientUDPSend(PacketType type, void *data, size_t size);
// Final step before UDP-packet leaving the client
int ClientUDPOut(UDPpacket *packet);
// Sends a TCP-packet to the server
int ClientTCPSend(PacketType type, void *data, size_t size);
// Final step before TCP-packet leaving the client
int ClientTCPOut(TCPpacket *packet);
// Thread function to listen for server-packets, parse them and put them into inBuffer
void ClientListenToServer();
// Try receive UDP-packet and add it to inBuffer
int ClientTryReceiveUDPPacket();
// Try receive TCP-packet and add it to inBuffer
int ClientTryReceiveTCPPacket();
// Destroy all packets and clear inBuffer
void ClientClearInBuffer();
// Resets timeout timer and marks as notWaitingForAliveReply
void ClientReceivedAlivePacket();
// Sets player ID and marks client with receivedPlayerID
void ClientSetPlayerID(int id);
// Sets player entity. Used when starting a match
void ClientSetPlayerEntity(Entity *entity);
// Returns the name set by the player
// This is never a null-pointer, but it might point to empty string
char *ClientGetName();
// Sets name of client, it copies MAX_PLAYERNAME_SIZE characters
// This function calls ClientClearName() before copying
void ClientSetName(char *name);
// Clear the name with memset
void ClientClearName();

ParsedPacket *ClientGetInBuffer();
size_t ClientGetInBufferSize();
SDL_mutex *ClientGetInBufferMutex();

SDL_bool ClientIsWaitingForAliveReply();

float ClientGetTimeoutTimer();

#endif