#ifndef NETWORKMGR_H
#define NETWORKMGR_H

#include "Client.h"
#include "Server.h"

typedef struct NetworkMgr NetworkMgr;

NetworkMgr *NetworkMgrCreate();
void NetworkMgrDestroy(NetworkMgr *networkMgr);

void NetworkMgrPollAll(NetworkMgr *networkMgr);

void NetworkMgrAddClient(NetworkMgr *networkMgr, Client *client);
void NetworkMgrAddServer(NetworkMgr *networkMgr, Server *server);

#endif