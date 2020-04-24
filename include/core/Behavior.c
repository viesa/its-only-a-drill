#include "Behavior.h"

#include "Dependencies.h"

#define addmove 1500.0f
#define moxsize 50 //boxsize

void BehaviorMoveEntity(EntityManager *entityManager)
{
    int amountOfEntitys = 8;    // ask??
    MoveingPattern mp;

    mp.point[0] = Vec2Create((float)200, (float)200);
    mp.point[1] = Vec2Create((float)283, (float)0);
    mp.point[2] = Vec2Create((float)200, (float)-200);
    mp.point[3] = Vec2Create((float)0, (float)-283);
    mp.point[4] = Vec2Create((float)-200, (float)200);
    mp.point[5] = Vec2Create((float)-283, (float)0);
    mp.point[6] = Vec2Create((float)-200, (float)-200);
    mp.point[7] = Vec2Create((float)50, (float)0);
    mp.point[8] = Vec2Create((float)150, (float)100);
    mp.point[9] = Vec2Create((float)200, (float)200);
    
    SDL_Rect boxDP;

    
    //box1 = (SDL_Rect){(int)axis_x1 - moxsize, (int)axis_y1 - moxsize, moxsize, moxsize};
    //box2 = (SDL_Rect){(int)axis_x2 - moxsize, (int)axis_y2 - moxsize, moxsize, moxsize};


    for (int i = 0; i < amountOfEntitys; i++)
    {
        boxDP = (SDL_Rect){(int)entities[i].desiredPoint.x - moxsize, (int)entities[i].desiredPoint.y - moxsize, moxsize, moxsize};

        switch (entities[i].entityState)
        {
            case GoForward:
            {
                    entities[i] = BehaviorMoveToPoint(entities[i], entities[i].desiredPoint.x, entities[i].desiredPoint.y);
                    if (SDL_HasIntersection(&entities[i].drawable.dst, &boxDP))
                    {
                        entities[i].desiredPoint.x -= 200;
                        entities[i].desiredPoint.y -= 200;
                        entities[i].entityState = GoBack;                        
                    }            
                break;
            }

            case GoBack:
            {
                entities[i] = BehaviorMoveToPoint(entities[i], entities[i].desiredPoint.x, entities[i].desiredPoint.y);
                if (SDL_HasIntersection(&entities[i].drawable.dst, &boxDP))
                {
                    entities[i].desiredPoint.x += 200;
                    entities[i].desiredPoint.y += 200;
                    entities[i].entityState = GoForward;
                }
                break;
            }

            case Fight:
            {

                break;
            }

            case EntityDead:
            {

                break;
            }
            case Aggressive:
            {
                entities[i] = BehaviorMoveToPoint(entities[i], entities[i].desiredPoint.x, entities[i].desiredPoint.y);
                if (SDL_HasIntersection(&entities[i].drawable.dst, &boxDP))
                {
                    Vec2Equ(entities[i].desiredPoint,mp.point[entities[i].indexPoint]);
                    if (entities[i].indexPoint == 9)
                    {
                        entities[i].indexPoint = 0;
                    } else
                    {
                        entities[i].indexPoint++;
                        //log_info("pointnr is : %d\n", entities[i].indexPoint);
                    }
                }
                break;
            }

            default:
                break;
        }
    }
}

Entity BehaviorMoveToPoint(Entity entity, float x, float y)
{
    float MovementSpeed = 500.0f;
    Vec2 nextPoint = Vec2Create(x, y);
    Vec2 enemyVec = Vec2Sub(nextPoint, RectMid(entity.drawables[0].dst));
    Vec2 unitenemyVec = Vec2Unit(enemyVec);
    Vec2 enemyMovement = Vec2MulL(unitenemyVec, MovementSpeed);
    entity.Force.x += enemyMovement.x;
    entity.Force.y += enemyMovement.y;
    return entity;
}