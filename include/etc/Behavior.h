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

//mode 0 move between two points 2modes!
void BehaviorMoveEntity(MovingPattern *Pattern, SDL_Renderer *renderer, Camera *camera);
Entity BehaviorMoveToPoint(Entity entity, float x, float y);
MovingPattern *behaviorPathsCreate();
void pathFree(MovingPattern *enemyP);
void switchStateLogic(Vec2 *enemyToPlayer, int *i);

#endif