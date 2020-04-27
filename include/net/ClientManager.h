#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "Client.h"
#include "../Player.h"

#define CLIENTMANAGER_PLAYERS ClientManagerGetPlayersArray()

struct
{
    Vector *players;
    Player *localPlayer;
} clientManager;

// Creates a vector to hold the players
void ClientManagerInitialize(Player *player);
// Updates the entire network situation for this client
void ClientManagerUpdate();
// Draws the entire network situation for this client
void ClientManagerDrawConnectedPlayers(Camera *camera);

// Handles different kind of packets
void ClientManagerHandleTextPacket(ParsedPacket packet);
void ClientManagerHandlePlayerIDPacket(ParsedPacket packet);
void ClientManagerHandleEntityPacket(ParsedPacket packet);
void ClientManagerHandleCompressedEntityPacket(ParsedPacket packet);
void ClientManagerHandleIPaddressPacket(ParsedPacket packet);

EntityIndexP *ClientManagerGetPlayersArray();

#endif
