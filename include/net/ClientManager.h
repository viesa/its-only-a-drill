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

// Creates the client manager
void ClientManagerInitialize();
// Destroys the client manager
void ClientManagerUninitialize();
// Updates the entire network situation for this client
void ClientManagerUpdate();
// Draws the entire network situation for this client
void ClientManagerDrawConnectedPlayers();
// Disconnect from server if it hasn't replied to 'alive' packets before time-out
void ClientManagerDisconnectFromTimeoutServer();
// Handles all state changes from leaving a session
// Menustate, Gamestate, inGame etc.
void ClientManagerLeaveSessionLocally();

SDL_bool ClientManagerIsInGame();
SDL_bool ClientManagerIsInLobby();
size_t ClientManagerGetJoinListSize();

void ClientManagerSetInLobby(SDL_bool inLobby);
void ClientManagerSetInGame(SDL_bool inGame);

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
