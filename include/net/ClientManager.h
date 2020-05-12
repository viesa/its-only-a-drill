#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "Client.h"
#include "JoinableSesssion.h"
#include "Lobby.h"
#include "State.h"
#include "Map.h"
#include "Notification.h"

#define CLIENTMANAGER_PLAYERS ClientManagerGetPlayersArray()
#define CLIENTMANAGER_JOINLIST ClientManagerGetJoinListArray()

struct
{
    Vector *players;
    // A vector of structs, JoinableSession
    Vector *joinList;
    SDL_bool inLobby; // both of these cannot be true at once
    SDL_bool inGame;  // :)
} clientManager;

// Creates the client manager
void ClientManagerInitialize();
// Destroys the client manager
void ClientManagerUninitialize();
// Updates the entire network situation for this client
void ClientManagerUpdate();
// Draws the entire network situation for this client
void ClientManagerDrawConnectedPlayers(Camera *camera);

// Adds deltatime to the server timeout timer
void ClientManagerUpdateServerTimeoutTimer();
// Disconnect from server if it hasn't replied to 'alive' packets before time-out
void ClientManagerDisconnectFromTimeoutServer();
// Sends 'alive' packets to server, updating its time-out timer
void ClientManagerPingServer();
// Handles all state changes from leaving a session
// Menustate, Gamestate, inGame etc.
void ClientManagerLeaveSessionLocally();

// Handles different kind of packets
void ClientManagerHandleTextPacket(ParsedPacket packet);
void ClientManagerHandleAreYouAlivePacket(ParsedPacket packet);
void ClientManagerHandleIAmAlivePacket(ParsedPacket packet);
void ClientManagerHandleConnectPacket(ParsedPacket packet);
void ClientManagerHandleDuplicateNamePacket(ParsedPacket packet);
void ClientManagerHandleDisconnectPacket(ParsedPacket packet);
void ClientManagerHandleNewPlayerPacket(ParsedPacket packet);
void ClientManagerHandleDelPlayerPacket(ParsedPacket packet);
void ClientManagerHandleEntityPacket(ParsedPacket packet);
void ClientManagerHandleCompressedEntityPacket(ParsedPacket packet);
void ClientManagerHandleCreateSessionPacket(ParsedPacket packet);
void ClientManagerHandleJoinSessionPacket(ParsedPacket packet);
void ClientManagerHandleFullSessionPacket(ParsedPacket packet);
void ClientManagerHandleStartSessionPacket(ParsedPacket packet);
void ClientManagerHandleHostAssignPacket(ParsedPacket packet);
void ClientManagerHandleFetchSessionsPacket(ParsedPacket packet);
void ClientManagerHandleFetchLobbyPacket(ParsedPacket packet);
void ClientManagerHandleCloseAllSessionsPacket(ParsedPacket packet);
void ClientManagerHandlePlayerHitPacket(ParsedPacket Packet);

EntityIndexP *ClientManagerGetPlayersArray();
JoinableSession *ClientManagerGetJoinListArray();

#endif
