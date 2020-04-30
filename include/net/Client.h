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

    char name[MAX_PLAYERNAME_SIZE];

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
// Chooses how to send the packet, if it is big or small
int ClientTCPOut(TCPpacket *packet);
// Final step before TCP-packet leaving the client, packetsize < 63535
int ClientTCPSmallOut(TCPpacket *packet);
// Final step before TCP-packet leaving the client, packetsize > 63535
int ClientTCPBigOut(TCPpacket *packet);
// Thread function to listen for server-packets, parse them and put them into inBuffer
void ClientListenToServer();
// Try receive UDP-packet and add it to inBuffer
int ClientTryReceiveUDPPacket();
// Receives header of TCP-packet and chooses how to receive the rest, big or small
int ClientTryReceiveTCPPacket();
// Receives TCP-packet and add it to inBuffer, packetsize < 63535
int ClientReceiveSmallTCPPacket(size_t packetSize);
// Receives TCP-packet and add it to inBuffer, packetsize > 63535
int ClientReceiveBigTCPPacket(size_t packetSize);

ParsedPacket *ClientGetInBufferArray();

#endif