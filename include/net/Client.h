#ifndef CLIENT_H
#define CLIENT_H

#include "../core/Dependencies.h"
#include "../core/Vector.h"
#include "../Player.h"
#include "Packager.h"

#define CLIENT_INBUFFER ClientGetInBufferArray()

struct
{
    Player *player;

    NetPlayer server;
    UDPsocket udpSocket;
    SDLNet_SocketSet socketSet;

    Vector *inBuffer;
    SDL_mutex *inBufferMutex;

    SDL_bool isActive;
    SDL_bool isInitialized;
    SDL_bool receivedPlayerID;

    SDL_Thread *listenThread;

} client;

void ClientInitialize();
void ClientUninitialize();

// Sets which player the client should pair up with
void ClientSetPlayer(Player *player);
// Marks client as active and sends out threads
void ClientStart();
// Marks client as inactive and collects threads
void ClientStop();
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
// Try receive UDP-packet and add it to inBuffer
int ClientTryReceiveTCPPacket();

ParsedPacket *ClientGetInBufferArray();

#endif