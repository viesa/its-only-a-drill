#include "EntityManager.h"
#include "Client.h"
#define MAX_ENTITY_INDICES 10000

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
        // carculate Net_force so friction & collision & the other forces is handle before
        EntityCalculateNetForces(&ENTITY_ARRAY[i]);

        // Garante stop Cause float
        ENTITY_ARRAY[i].Velocity = Vec2DivL(ENTITY_ARRAY[i].Force, ENTITY_ARRAY[i].mass);
        ENTITY_ARRAY[i].Velocity.x = (fabs(ENTITY_ARRAY[i].Velocity.x) < 7.8f) ? 0 : ENTITY_ARRAY[i].Velocity.x;
        ENTITY_ARRAY[i].Velocity.y = (fabs(ENTITY_ARRAY[i].Velocity.y) < 7.8f) ? 0 : ENTITY_ARRAY[i].Velocity.y;

        // update new position
        ENTITY_ARRAY[i].position.x += ENTITY_ARRAY[i].Velocity.x * ClockGetDeltaTime();
        ENTITY_ARRAY[i].position.y += ENTITY_ARRAY[i].Velocity.y * ClockGetDeltaTime();
        if (Vec2LenSq(ENTITY_ARRAY[i].Velocity) > 0.1f &&
            ENTITY_ARRAY[i].id >= 10000)
        {
            CompressedEntity cEntity = EntityCompress(&ENTITY_ARRAY[i]);
            ClientUDPSend(PT_CompressedEntity, &cEntity, sizeof(CompressedEntity));
        }
#ifdef ENTITY_DEBUG
        if (i == 0)
        {
            log_debug("CurrentEntity:");
            log_debug("position x: %f", ENTITY_ARRAY[i].position.x);
            log_debug("position y: %f", ENTITY_ARRAY[i].position.y);
            log_debug("Force x: %f", ENTITY_ARRAY[i].Force.x);
            log_debug("Force y: %f", ENTITY_ARRAY[i].Force.y);
            log_debug("Acceleration x: %f", ENTITY_ARRAY[i].Acceleration.x);
            log_debug("Acceleration y: %f", ENTITY_ARRAY[i].Acceleration.y);
            log_debug("Velocity x: %f", ENTITY_ARRAY[i].Velocity.x);
            log_debug("Velocity y: %f", ENTITY_ARRAY[i].Velocity.y);
            log_debug("Friction: %f", ENTITY_ARRAY[i].Friction);
            log_debug("mass: %f", ENTITY_ARRAY[i].mass);
        }
#endif
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

                            Vec2 ResultDistance; // Calculating distance
                            ResultDistance.x = DominantCenter.x - RecessiveCenter.x;
                            ResultDistance.y = DominantCenter.y - RecessiveCenter.y;

                            ENTITY_ARRAY[Recessive].position.x -= ResultDistance.x / 17.0f;
                            ENTITY_ARRAY[Recessive].position.y -= ResultDistance.y / 17.0f;

                            ENTITY_ARRAY[Recessive].Force.x -= ResultDistance.x;
                            ENTITY_ARRAY[Recessive].Force.y -= ResultDistance.y;

                            ENTITY_ARRAY[Dominant].Force.x += ResultDistance.x;
                            ENTITY_ARRAY[Dominant].Force.y += ResultDistance.y;
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