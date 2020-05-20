#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "Client.h"
#include "JoinableSesssion.h"
#include "Lobby.h"
#include "Instance.h"
#include "Round.h"
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
void ClientManagerHandleAllPackets();
// Draws the entire network situation for this client
void ClientManagerDrawConnectedPlayers();
// Draws all buffred shooting lines
void ClientManagerDrawBufferedShootingLines();
// Disconnect from server if it hasn't replied to 'alive' packets before time-out
void ClientManagerDisconnectFromTimeoutServer();
// Handles all state changes from leaving a session
// Menustate, Gamestate, inGame etc.
void ClientManagerLeaveSessionLocally();
// Clears the connected players entities
void ClientManagerClearPlayers();
// Adds shooting line to shooting line buffer
// Can be used even if you are in local game
void ClientManagerAddShootingLine(ShootData shootData);

SDL_bool ClientManagerIsInGame();
SDL_bool ClientManagerIsInLobby();
size_t ClientManagerGetJoinListSize();

void ClientManagerSetInLobby(SDL_bool inLobby);
void ClientManagerSetInGame(SDL_bool inGame);

// Handles different kind of packets
void ClientManagerHandleTextPacket(ParsedPacket packet);
void ClientManagerHandleAreYouAlivePacket(ParsedPacket packet);
void ClientManagerHandleIAmAlivePacket(ParsedPacket packet);
void ClientManagerHandleIsPlayerActivePacket(ParsedPacket packet);
void ClientManagerHandleConnectPacket(ParsedPacket packet);
void ClientManagerHandleDuplicateNamePacket(ParsedPacket packet);
void ClientManagerHandleDisconnectPacket(ParsedPacket packet);
void ClientManagerHandleNewPlayerPacket(ParsedPacket packet);
void ClientManagerHandleDelPlayerPacket(ParsedPacket packet);
void ClientManagerHandleEntityPacket(ParsedPacket packet);
void ClientManagerHandleCompressedEntityPacket(ParsedPacket packet);
void ClientManagerHandleCloseSessionPacket(ParsedPacket packet);
void ClientManagerHandleCreateSessionPacket(ParsedPacket packet);
void ClientManagerHandleJoinSessionPacket(ParsedPacket packet);
void ClientManagerHandleFullSessionPacket(ParsedPacket packet);
void ClientManagerHandleStartRoundPacket(ParsedPacket packet);
void ClientManagerHandleHostAssignPacket(ParsedPacket packet);
void ClientManagerHandleFetchSessionsPacket(ParsedPacket packet);
void ClientManagerHandleFetchLobbyPacket(ParsedPacket packet);
void ClientManagerHandlePlayerHitPacket(ParsedPacket packet);
void ClientManagerHandlePlayerShootPacket(ParsedPacket packet);
void ClientManagerHandlePlayerDeadPacket(ParsedPacket packet);
void ClientManagerHandleNewRoundPacket(ParsedPacket packet);
void ClientManagerHandleScoreboardPacket(ParsedPacket packet);
void ClientManagerHandleCloseAllSessionsPacket(ParsedPacket packet);
void ClientManagerHandleMatchFinishedPacket(ParsedPacket packet);
void ClientManagerHandleRoundFinishedPacket(ParsedPacket packet);
void ClientManagerHandleCountdownPacket(ParsedPacket packet);
void ClientManagerHandleFetchScoreboardPacket(ParsedPacket packet);

EntityIndexP *ClientManagerGetPlayersArray();
JoinableSession *ClientManagerGetJoinListArray();

#endif
