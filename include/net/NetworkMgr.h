#ifndef NETWORKMGR_H
#define NETWORKMGR_H
#define CLIENT
#define SERVER

#ifdef CLIENT
#include "Client.h"
#endif
#ifdef SERVER
#include "Server.h"
#endif

typedef struct NetworkMgr NetworkMgr;

NetworkMgr *NetworkMgrCreate();
void NetworkMgrDestroy(NetworkMgr *networkMgr);

void NetworkMgrPollAll(NetworkMgr *networkMgr);

#ifdef CLIENT
void NetworkMgrAddClient(NetworkMgr *networkMgr, Client *client);
#endif
#ifdef SERVER
void NetworkMgrAddServer(NetworkMgr *networkMgr, Server *server);
#endif

#endif