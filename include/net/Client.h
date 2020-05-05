#ifndef CLIENT_H
#define CLIENT_H

#include "Dependencies.h"
#include "Vector.h"
#include "Player.h"
#include "Packager.h"
#include "State.h"
#include "Notification.h"

#define CLIENT_INBUFFER ClientGetInBufferArray()

struct
{
    Player *player;

    NetPlayer server;
    UDPsocket udpSocket;
    SDLNet_SocketSet socketSet;

    Vector *inBuffer;
    SDL_mutex *inBufferMutex;

    SDL_bool isListening;
    SDL_bool isInitialized;
    SDL_bool receivedPlayerID;

    SDL_Thread *listenThread;

    char name[MAX_PLAYERNAME_SIZE];
    SDL_bool hasSentName;

    float connectTimer;
    SDL_mutex *connectMutex;
} client;

///\param player: which player the client should pair up with
void ClientInitialize(Player *player);
void ClientUninitialize();

void ClientUpdate();

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

ParsedPacket *ClientGetInBufferArray();

#endif