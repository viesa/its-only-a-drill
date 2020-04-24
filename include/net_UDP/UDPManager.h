#ifndef UDPMANAGER_H
#define UDPMANAGER_H
#include "UDPClient.h"
#include "../core/EntityManager.h"
#include "../core/Entity.h"
#define MAX_PLAYERS 10
typedef struct UDPManager
{
    EntityIndexP players[MAX_PLAYERS];
    int nrPlayers;
} UDPManager;
// Returns a UDPManager struct with 0 as nr of players
UDPManager UDPManagerCreate();
// Updates the entire network situation for this client
void UDPManagerUpdate(UDPManager *mgr, UDPClient *client);
// Draws the entire network situation for this client
void UDPManagerDraw(UDPManager *mgr, Camera *camera);
#endif