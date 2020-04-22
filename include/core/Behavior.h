#ifndef BEHAVIOR_H
#define BEHAVIOR_H
#include "Entity.h"
#include "../math/Vec2.h"
#include "Library.h"

typedef struct moveingPattern
{
    Vec2 point[10];
} moveingPattern;

//mode 0 move between two points 2modes!
void BehaviorMoveEntity(Entity entities[], int amountOfEntitys);
Entity BehaviorMoveToPoint(Entity entity, float x, float y);

#endif