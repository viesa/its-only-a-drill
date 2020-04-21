#include "behavior.h"
#include "Entity.h"
#include <stdio.h>

#define addmove 1500.0f
#define moxsize 50

void BehaviorMoveEntity(Entity entities[])
{
    float axis_x1 = 200;
    float axis_y1 = 200;

    float axis_x2 = -200;
    float axis_y2 = -200;

    SDL_Rect box1, box2;
    box1 = (SDL_Rect){(int)axis_x1 - moxsize, (int)axis_y1 - moxsize, moxsize, moxsize};
    box2 = (SDL_Rect){(int)axis_x2 - moxsize, (int)axis_y2 - moxsize, moxsize, moxsize};
    for (int i = 0; i < 2; i++)
    {
        switch (entities[i].entityState)
        {
        case GoForward:
        {
            entities[i] = BehaviorMoveToPoint(entities[i], axis_x1, axis_y1);
            if (SDL_HasIntersection(&entities[i].drawable.dst, &box1))
            {
                entities[i].entityState = GoBack;
            }
            break;
        }

        case GoBack:
        {
            entities[i] = BehaviorMoveToPoint(entities[i], axis_x2, axis_y2);
            if (SDL_HasIntersection(&entities[i].drawable.dst, &box2))
            {
                entities[i].entityState = GoForward;
            }
            break;
        }
        case Fight:
        {
        }
        default:
            break;
        }
    }
}

Entity BehaviorMoveToPoint(Entity entity, float x, float y)
{
    float MovementSpeed = 900.0f;
    Vec2 nextPoint = Vec2Create(x, y);
    Vec2 enemyVec = Vec2Sub(nextPoint, RectMid(entity.drawable.dst));
    Vec2 unitenemyVec = Vec2Unit(enemyVec);
    Vec2 enemyMovement = Vec2MulL(unitenemyVec, MovementSpeed);
    entity.Force.x += enemyMovement.x;
    entity.Force.y += enemyMovement.y;
    return entity;
}