#include "UDPManager.h"
UDPManager UDPManagerCreate()
{
    UDPManager mgr;
    mgr.nrPlayers = 0;
    return mgr;
}
void UDPManagerUpdate(UDPManager *mgr, UDPClient *client, EntityManager *entityManager)
{
    if (client->hasPacket)
    {
        switch (UDPPackageDecode((char *)client->pack->data))
        {
        case UDPTypeEntity:
            UDPPackageRemoveTypeNULL(client->pack);
            Entity ent;
            SDL_memcpy(&ent, client->pack->data, client->pack->len);
            SDL_bool exist1 = SDL_FALSE;

            for (int i = 0; i < entityManager->highestIndex; i++)
            {
                if (entityManager->entities[i].id == ent.id) //entity exists
                {
                    exist1 = SDL_TRUE;
                    entityManager->entities[i] = ent;
                }
            }
            if (!exist1) //entity doesnt exist, allocate
            {
                Entity *e1 = EntityManagerAdd(entityManager, EntityPlayer, Vec2Create(100.0f * 11, 0.0f));
                *e1 = ent;
                mgr->players[mgr->nrPlayers] = e1;
                mgr->nrPlayers++;
            }
            break;
        case UDPTypeCompressedEntity:
            UDPPackageRemoveTypeNULL(client->pack);
            CompressedEntity comp;
            SDL_memcpy(&comp, client->pack->data, sizeof(CompressedEntity));
            SDL_bool exist2 = SDL_FALSE;

            for (int i = 0; i < entityManager->highestIndex; i++)
            {
                if (entityManager->entities[i].id == comp.id) //entity exists
                {
                    exist2 = SDL_TRUE;
                    EntityAddCompressed(comp, &entityManager->entities[i]);
                }
            }
            if (!exist2) //entity doesnt exist, allocate
            {
                Entity *e2 = EntityManagerAdd(entityManager, EntityPlayer, Vec2Create(100.0f * 11, 0.0f));
                EntityAddCompressed(comp, e2);
                mgr->players[mgr->nrPlayers] = e2;
                mgr->nrPlayers++;
            }
            break;
        case UDPTypeIPaddress:
            UDPPackageRemoveTypeNULL(client->pack);
            IPaddress ip;
            SDL_memcpy(&ip, client->pack->data, sizeof(ip));
            for (int i = 0; i < mgr->nrPlayers; i++)
            {
                if (client->serverip.host == ip.host && client->serverip.port == ip.port)
                {
                    EntityManagerRemove(entityManager, mgr->players[i]);
                    mgr->nrPlayers--;
                    for (int j = i; j < mgr->nrPlayers; i++)
                    {
                        mgr->players[j] = mgr->players[j + 1];
                    }
                }
            }
            break;
        }
        client->hasPacket = SDL_FALSE;
    }
}
void UDPManagerDraw(UDPManager *mgr, Camera *camera)
{
    for (int i = 0; i < mgr->nrPlayers; i++)
    {
        EntityDraw(mgr->players[i], camera);
    }
}