#ifndef UDPMANAGER_H
#define UDPMANAGER_H
#include "UDPClient.h"
#include "../core/EntityManager.h"
#include "../core/Entity.h"
#define MAX_PLAYERS 10
typedef struct UDPManager
{
    Entity *players[MAX_PLAYERS];
    int nrPlayers;
} UDPManager;
UDPManager UDPManagerCreate();
void UDPManagerUpdate(UDPManager *mgr, UDPClient *client, EntityManager *entityManager);
void UDPManagerDraw(UDPManager *mgr, Camera *camera);
#endif