#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include "Dependencies.h"
#include "EntityManager.h"
#include "../math/Vec2.h"
#include "Library.h"
#include "Weapon.h"

typedef struct MovingPattern
{
    Vec2 point[10];
} MovingPattern;

//mode 0 move between two points 2modes!
void BehaviorMoveEntity(MovingPattern *Pattern);
Entity BehaviorMoveToPoint(Entity entity, float x, float y);
MovingPattern *behaviorPathsCreate();
void pathFree(MovingPattern *enemyP);

#endif