#include "Entity.h"

Entity EntityCreate(Vec2 posVec, int moveSpeed, int rotSpeed, EntityPresets preset, int uniqueIdentifier)
{
    Entity e;
    e.id = uniqueIdentifier;
    e.posVec = posVec;
    e.velocity.x = 0;
    e.velocity.x = 0;
    switch (preset)
    {
    case EntityWoman:
        e.moveSpeed = moveSpeed;
        e.rotSpeed = rotSpeed;
        e.isCollider = SDL_TRUE;
        e.isMovable = SDL_TRUE;
        e.drawable = DrawableCreate((SDL_Rect){0, 44, 57, 43}, (SDL_Rect){e.posVec.x, e.posVec.y, 57, 43}, SS_Characters);
        break;

    default:
        break;
    }
    return e;
}
SDL_bool EntityOnCollision(Entity entities[], int nrEnts, Entity user, Clock *clk)
{
    SDL_Rect result;
    for (int i = 0; i < nrEnts; i++)
    {
        for (int j = 0; j < nrEnts; j++)
        {
            if (i != j)
            {
                if (SDL_IntersectRect(&entities[i].drawable.dst, &entities[j].drawable.dst, &result))
                {
                    if (entities[j].isMovable == SDL_TRUE)
                    {
                        if (result.w < result.h) // assume the smallest number is the interaction point
                        {
                            entities[j].velocity.x += result.w;
                        }
                        else
                        {
                            entities[j].velocity.y += result.h;
                        }
                    }
                }
            }
        }
    }
    return SDL_FALSE;
}
void EntityUpdate(Entity entities[], int nrEnts, Entity *user, Clock *clk)
{
    if (entities)
    {
        if (user->isCollider || user->isMovable)
        {
            if (EntityOnCollision(entities, nrEnts, *user, clk))
                return;
        }
    }

    if (user->velocity.x > 0)
    {
        user->posVec.x += user->moveSpeed * ClockGetDeltaTime(clk);
        user->velocity.x -= 1;
    }
    if (user->velocity.x < 0)
    {
        user->posVec.x -= user->moveSpeed * ClockGetDeltaTime(clk);
        user->velocity.x += 1;
    }
    if (user->velocity.y > 0)
    {
        user->posVec.y += user->moveSpeed * ClockGetDeltaTime(clk);
        user->velocity.y -= 1;
    }
    if (user->velocity.y < 0)
    {
        user->posVec.y -= user->moveSpeed * ClockGetDeltaTime(clk);
        user->velocity.y += 1;
    }
    if (user->rot > 0)
    {
        user->drawable.rot += user->rotSpeed * ClockGetDeltaTime(clk);
        user->rot -= 1;
    }
    if (user->rot < 0)
    {
        user->drawable.rot -= user->rotSpeed * ClockGetDeltaTime(clk);
        user->rot += 1;
    }
}
void EntityDraw(Camera *camera, Entity *entity)
{
    entity->drawable.dst.x = entity->posVec.x;
    entity->drawable.dst.y = entity->posVec.y;
    CameraDraw(camera, entity->drawable);
}

/*

    ent +=1;
    ent.vol += 1; // f=ma

*/