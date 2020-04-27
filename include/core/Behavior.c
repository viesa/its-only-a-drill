#include "Behavior.h"

#include "Dependencies.h"

#define addmove 1500.0f
#define moxsize 50 //boxsize
#define aggravationRadius 500.0f

void BehaviorMoveEntity(EntityManager *entityManager)
{
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

    Vec2 playerPosition = RectMid(entityManager->entities[0].drawables[0].dst);
    Vec2 enemyPosition, enemyToPlayer;
    
    for (int i = 0; i < entityManager->highestIndex; i++)
    {
        if (entityManager->bitmap[i] && entityManager->entities[i].isNPC) // if there is an entity and npc = non playable charecter
        {
            boxDP = (SDL_Rect){(int)entityManager->entities[i].desiredPoint.x - moxsize, (int)entityManager->entities[i].desiredPoint.y - moxsize, moxsize, moxsize};
            
            enemyPosition = RectMid(entityManager->entities[i].drawables[0].dst);
            enemyToPlayer = Vec2Sub(enemyPosition,playerPosition);
            
            // if (Vec2Len(enemyToPlayer) < aggravationRadius)
            // {
            //     entityManager->entities[i].entityState = Fight;
            // }
            
            if (entityManager->entities[i].health < 100)  
            {
                if(entityManager->entities[i].entityState != Fight)
                {
                    entityManager->entities[i].entityState = Aggressive;
                }
            }
            
            switch (entityManager->entities[i].entityState)
            {
                case GoForward:
                {
                    
                    entityManager->entities[i] = BehaviorMoveToPoint(entityManager->entities[i], entityManager->entities[i].desiredPoint.x, entityManager->entities[i].desiredPoint.y);
                    if (SDL_HasIntersection(&entityManager->entities[i].drawables[0].dst, &boxDP))
                    {
                        entityManager->entities[i].desiredPoint.x -= 200;
                        entityManager->entities[i].desiredPoint.y -= 200;
                        entityManager->entities[i].entityState = GoBack;                        
                    }            
                    break;
                }

                case GoBack:
                {
                    entityManager->entities[i] = BehaviorMoveToPoint(entityManager->entities[i], entityManager->entities[i].desiredPoint.x, entityManager->entities[i].desiredPoint.y);
                    if (SDL_HasIntersection(&entityManager->entities[i].drawables[0].dst, &boxDP))
                    {
                        entityManager->entities[i].desiredPoint.x += 200;
                        entityManager->entities[i].desiredPoint.y += 200;
                        entityManager->entities[i].entityState = GoForward;
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
                    entityManager->entities[i] = BehaviorMoveToPoint(entityManager->entities[i], entityManager->entities[i].desiredPoint.x, entityManager->entities[i].desiredPoint.y);
                    if (SDL_HasIntersection(&entityManager->entities[i].drawables[0].dst, &boxDP))
                    {                        
                        entityManager->entities[i].desiredPoint = mp.point[entityManager->entities[i].indexPoint];
                        if (entityManager->entities[i].indexPoint == 9)
                        {
                            entityManager->entities[i].indexPoint = 0;
                        } else
                        {
                            entityManager->entities[i].indexPoint++;
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