#include "Behavior.h"

#include "Dependencies.h"

#define addmove 1500.0f
#define moxsize 50 //boxsize
#define aggravationRadius 500.0f

void BehaviorMoveEntity()
{
    MovingPattern mp;
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

    Vec2 playerPosition = RectMid(ENTITY_ARRAY[0].drawables[0].dst);
    Vec2 enemyPosition, enemyToPlayer;

    for (int i = 0; i < ENTITY_ARRAY_SIZE; i++)
    {
        if (ENTITY_ARRAY[i].isNPC) // if there is an entity and npc = non playable charecter
        {
            boxDP = (SDL_Rect){(int)ENTITY_ARRAY[i].desiredPoint.x - moxsize, (int)ENTITY_ARRAY[i].desiredPoint.y - moxsize, moxsize, moxsize};

            enemyPosition = RectMid(ENTITY_ARRAY[i].drawables[0].dst);
            enemyToPlayer = Vec2Sub(enemyPosition,playerPosition);
            
            // if (Vec2Len(enemyToPlayer) < aggravationRadius)
            // {
            //     ENTITY_ARRAY[i].entityState = Fight;
            // }
            
            if (ENTITY_ARRAY[i].health < 100)  
            {
                if(ENTITY_ARRAY[i].entityState != Fight)
                {
                    ENTITY_ARRAY[i].entityState = Aggressive;
                }
            }
            switch (ENTITY_ARRAY[i].entityState)
            {
            case GoForward:
            {
                ENTITY_ARRAY[i] = BehaviorMoveToPoint(ENTITY_ARRAY[i], ENTITY_ARRAY[i].desiredPoint.x, ENTITY_ARRAY[i].desiredPoint.y);
                if (SDL_HasIntersection(&ENTITY_ARRAY[i].drawables[0].dst, &boxDP))
                {
                    ENTITY_ARRAY[i].desiredPoint.x -= 200;
                    ENTITY_ARRAY[i].desiredPoint.y -= 200;
                    ENTITY_ARRAY[i].entityState = GoBack;
                }
                break;
            }

            case GoBack:
            {
                ENTITY_ARRAY[i] = BehaviorMoveToPoint(ENTITY_ARRAY[i], ENTITY_ARRAY[i].desiredPoint.x, ENTITY_ARRAY[i].desiredPoint.y);
                if (SDL_HasIntersection(&ENTITY_ARRAY[i].drawables[0].dst, &boxDP))
                {
                    ENTITY_ARRAY[i].desiredPoint.x += 200;
                    ENTITY_ARRAY[i].desiredPoint.y += 200;
                    ENTITY_ARRAY[i].entityState = GoForward;
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
                ENTITY_ARRAY[i] = BehaviorMoveToPoint(ENTITY_ARRAY[i], ENTITY_ARRAY[i].desiredPoint.x, ENTITY_ARRAY[i].desiredPoint.y);
                if (SDL_HasIntersection(&ENTITY_ARRAY[i].drawables[0].dst, &boxDP))
                {
                    ENTITY_ARRAY[i].desiredPoint = mp.point[ENTITY_ARRAY[i].indexPoint];
                    if (ENTITY_ARRAY[i].indexPoint == 9)
                    {
                        ENTITY_ARRAY[i].indexPoint = 0;
                    }
                    else
                    {
                        ENTITY_ARRAY[i].indexPoint++;
                    }
                }
                break;
            }

            default:
                break;
            }
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