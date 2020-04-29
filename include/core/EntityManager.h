#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "Dependencies.h"
#include "Entity.h"
#include "Vector.h"

#define ENTITY_VECTOR EntityManagerGetVector()
#define ENTITY_ARRAY_SIZE EntityManagerGetVectorSize()
#define ENTITY_ARRAY EntityManagerGetArray()

void EntityManagerInitialize();
void EntityManagerUninitalize();

///Waring only one
///\param entites: ALL entitys are needed
///@IMPORTANT the first entity is dominant whitch means it does the pyhsics first.
void EntityManagerUpdate(Clock *clk);

void EntityManagerUpdateMovement(Clock *clk);
/// finds the collision and fixes the problem with rules
void EntityManagerOnCollision();

EntityIndexP EntityManagerAdd(EntityType entityType, Vec2 position);
EntityIndexP EntityManagerAddNoConfig();
void EntityManagerRemove(EntityIndexP index);
void EntityManagerRemoveRange(EntityIndexP start, EntityIndexP end);

Vector *EntityManagerGetVector();
size_t EntityManagerGetVectorSize();
Entity *EntityManagerGetArray();

size_t EntityManagerGetFreeIndex();
size_t EntityManagerGetHighestIndex();

#endif