#include "UDPManager.h"

void UDPManagerInitialize(Player *player)
{
    udpManager.players = VectorCreate(sizeof(EntityIndexP), 100);
    udpManager.localPlayer = player;
}

void UDPManagerUpdate()
{
    SDL_LockMutex(udpClient.inBufferMutex);
    for (size_t i = 0; i < udpClient.inBuffer->size; i++)
    {
        ParsedUDPPacket nextPacket = UDPCLIENT_INBUFFER[i];

        switch (nextPacket.type)
        {
        case UDPType_Text:
            UDPManagerHandleTextPacket(nextPacket);
            break;
        case UDPType_PlayerID:
            UDPManagerHandlePlayerIDPacket(nextPacket);
            break;
        case UDPType_Entity:
            UDPManagerHandleEntityPacket(nextPacket);
            break;
        case UDPType_CompressedEntity:
            UDPManagerHandleCompressedEntityPacket(nextPacket);
            break;
        case UDPType_IPaddress:
            UDPManagerHandleIPaddressPacket(nextPacket);
            break;
        default:
            break;
        }
    }
    for (size_t i = 0; i < udpClient.inBuffer->size; i++)
    {
        ParsedUDPPacketDestroy(&UDPCLIENT_INBUFFER[i]);
    }
    VectorClear(udpClient.inBuffer);
    SDL_UnlockMutex(udpClient.inBufferMutex);
}

void UDPManagerDrawConnectedPlayers(Camera *camera)
{
    for (int i = 0; i < udpManager.players->size; i++)
    {
        EntityDrawIndex(UDPMANAGER_PLAYERS[i], camera);
    }
}

void UDPManagerHandleTextPacket(ParsedUDPPacket packet)
{
    if (!SDL_memcmp(packet.data, "quit", 4))
    {
        char id_buffer[10] = {0};

        for (int i = 5; i < packet.size; i++)
        {
            id_buffer[i - 5] = ((char *)packet.data)[i];
        }
        int id = SDL_atoi(id_buffer);
        for (int i = 0; i < udpManager.players->size; i++)
        {
            if (ENTITY_ARRAY[*UDPMANAGER_PLAYERS[i]].id == id)
            {
                EntityManagerRemove(UDPMANAGER_PLAYERS[i]);
                VectorErase(udpManager.players, i);
                break;
            }
        }
        log_info("Player(id:%d) disconnected\n", id);
    }
}
void UDPManagerHandlePlayerIDPacket(ParsedUDPPacket packet)
{
    int ID = *(int *)packet.data;
    ENTITY_ARRAY[*udpManager.localPlayer->entity].id = ID;
    udpClient.receivedPlayerID = SDL_TRUE;
}
void UDPManagerHandleEntityPacket(ParsedUDPPacket packet)
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
        VectorPushBack(udpManager.players, &newEntity);

        log_info("Player(id:%d) connected\n", incomingEntity.id);
    }
}
void UDPManagerHandleCompressedEntityPacket(ParsedUDPPacket packet)
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
        VectorPushBack(udpManager.players, &newEntity);
    }
}
void UDPManagerHandleIPaddressPacket(ParsedUDPPacket packet)
{
}

EntityIndexP *UDPManagerGetPlayersArray()
{
    return (EntityIndexP *)udpManager.players->data;
}