#include "Behavior.h"

#include "Dependencies.h"

#define addmove 1500.0f
#define boxSize 5 //boxsize
#define aggravationRadius 250.0f

MovingPattern *behaviorPathsCreate()
{
    MovingPattern *pattern = MALLOC(MovingPattern);
    //pattern->point[0] //när enemy kommer till desierd point -> pattern + pos = new desierd point
    pattern->point[0] = Vec2Create(200.0f, 200.0f);
    pattern->point[1] = Vec2Create(283.0f, 0.0f);
    pattern->point[2] = Vec2Create(200.0f, -200.0f);
    pattern->point[3] = Vec2Create(0.0f, -283.0f);
    pattern->point[4] = Vec2Create(-200.0f, 200.0f);
    pattern->point[5] = Vec2Create(-283.0f, 0.0f);
    pattern->point[6] = Vec2Create(-200.0f, -200.0f);
    pattern->point[7] = Vec2Create(50.0f, 0.0f);
    pattern->point[8] = Vec2Create(150.0f, 100.0f);
    pattern->point[9] = Vec2Create(200.0f, 200.0f);
    pattern->point[10] = Vec2Create(100.0f, 100.0f);
    pattern->point[11] = Vec2Create(-100.0f, -100.0f);

    return pattern;
}

void pathFree(MovingPattern *enemyP)
{
    SDL_free(enemyP);
}

void switchStateLogic(Vec2 *enemyToPlayer, int *i, EntityIndexP player)
{

    if (ENTITY_ARRAY[*i].health <= 0)
    {
        ENTITY_ARRAY[*i].entityState = EntityDead;
    }
    else
    {
        if (ENTITY_ARRAY[*i].health < 100)
        {
            ENTITY_ARRAY[*i].entityState = Aggressive;
        }
        if (Vec2Len(*enemyToPlayer) <= aggravationRadius)
        {
            if (ENTITY_ARRAY[*player].entityState != EntityDead)
            {
                ENTITY_ARRAY[*i].entityState = Fight;
            } else
            {
                ENTITY_ARRAY[*i].entityState = Neutral;
            }
        }
    }
}

void BehaviorMoveEntity(MovingPattern *pattern, SDL_Renderer *renderer, Camera *camera, EntityIndexP player)
{
    SDL_Rect boxDP;

    Vec2 enemyPosition, enemyToPlayer, playerPosition;

    playerPosition = RectMid(ENTITY_ARRAY[*player].drawables[*player].dst);

    for (int i = 1; i < ENTITY_ARRAY_SIZE; i++)
    {
        if (ENTITY_ARRAY[i].isNPC) // if there is an entity and npc = non playable charecter
        {
            boxDP = (SDL_Rect){(int)ENTITY_ARRAY[i].desiredPoint.x - boxSize, (int)ENTITY_ARRAY[i].desiredPoint.y - boxSize, boxSize, boxSize};

            enemyPosition = RectMid(ENTITY_ARRAY[i].drawables[0].dst);
            enemyToPlayer = Vec2Sub(playerPosition, enemyPosition);

            switchStateLogic(&enemyToPlayer, &i, player);

            switch (ENTITY_ARRAY[i].entityState)
            {
            case Passive:
            {
                // unused
                break;
            }
            case Neutral:
            {
                ENTITY_ARRAY[i] = BehaviorMoveToPoint(ENTITY_ARRAY[i], ENTITY_ARRAY[i].desiredPoint.x, ENTITY_ARRAY[i].desiredPoint.y);
                if (SDL_HasIntersection(&ENTITY_ARRAY[i].drawables[0].dst, &boxDP))
                {
                    ENTITY_ARRAY[i].desiredPoint = Vec2Add(enemyPosition, pattern->point[ENTITY_ARRAY[i].indexPoint]);
                    if (ENTITY_ARRAY[i].indexPoint == 11 || ENTITY_ARRAY[i].indexPoint <= 9)
                        ENTITY_ARRAY[i].indexPoint = 10;
                    else
                        ENTITY_ARRAY[i].indexPoint++;
                }
                break;
            }
            case Fight:
            {
                if (ENTITY_ARRAY[*player].entityState == EntityDead)
                {
                    ENTITY_ARRAY[i].entityState = Neutral;
                }
                if (ENTITY_ARRAY[i].isNPC == 1)
                {
                    // entityShoot(&i, playerPosition, &ENTITY_ARRAY[i].inventory.contents[ENTITY_ARRAY[i].inventory.top - 1], renderer, camera);
                }
                if (ENTITY_ARRAY[*player].health < 0)
                {
                    ENTITY_ARRAY[i].entityState = Neutral;
                }
                if (Vec2Len(enemyToPlayer) > aggravationRadius)
                {
                    ENTITY_ARRAY[i].entityState = Aggressive;
                }
                break;
            }

            case EntityDead:
            {
                // make mortal
                ENTITY_ARRAY[i].drawables[0] = DrawableCreate((SDL_Rect){0, 0, 70, 70}, (SDL_Rect){ENTITY_ARRAY[i].position.x, ENTITY_ARRAY[i].position.y, 77, 63}, SS_Character_Prisoner);
                //ENTITY_ARRAY[i].isNPC = 0;
                ENTITY_ARRAY[i].isCollider = 0;
                break;
            }
            case Aggressive:
            {
                ENTITY_ARRAY[i] = BehaviorMoveToPoint(ENTITY_ARRAY[i], ENTITY_ARRAY[i].desiredPoint.x, ENTITY_ARRAY[i].desiredPoint.y);
                if (SDL_HasIntersection(&ENTITY_ARRAY[i].drawables[0].dst, &boxDP))
                {
                    ENTITY_ARRAY[i].desiredPoint = Vec2Add(pattern->point[ENTITY_ARRAY[i].indexPoint], enemyPosition);
                    if (ENTITY_ARRAY[i].indexPoint >= 9)
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
