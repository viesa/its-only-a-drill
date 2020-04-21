#include "behavior.h"
#include "Entity.h"
#include <stdio.h>

void moveEntity(Entity entities[])
{
    for (int i = 0; i < 3; i++)
    {
        switch (entities[i].entityState)
        {
        case GoForward:
        {
            Vec2 nextPoint = Vec2Create(entities[i].position.x + 1000, entities[i].position.y);
            entities[i].Force.x += 500;
            if (entities[i].position.x = nextPoint.x)
            {
                entities[i].entityState = GoBack;
            }
            break;
        }

        case GoBack:
        {
            Vec2 backPoint = Vec2Create(entities[i].position.x - 1000, entities[i].position.y);
            entities[i].Force.x -= 500;
            if (entities[i].position.x = backPoint.x)
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
        // Vec2 nextPoint = Vec2Create(entities[i].position.x + 1000, entities[i].position.y + 1000);
        // Vec2 enemyVec = Vec2Sub(nextPoint, entities[i].position);
        // Vec2 unitenemyVec = Vec2Unit(enemyVec);
        // Vec2 enemyMovement = Vec2MulL(unitenemyVec, (float)500 );
        // entities[i].Force.x += enemyMovement.x;
        // entities[i].Force.y += enemyMovement.y;
    }
}

// void moveEntityFrom(Entity entities[])
// {
//     for (size_t i = 0; i < count; i++)
//     {
//         /* code */
//     }

// }