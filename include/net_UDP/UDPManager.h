#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "UDPClient.h"
#include "../Player.h"

#define UDPMANAGER_PLAYERS UDPManagerGetPlayersArray()

struct
{
    Vector *players;
    Player *localPlayer;
} udpManager;

// Creates a vector to hold the players
void UDPManagerInitialize(Player *player);
// Updates the entire network situation for this client
void UDPManagerUpdate();
// Draws the entire network situation for this client
void UDPManagerDrawConnectedPlayers(Camera *camera);

// Handles different kind of packets
void UDPManagerHandleTextPacket(ParsedUDPPacket packet);
void UDPManagerHandlePlayerIDPacket(ParsedUDPPacket packet);
void UDPManagerHandleEntityPacket(ParsedUDPPacket packet);
void UDPManagerHandleCompressedEntityPacket(ParsedUDPPacket packet);
void UDPManagerHandleIPaddressPacket(ParsedUDPPacket packet);

EntityIndexP *UDPManagerGetPlayersArray();

#endif
