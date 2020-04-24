#include "EntityManager.h"

EntityManager *EntityManagerCreate()
{
    EntityManager *entityManager = MALLOC(EntityManager);
    entityManager->highestIndex = 0;
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        entityManager->bitmap[i] = SDL_FALSE;
    }
    return entityManager;
}

void EntityManagerDestroy(EntityManager *entityManager)
{
    SDL_free(entityManager);
}

void EntityManagerUpdate(EntityManager *entityManager, Clock *clk)
{
    EntityManagerUpdateMovement(entityManager, clk);
    EntityManagerOnCollision(entityManager);
}

void EntityManagerUpdateMovement(EntityManager *entityManager, Clock *clk)
{
    for (int i = 0; i < entityManager->highestIndex; i++)
    {
        if (entityManager->bitmap[i])
        { // carculate Net_force so friction & collision & the other forces is handle before
            EntityCalculateNetForces(&entityManager->entities[i]);

            // Garante stop Cause float
            entityManager->entities[i].Velocity = Vec2DivL(entityManager->entities[i].Force, entityManager->entities[i].mass);
            entityManager->entities[i].Velocity.x = (fabs(entityManager->entities[i].Velocity.x) < 7.8f) ? 0 : entityManager->entities[i].Velocity.x;
            entityManager->entities[i].Velocity.y = (fabs(entityManager->entities[i].Velocity.y) < 7.8f) ? 0 : entityManager->entities[i].Velocity.y;

            // update new position
            entityManager->entities[i].position.x += entityManager->entities[i].Velocity.x * ClockGetDeltaTime(clk);
            entityManager->entities[i].position.y += entityManager->entities[i].Velocity.y * ClockGetDeltaTime(clk);
#ifdef DegBug
            if (entities[i].id == 0)
            {
                log_debug("CurrentEntity:");
                log_debug("position x: %f", entities[i].position.x);
                log_debug("position y: %f", entities[i].position.y);
                log_debug("Force x: %f", entities[i].Force.x);
                log_debug("Force y: %f", entities[i].Force.y);
                log_debug("Acceleration x: %f", entities[i].Acceleration.x);
                log_debug("Acceleration y: %f", entities[i].Acceleration.y);
                log_debug("Velocity x: %f", entities[i].Velocity.x);
                log_debug("Velocity y: %f", entities[i].Velocity.y);
                log_debug("Friction: %f", entities[i].Friction);
                log_debug("mass: %f", entities[i].mass);
            }
#endif
        }
    }
}

void EntityManagerOnCollision(EntityManager *entityManager)
{
    SDL_Rect result;
    for (int Dominant = 0; Dominant < entityManager->highestIndex; Dominant++)
    {
        if (entityManager->bitmap[Dominant] &&
            entityManager->entities[Dominant].isCollider)
            for (int Recessive = 0; Recessive < entityManager->highestIndex; Recessive++)
            {
                if (entityManager->bitmap[Recessive] &&
                    entityManager->entities[Recessive].isCollider)
                {
                    if (Dominant != Recessive)
                    {
                        // Dominant hitbox
                        SDL_Rect *dHitbox = &entityManager->entities[Dominant].drawables[entityManager->entities[Dominant].hitboxIndex].dst;
                        // Recessive hitbox
                        SDL_Rect *rHitbox = &entityManager->entities[Recessive].drawables[entityManager->entities[Recessive].hitboxIndex].dst;
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

                            entityManager->entities[Recessive].position.x -= ResultDistance.x / 17.0f; // the reason it's divide in to 17 pices it's to avoid the heartbeat affect
                            entityManager->entities[Recessive].position.y -= ResultDistance.y / 17.0f;
                        }
                    }
                }
            }
    }
}

Entity *EntityManagerAdd(EntityManager *entityManager, EntityType entityType, Vec2 position)
{
    Entity entity = EntityCreate(position, entityType, 0);
    int index = EntityManagerGetFreeIndex(entityManager);
    if (index == -1)
    {
        log_fatal("EntityManager is FULL!");
        return NULL;
    }
    {
        entityManager->entities[index] = entity;
        entityManager->bitmap[index] = SDL_TRUE;
        EntityManagerCalculateHighestIndex(entityManager);
        return &entityManager->entities[index];
    }
}

void EntityManagerRemove(EntityManager *entityManager, Entity *entity)
{
    int index = -1;
    for (int i = 0; i < entityManager->highestIndex; i++)
    {
        if (entityManager->bitmap[i] && entity == &entityManager->entities[i])
        {
            index = i;
        }
    }
    if (index == -1)
        return;

    entityManager->bitmap[index] = SDL_FALSE;
    EntityManagerCalculateHighestIndex(entityManager);
}

int EntityManagerGetFreeIndex(EntityManager *entityManager)
{
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        if (!entityManager->bitmap[i])
            return i;
    }
    return -1;
}

void EntityManagerCalculateHighestIndex(EntityManager *entityManager)
{
    for (int i = MAX_ENTITIES; i >= 0; i--)
    {
        if (entityManager->bitmap[i])
        {
            entityManager->highestIndex = i;
            return;
        }
    }
    entityManager->highestIndex = 0;
}