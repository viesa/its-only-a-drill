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
                            entities[j].vector.x += result.w;
                        }
                        else
                        {
                            entities[j].vector.y += result.h;
                        }
                    }
                }
            }
        }
    }
    //             if (SDL_IntersectRect(&entities[i].drawable.dst, &entities[j].drawable.dst, &result))
    //             {
    //                 if (entities[j].isMovable == SDL_TRUE)
    //                 {
    //                     if (result.w < result.h) // assume the smallest number is the interaction point
    //                     {
    //                         entities[j].vector.x += result.w;
    //                     }
    //                     else
    //                     {
    //                         entities[j].vector.y += result.h;
    //                     }
    //                 }
    //                 else
    //                 {
    //                     if (result.w < result.h) // assume the smallest number is the interaction point
    //                     {
    //                         entities[i].vector.x -= result.w;
    //                     }
    //                     else
    //                     {
    //                         entities[i].vector.y -= result.h;
    //                     }
    //                 }
    //             }
    //             else
    //             {
    //                 return SDL_FALSE;
    //             }
    //         }
    //     }
    // }
    // return SDL_TRUE;
    // all ents
    // loop
    // has interset = result
    // if ent = movable
    // do result player --> ent
    // else
    // do result ent --> player

    // Entity test = user; // player = ent
    // if (test.move_x > 0)
    // {
    //     test.vector.x += test.moveSpeed * ClockGetDeltaTime(clk);
    //     test.move_x -= 1;

    //     for (int i = 0; i < nrEnts; i++)
    //     {
    //         if (i != nrSelfIndex && entities[i].isCollider)
    //         {
    //             if (SDL_IntersectRect(&entities[i].drawable.dst, &test.drawable.dst, &(SDL_Rect){0, 0, 0, 0}))
    //             {
    //                 //connects with first thing
    //                 if (entities[i].isMovable)
    //                 {
    //                     entities[i].vector.x = test.vector.x + test.drawable.dst.w;
    //                 }
    //             }
    //         }
    //     }
    // }
    // if (test.move_x < 0)
    // {
    //     test.vector.x -= test.moveSpeed * ClockGetDeltaTime(clk);
    //     test.move_x += 1;

    //     for (int i = 0; i < nrEnts; i++)
    //     {
    //         if (i != nrSelfIndex && entities[i].isCollider)
    //         {
    //             if (SDL_IntersectRect(&entities[i].drawable.dst, &test.drawable.dst, &(SDL_Rect){0, 0, 0, 0}))
    //             {
    //                 //connects
    //                 if (entities[i].isMovable)
    //                 {
    //                     entities[i].vector.x = test.vector.x - test.drawable.dst.w;
    //                 }
    //             }
    //         }
    //     }
    // }
    // if (test.move_y > 0)
    // {
    //     test.vector.y += test.moveSpeed * ClockGetDeltaTime(clk);
    //     test.move_y -= 1;

    //     for (int i = 0; i < nrEnts; i++)
    //     {
    //         if (i != nrSelfIndex && entities[i].isCollider)
    //         {
    //             if (SDL_IntersectRect(&entities[i].drawable.dst, &test.drawable.dst, &(SDL_Rect){0, 0, 0, 0}))
    //             {
    //                 //connects
    //                 if (entities[i].isMovable)
    //                 {
    //                     entities[i].vector.y = test.vector.y + test.drawable.dst.h;
    //                 }
    //             }
    //         }
    //     }
    // }
    // if (test.move_y < 0)
    // {
    //     test.vector.y -= test.moveSpeed * ClockGetDeltaTime(clk);
    //     test.move_y += 1;

    //     for (int i = 0; i < nrEnts; i++)
    //     {
    //         if (i != nrSelfIndex && entities[i].isCollider)
    //         {
    //             if (SDL_IntersectRect(&entities[i].drawable.dst, &test.drawable.dst, &(SDL_Rect){0, 0, 0, 0}))
    //             {
    //                 //connects
    //                 if (entities[i].isMovable)
    //                 {
    //                     entities[i].vector.y = test.vector.y - test.drawable.dst.h;
    //                 }
    //             }
    //         }
    //     }
    // }
    // for (int i = 0; i < nrEnts; i++)
    // {
    //     if (i != nrSelfIndex && entities[i].isCollider)
    //     {
    //         if (SDL_IntersectRect(&entities[i].drawable.dst, &test.drawable.dst, &(SDL_Rect){0, 0, 0, 0}))
    //         {
    //             //connects
    //             return SDL_TRUE;
    //         }
    //     }
    // }
    // return SDL_FALSE;
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