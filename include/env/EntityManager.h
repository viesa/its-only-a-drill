#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "Dependencies.h"
#include "Vector.h"
#include "Entity.h"
#include "CollisionDetection.h"

#define ENTITY_VECTOR EntityManagerGetVector()
#define ENTITY_ARRAY_SIZE EntityManagerGetVectorSize()
#define ENTITY_ARRAY EntityManagerGetArray()

void EntityManagerInitialize();
void EntityManagerUninitialize();

///Waring only one
///@IMPORTANT the first entity is dominant whitch means it does the pyhsics first.
void EntityManagerUpdate();

void EntityManagerUpdateMovement();
/// Finds the collision and fixes the problem with rules
void EntityManagerOnCollision();

EntityIndexP EntityManagerAdd(EntityType entityType, Vec2 position);
EntityIndexP EntityManagerAddNoConfig();
void EntityManagerRemove(EntityIndexP index);
void EntityManagerRemoveRange(size_t start, size_t end);

Vector *EntityManagerGetVector();
size_t EntityManagerGetVectorSize();
Entity *EntityManagerGetArray();

size_t EntityManagerGetFreeIndex();
size_t EntityManagerGetHighestIndex();

#endif