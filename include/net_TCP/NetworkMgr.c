#include "NetworkMgr.h"

#include <stdio.h>

#include "../core/List.h"

struct NetworkMgr
{
    List m_clients;
    List m_servers;
};

NetworkMgr *NetworkMgrCreate()
{
    NetworkMgr *ret = MALLOC(NetworkMgr);
    ret->m_clients = ListCreate();
    ret->m_servers = ListCreate();
    return ret;
}
void NetworkMgrDestroy(NetworkMgr *networkMgr)
{
    SDL_free(networkMgr);
}

void NetworkMgrPollAll(NetworkMgr *networkMgr)
{
#ifdef CLIENT
    for (Node *node = networkMgr->m_clients.front; node; node = node->next)
        for (Payload payload = ClientPopFront(*(Client **)node->data);
             payload.query != Empty;
             payload = ClientPopFront(*(Client **)node->data))
        {
            switch (payload.query)
            {
            case Connect:
                break;
            case Test:
                printf("%s\n", (char *)payload.data);
                break;
            case Empty:
                break;
            default:
                break;
            }
        }
#endif
#ifdef SERVER
    for (Node *node = networkMgr->m_servers.front; node; node = node->next)
        for (Payload payload = ServerPopFront(*(Server **)node->data);
             payload.query != Empty;
             payload = ServerPopFront(*(Server **)node->data))
        {
            switch (payload.query)
            {
            case Connect:
                break;
            case Test:
                printf("%s\n", (char *)payload.data);
                break;
            case Empty:
                break;
            default:
                break;
            }
        }
#endif
}

#ifdef CLIENT
void NetworkMgrAddClient(NetworkMgr *networkMgr, Client *client)
{
    ListPushBack(&networkMgr->m_clients, (void *)&client, sizeof(Client *));
}
#endif
#ifdef SERVER
void NetworkMgrAddServer(NetworkMgr *networkMgr, Server *server)
{
    ListPushBack(&networkMgr->m_servers, (void *)&server, sizeof(Server *));
}
#endif