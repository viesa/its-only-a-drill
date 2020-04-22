#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
#include "Entity.h"
#include <SDL2/SDL.h>
typedef struct EntityManager
{
    Entity *entities;
    int nrEntities;
} EntityManager;
EntityManager EntityManagerCreate();
void EntityManagerAdd(EntityManager *m, Entity ent);
void EntityManagerRemove(EntityManager *m, int index);
void EntityManagerDestroy(EntityManager *m);
#endif