#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#define MAX_ENTITIES 5000

#include <SDL2/SDL_bits.h>

#include "Entity.h"

typedef struct EntityManager
{
    Entity entities[MAX_ENTITIES];
    SDL_bool bitmap[MAX_ENTITIES];
    int highestIndex;
} EntityManager;

EntityManager *EntityManagerCreate();
void EntityManagerDestroy(EntityManager *entityManager);

///Waring only one
///\param entites: ALL entitys are needed
///@IMPORTANT the first entity is dominant whitch means it does the pyhsics first.
void EntityManagerUpdate(EntityManager *entityManager, Clock *clk);

void EntityManagerUpdateMovement(EntityManager *entityManager, Clock *clk);
/// finds the collision and fixes the problem with rules
SDL_bool EntityManagerOnCollision(EntityManager *entityManager);

Entity *EntityManagerAdd(EntityManager *entityManager, EntityType entityType, Vec2 position);
void EntityManagerRemove(EntityManager *entityManager, Entity *entity);

int EntityManagerGetFreeIndex(EntityManager *entityManager);
void EntityManagerCalculateHighestIndex(EntityManager *entityManager);

#endif