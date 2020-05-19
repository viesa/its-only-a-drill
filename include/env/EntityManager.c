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

void EntityManagerUninitialize()
{
    VectorDestroy(entityManager.entityVector);
    FREE(entityManager.indices);
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
        if (ENTITY_ARRAY[i].isMovable)
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
}

void EntityManagerOnCollision()
{
    for (int Dominant = 1; Dominant < ENTITY_ARRAY_SIZE; Dominant++)
    {
        Entity *dEntity = &ENTITY_ARRAY[Dominant];
        if (dEntity->isCollider && dEntity->isMovable)
        {
            for (int Recessive = 1; Recessive < ENTITY_ARRAY_SIZE; Recessive++)
            {
                Entity *rEntity = &ENTITY_ARRAY[Recessive];
                if (rEntity->isCollider && Dominant != Recessive)
                {
                    // Dominant hitbox
                    SDL_Rect dHitbox = EntityGetHitbox(dEntity);
                    // Recessive hitbox
                    SDL_Rect rHitbox = EntityGetHitbox(rEntity);
                    // Test intersection
                    if (CollisionBoxWithBox_AABB(&dHitbox, &rHitbox))
                    {
                        // Calculating centers
                        Vec2 DominantCenter = RectMid(dHitbox);
                        Vec2 RecessiveCenter = RectMid(rHitbox);

                        // Direction of impact (cemter to center)
                        Vec2 ResultDirection = Vec2Unit(Vec2Sub(DominantCenter, RecessiveCenter));
                        // If dominant-center == recessive-center, pretend impact was sideways
                        if (Vec2LenSq(ResultDirection) == 0.0f)
                            ResultDirection = Vec2Create(1.0f, 0.0f);

                        float *dMass = &dEntity->mass;
                        float *rMass = &rEntity->mass;
                        // If dominant is not movable, make it's mass simulate infinity
                        float massRatioBasedOnDominant = !dEntity->isMovable ? 0.0f : *dMass / (*dMass + *rMass);

                        // Carefully step back where the rects did not intesect
                        for (const float stepSize = 0.05f; CollisionBoxWithBox_AABB(&dHitbox, &rHitbox);)
                        {
                            dEntity->position.x += ResultDirection.x * (1.0f - massRatioBasedOnDominant) * stepSize;
                            dEntity->position.y += ResultDirection.y * (1.0f - massRatioBasedOnDominant) * stepSize;
                            rEntity->position.x -= ResultDirection.x * massRatioBasedOnDominant * stepSize;
                            rEntity->position.y -= ResultDirection.y * massRatioBasedOnDominant * stepSize;
                            // Update hitboxes with new positions
                            dHitbox = EntityGetHitbox(dEntity);
                            rHitbox = EntityGetHitbox(rEntity);
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