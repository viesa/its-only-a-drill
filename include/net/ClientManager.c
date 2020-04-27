#include "ClientManager.h"

void ClientManagerInitialize(Player *player)
{
    clientManager.players = VectorCreate(sizeof(EntityIndexP), 100);
    clientManager.localPlayer = player;
}

void ClientManagerUpdate()
{
    SDL_LockMutex(client.inBufferMutex);
    for (size_t i = 0; i < client.inBuffer->size; i++)
    {
        ParsedPacket nextPacket = CLIENT_INBUFFER[i];

        switch (nextPacket.type)
        {
        case PT_Text:
            ClientManagerHandleTextPacket(nextPacket);
            break;
        case PT_PlayerID:
            ClientManagerHandlePlayerIDPacket(nextPacket);
            break;
        case PT_Entity:
            ClientManagerHandleEntityPacket(nextPacket);
            break;
        case PT_CompressedEntity:
            ClientManagerHandleCompressedEntityPacket(nextPacket);
            break;
        case PT_IPaddress:
            ClientManagerHandleIPaddressPacket(nextPacket);
            break;
        default:
            break;
        }
    }
    for (size_t i = 0; i < client.inBuffer->size; i++)
    {
        ParsedPacketDestroy(&CLIENT_INBUFFER[i]);
    }
    VectorClear(client.inBuffer);
    SDL_UnlockMutex(client.inBufferMutex);
}

void ClientManagerDrawConnectedPlayers(Camera *camera)
{
    for (int i = 0; i < clientManager.players->size; i++)
    {
        EntityDrawIndex(CLIENTMANAGER_PLAYERS[i], camera);
    }
}

void ClientManagerHandleTextPacket(ParsedPacket packet)
{
    if (!SDL_memcmp(packet.data, "quit", 4))
    {
        char id_buffer[10] = {0};

        for (int i = 5; i < packet.size; i++)
        {
            id_buffer[i - 5] = ((char *)packet.data)[i];
        }
        int id = SDL_atoi(id_buffer);
        for (int i = 0; i < clientManager.players->size; i++)
        {
            if (ENTITY_ARRAY[*CLIENTMANAGER_PLAYERS[i]].id == id)
            {
                EntityManagerRemove(CLIENTMANAGER_PLAYERS[i]);
                VectorErase(clientManager.players, i);
                break;
            }
        }
        log_info("Player(id:%d) disconnected\n", id);
    }
}
void ClientManagerHandlePlayerIDPacket(ParsedPacket packet)
{
    int ID = *(int *)packet.data;
    ENTITY_ARRAY[*clientManager.localPlayer->entity].id = ID;
    client.receivedPlayerID = SDL_TRUE;
}
void ClientManagerHandleEntityPacket(ParsedPacket packet)
{
    Entity incomingEntity = *(Entity *)packet.data;

    SDL_bool exist = SDL_FALSE;
    for (int i = 0; i < ENTITY_ARRAY_SIZE; i++)
    {
        if (ENTITY_ARRAY[i].id == incomingEntity.id) //entity exists
        {
            exist = SDL_TRUE;
            ENTITY_ARRAY[i] = incomingEntity;
            //printf("Updating old player\n");
            break;
        }
    }
    if (!exist) //entity doesnt exist, allocate
    {
        EntityIndexP newEntity = EntityManagerAddNoConfig();
        ENTITY_ARRAY[*newEntity] = incomingEntity;
        VectorPushBack(clientManager.players, &newEntity);

        log_info("Player(id:%d) connected\n", incomingEntity.id);
    }
}
void ClientManagerHandleCompressedEntityPacket(ParsedPacket packet)
{
    CompressedEntity cEntity = *(CompressedEntity *)packet.data;

    SDL_bool exist = SDL_FALSE;
    for (int i = 0; i < ENTITY_ARRAY_SIZE; i++)
    {
        if (ENTITY_ARRAY[i].id == cEntity.id) //entity exists
        {
            exist = SDL_TRUE;
            EntityAddCompressed(&ENTITY_ARRAY[i], &cEntity);
            break;
        }
    }
    if (!exist) //entity doesnt exist, allocate
    {
        EntityIndexP newEntity = EntityManagerAdd(ET_Player, Vec2Create(100.0f * 11, 0.0f));
        EntityAddCompressed(&ENTITY_ARRAY[*newEntity], &cEntity);
        VectorPushBack(clientManager.players, &newEntity);
    }
}
void ClientManagerHandleIPaddressPacket(ParsedPacket packet)
{
}

EntityIndexP *ClientManagerGetPlayersArray()
{
    return (EntityIndexP *)clientManager.players->data;
}