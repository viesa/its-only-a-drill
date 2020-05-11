#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include "Dependencies.h"
#include "EntityManager.h"
#include "Vec2.h"
#include "Library.h"
#include "Weapon.h"

typedef struct MovingPattern
{
    Vec2 point[20];
} MovingPattern;

// BehaviorMoveEntity is the update funktion for the NPC(non playable charecter) logic
void BehaviorMoveEntity(MovingPattern *Pattern, SDL_Renderer *renderer, Camera *camera, Player *player);
// gives the entity a defined speed to the given point
Entity BehaviorMoveToPoint(Entity entity, float x, float y);
// creates the current path that entitys can use
MovingPattern *behaviorPathsCreate();
// removes it form memory
void pathFree(MovingPattern *enemyP);
// the logic for how enemys changes states
void switchStateLogic(Vec2 *enemyToPlayer, int *i, EntityIndexP player);

#endif