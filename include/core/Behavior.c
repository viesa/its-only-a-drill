#include "Behavior.h"
#include <stdio.h>

#define addmove 1500.0f
#define moxsize 50

void BehaviorMoveEntity(EntityManager *entityManager)
{
    float axis_x1 = 200;
    float axis_y1 = 200;

    float axis_x2 = -200;
    float axis_y2 = -200;

    SDL_Rect box1, box2;
    box1 = (SDL_Rect){(int)axis_x1 - moxsize, (int)axis_y1 - moxsize, moxsize, moxsize};
    box2 = (SDL_Rect){(int)axis_x2 - moxsize, (int)axis_y2 - moxsize, moxsize, moxsize};
    for (int i = 0; i < entityManager->highestIndex; i++)
    {
        if (entityManager->bitmap[i] && entityManager->entities[i].isNPC)
        {
            switch (entityManager->entities[i].entityState)
            {
            case GoForward:
            {
                entityManager->entities[i] = BehaviorMoveToPoint(entityManager->entities[i], axis_x1, axis_y1);
                if (SDL_HasIntersection(&entityManager->entities[i].drawables[0].dst, &box1))
                {
                    entityManager->entities[i].entityState = GoBack;
                }
                break;
            }

            case GoBack:
            {
                entityManager->entities[i] = BehaviorMoveToPoint(entityManager->entities[i], axis_x2, axis_y2);
                if (SDL_HasIntersection(&entityManager->entities[i].drawables[0].dst, &box2))
                {
                    entityManager->entities[i].entityState = GoForward;
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
}

Entity BehaviorMoveToPoint(Entity entity, float x, float y)
{
    float MovementSpeed = 900.0f;
    Vec2 nextPoint = Vec2Create(x, y);
    Vec2 enemyVec = Vec2Sub(nextPoint, RectMid(entity.drawables[0].dst));
    Vec2 unitenemyVec = Vec2Unit(enemyVec);
    Vec2 enemyMovement = Vec2MulL(unitenemyVec, MovementSpeed);
    entity.Force.x += enemyMovement.x;
    entity.Force.y += enemyMovement.y;
    return entity;
}