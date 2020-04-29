#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "Client.h"

#define CLIENTMANAGER_PLAYERS ClientManagerGetPlayersArray()

struct
{
    Vector *players;
} clientManager;

// Creates the client manager
void ClientManagerInitialize();
// Destroys the client manager
void ClientManagerUninitialize();
// Updates the entire network situation for this client
void ClientManagerUpdate();
// Draws the entire network situation for this client
void ClientManagerDrawConnectedPlayers(Camera *camera);

// Handles different kind of packets
void ClientManagerHandleTextPacket(ParsedPacket packet);
void ClientManagerHandleConnectPacket(ParsedPacket packet);
void ClientManagerHandleDisconnectPacket(ParsedPacket packet);
void ClientManagerHandleNewPlayerPacket(ParsedPacket packet);
void ClientManagerHandleDelPlayerPacket(ParsedPacket packet);
void ClientManagerHandleEntityPacket(ParsedPacket packet);
void ClientManagerHandleCompressedEntityPacket(ParsedPacket packet);

EntityIndexP *ClientManagerGetPlayersArray();

#endif
