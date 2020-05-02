#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "Client.h"
#include "JoinableSesssion.h"
#include "Lobby.h"
#include "../core/State.h"
#include "../Map.h"

#define CLIENTMANAGER_PLAYERS ClientManagerGetPlayersArray()
#define CLIENTMANAGER_JOINLIST ClientManagerGetJoinListArray()

struct
{
    Vector *players;
    // A vector of structs, JoinableSession
    Vector *joinList;
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
void ClientManagerHandleCreateSessionPacket(ParsedPacket packet);
void ClientManagerHandleJoinSessionPacket(ParsedPacket packet);
void ClientManagerHandleLeaveSessionPacket(ParsedPacket packet);
void ClientManagerHandleFullSessionPacket(ParsedPacket packet);
void ClientManagerHandleHostAssignPacket(ParsedPacket packet);
void ClientManagerHandleFetchSessionsPacket(ParsedPacket packet);
void ClientManagerHandleFetchLobbyPacket(ParsedPacket packet);

EntityIndexP *ClientManagerGetPlayersArray();
JoinableSession *ClientManagerGetJoinListArray();

#endif
