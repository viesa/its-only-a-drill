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
        UDPPackageTypes type = UDPPackageDecode((char *)client->pack->data);
        switch (type)
        {
        case UDPTypeText:
            UDPPackageRemoveType(client->pack);
            char msg[MAX_MSGLEN];
            SDL_memcpy(msg, client->pack->data, client->pack->len);
            if (msg[0] == 'q' && msg[1] == 'u' && msg[2] == 'i' && msg[3] == 't') // om strcmp funkar här snälla byt ut det :)
            {
                int id = 0;
                char nr[10];
                for (int i = 5; i < client->pack->len - 1; i++)
                {
                    nr[i - 5] = client->pack->data[i];
                }
                nr[client->pack->len - 4] = '\0';
                id = atoi(nr);
                for (int i = 0; i < mgr->nrPlayers; i++)
                {
                    if (mgr->players[i]->id == id)
                    {
                        mgr->nrPlayers--;
                        for (int j = i; j < mgr->nrPlayers; j++)
                        {
                            mgr->players[j] = mgr->players[j + 1];
                        }
                    }
                }
                log_info("Player(id:%d) disconnected\n", id);
            }
            break;
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
                    //printf("Updating old player\n");
                }
            }
            if (!exist1) //entity doesnt exist, allocate
            {
                Entity *e1 = EntityManagerAdd(entityManager, ET_Player, Vec2Create(100.0f * 11, 0.0f));
                *e1 = ent;
                mgr->players[mgr->nrPlayers] = e1;
                log_info("Player(id:%d) connected\n", ent.id);
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
                Entity *e2 = EntityManagerAdd(entityManager, ET_Player, Vec2Create(100.0f * 11, 0.0f));
                EntityAddCompressed(comp, e2);
                mgr->players[mgr->nrPlayers] = e2;
                mgr->nrPlayers++;
            }
            break;
        default:
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