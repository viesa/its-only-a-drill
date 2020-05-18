#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include "Server.h"
#include "JoinableSesssion.h"
#include "Weapon.h"

void ServerManagerInitalize(SDL_bool *isRunning);
void ServerManagerDestroy();

// Handles all incoming packets
void ServerManagerHandleAllPackets();

// Kicks all clients that hasn't replied to 'alive' packets before time-out
void ServerManagerKickTimeoutClients();

// Handles different kind of packets
void ServerManagerHandleTextPacket(ParsedPacket packet);
void ServerManagerHandleAreYouAlivePacket(ParsedPacket packet);
void ServerManagerHandleIAmAlivePacket(ParsedPacket packet);
void ServerManagerHandleIsPlayerActivePacket(ParsedPacket packet);
void ServerManagerHandleConnectPacket(ParsedPacket packet);
void ServerManagerHandleUDPRespondIPPacket(ParsedPacket packet);
void ServerManagerHandleDisconnectPacket(ParsedPacket packet);
void ServerManagerHandleEntityPacket(ParsedPacket packet);
void ServerManagerHandleCompressedEntityPacket(ParsedPacket packet);
void ServerManagerHandleCreateSessionPacket(ParsedPacket packet);
void ServerManagerHandleJoinSessionPacket(ParsedPacket packet);
void ServerManagerHandleLeaveSessionPacket(ParsedPacket packet);
void ServerManagerHandleStartSessionPacket(ParsedPacket packet);
void ServerManagerHandleChangeSkinPacket(ParsedPacket packet);
void ServerManagerHandleFetchLobbyPacket(ParsedPacket packet);
void ServerManagerHandleFetchSessionsPacket(ParsedPacket packet);
void ServerManagerHandlePlayerHitPacket(ParsedPacket packet);
void ServerManagerHandlePlayerShootPacket(ParsedPacket packet);
void ServerManagerHandlePlayerDeadPacket(ParsedPacket packet);
void ServerManagerHandleFetchPlayerPointsPacket(ParsedPacket packet);

#endif