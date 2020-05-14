#include "EntityManager.h"
#include "Client.h"
#define MAX_ENTITY_INDICES 10000
#define energy_lost 0.1f

struct
{
    Vector *entityVector;
    size_t *indices;
} entityManager;

void EntityManagerInitialize()
{
    entityManager.entityVector = VectorCreate(sizeof(Entity), 100);
    entityManager.indices = MALLOC_N(size_t, MAX_ENTITY_INDICES);
    ALLOC_ERROR_CHECK(entityManager.indices);
    for (size_t i = 0; i < MAX_ENTITY_INDICES; i++)
        entityManager.indices[i] = 0;
    // Pushes back fallback entity
    EntityManagerAdd(ET_None, Vec2Create(0.0f, 0.0f));
}

void EntityManagerUninitalize()
{
    VectorDestroy(entityManager.entityVector);
}

void EntityManagerUpdate()
{
    EntityManagerUpdateMovement();
    EntityManagerOnCollision();
}

void EntityManagerUpdateMovement()
{
    for (int i = 1; i < ENTITY_ARRAY_SIZE; i++)
    {
        // velocity
        ENTITY_ARRAY[i].Velocity = Vec2DivL(ENTITY_ARRAY[i].Force, ENTITY_ARRAY[i].mass);
        // carculate Net_force so friction & collision & the other forces is handle before
        EntityCalculateNetForces(&ENTITY_ARRAY[i]);

        // update new position
        ENTITY_ARRAY[i].position.x += ENTITY_ARRAY[i].Velocity.x * ClockGetDeltaTime();
        ENTITY_ARRAY[i].position.y += ENTITY_ARRAY[i].Velocity.y * ClockGetDeltaTime();
        if (Vec2LenSq(ENTITY_ARRAY[i].Velocity) > 0.1f &&
            ENTITY_ARRAY[i].id >= 10000)
        {
            CompressedEntity cEntity = EntityCompress(&ENTITY_ARRAY[i]);
            ClientUDPSend(PT_CompressedEntity, &cEntity, sizeof(CompressedEntity));
        }
    }
}

void EntityManagerOnCollision()
{
    SDL_Rect result;
    for (int Dominant = 1; Dominant < ENTITY_ARRAY_SIZE; Dominant++)
    {
        if (ENTITY_ARRAY[Dominant].isCollider)
            for (int Recessive = 1; Recessive < ENTITY_ARRAY_SIZE; Recessive++)
            {
                if (ENTITY_ARRAY[Recessive].isCollider)
                {
                    if (Dominant != Recessive)
                    {
                        // Dominant hitbox
                        SDL_Rect *dHitbox = &ENTITY_ARRAY[Dominant].drawables[ENTITY_ARRAY[Dominant].hitboxIndex].dst;
                        // Recessive hitbox
                        SDL_Rect *rHitbox = &ENTITY_ARRAY[Recessive].drawables[ENTITY_ARRAY[Recessive].hitboxIndex].dst;
                        if (SDL_IntersectRect(dHitbox, rHitbox, &result))
                        {
                            Vec2 DominantCenter; // calculating center
                            DominantCenter.x = (float)dHitbox->x + (float)dHitbox->w / 2.0f;
                            DominantCenter.y = (float)dHitbox->y + (float)dHitbox->h / 2.0f;

                            Vec2 RecessiveCenter; // calculating center
                            RecessiveCenter.x = (float)rHitbox->x + (float)rHitbox->w / 2.0f;
                            RecessiveCenter.y = (float)rHitbox->y + (float)rHitbox->h / 2.0f;

                            Vec2 ResultDistance = Vec2Sub(DominantCenter, RecessiveCenter);
                            Vec2 direction = Vec2Unit(ResultDistance);

                            //keept for transfer of energy
                            //float HarassersForce = Vec2Len(ENTITY_ARRAY[Dominant].Force);
                            //Vec2 forceVector = Vec2MulL(direction, HarassersForce);

                            float massRatioBasedOnDominant = ENTITY_ARRAY[Dominant].mass / (ENTITY_ARRAY[Dominant].mass + ENTITY_ARRAY[Recessive].mass);
                            if (massRatioBasedOnDominant >= 0.05f && massRatioBasedOnDominant != 0.5f)
                            {
                                ENTITY_ARRAY[Recessive].position.x -= (ResultDistance.x * massRatioBasedOnDominant) / 17.0f;
                                ENTITY_ARRAY[Recessive].position.y -= (ResultDistance.y * massRatioBasedOnDominant) / 17.0f;

                                ENTITY_ARRAY[Dominant].position.x += (ResultDistance.x * (1 - massRatioBasedOnDominant)) / 17.0f;
                                ENTITY_ARRAY[Dominant].position.y += (ResultDistance.y * (1 - massRatioBasedOnDominant)) / 17.0f;
                            }
                        }
                    }
                }
            }
    }
}

EntityIndexP EntityManagerAdd(EntityType entityType, Vec2 position)
{
    size_t freeIndex = EntityManagerGetFreeIndex();
    if (!freeIndex)
    {
        log_fatal("No free entity indices");
    }
    else
    {
        Entity entity = EntityCreate(position, entityType, 0);
        VectorPushBack(ENTITY_VECTOR, &entity);
        entityManager.indices[freeIndex] = ENTITY_ARRAY_SIZE - 1;
    }
    return (EntityIndexP)&entityManager.indices[freeIndex];
}

EntityIndexP EntityManagerAddNoConfig()
{
    return EntityManagerAdd(ET_None, Vec2Create(0.0f, 0.0f));
}

void EntityManagerRemove(EntityIndexP index)
{
    EntityManagerRemoveRange(*index, *index + 1);
}

void EntityManagerRemoveRange(size_t start, size_t end)
{
    size_t delta = end - start;
    for (size_t i = 1; i < MAX_ENTITY_INDICES; i++)
    {
        if (entityManager.indices[i] >= start && entityManager.indices[i] < end)
        {
            entityManager.indices[i] = 0;
        }
    }
    for (size_t i = 1; i < MAX_ENTITY_INDICES; i++)
    {
        if (entityManager.indices[i] >= end)
            entityManager.indices[i] -= delta;
    }
    VectorEraseRange(ENTITY_VECTOR, start, end);
}

Vector *EntityManagerGetVector()
{
    return entityManager.entityVector;
}

size_t EntityManagerGetVectorSize()
{
    return entityManager.entityVector->size;
}
Entity *EntityManagerGetArray()
{
    return ((Entity *)entityManager.entityVector->data);
}

size_t EntityManagerGetFreeIndex()
{
    for (int i = 1; i < MAX_ENTITY_INDICES; i++)
    {
        if (!entityManager.indices[i])
            return i;
    }
    return 0;
}

size_t EntityManagerGetHighestIndex()
{
    for (size_t i = MAX_ENTITY_INDICES - 1; i > 0; i--)
    {
        if (entityManager.indices[i])
            return i;
    }
    return 0;
}