#include "Entity.h"
#include <stdio.h>

#define Replaceable_Transfers_force -1

Entity EntityCreate(Vec2 position, int moveSpeed, int rotSpeed, EntityPresets preset, int uniqueIdentifier)
{
    Entity e;
    e.id = uniqueIdentifier;
    e.position = position;
    e.Force.x = 0;
    e.Force.y = 0;
    e.moveSpeed = moveSpeed;
    e.rotSpeed = rotSpeed;
    switch (preset)
    {
    case EntityWoman:
        e.Friction = 0.05f;
        e.mass = 50.0f;
        e.isCollider = SDL_TRUE;
        e.isMovable = SDL_TRUE;
        e.drawable = DrawableCreate((SDL_Rect){0, 44, 57, 43}, (SDL_Rect){e.position.x, e.position.y, 57, 43}, SS_Characters);
        break;
    default:
        break;
    }
    return e;
}
SDL_bool EntityOnCollision(Entity entities[], int nrEnts, Clock *clk)
{
    SDL_Rect result;
    for (int Dominant = 0; Dominant < nrEnts; Dominant++)
    {
        for (int Recessive = 0; Recessive < nrEnts; Recessive++)
        {
            if (entities[Recessive].isCollider || entities[Recessive].isMovable)
            {
                if (Dominant != Recessive)
                {
                    if (SDL_IntersectRect(&entities[Dominant].drawable.dst, &entities[Recessive].drawable.dst, &result))
                    {
                        if (entities[Recessive].isMovable == SDL_TRUE)
                        {
                            // basic if collision give the force but inverted
                            // Replaceable transfers force is -1 default, witch means that we don't lose or gain force if you want to be slower after hit make it -0.X minus is importend
                            entities[Dominant].Force.x = entities[Dominant].Force.x * Replaceable_Transfers_force;
                            entities[Recessive].Force.x += entities[Dominant].Force.x;
                            entities[Dominant].Force.y = entities[Dominant].Force.y * Replaceable_Transfers_force;
                            entities[Recessive].Force.y += entities[Dominant].Force.y;

                            if (result.w < result.h)
                            {
                                entities[Dominant].position.x += result.w;
                            }
                            else
                            {
                                entities[Dominant].position.y += result.h;
                            }
                        }
                    }
                }
            }
        }
    }
    return SDL_FALSE;
}
void EntityUpdate(Entity entities[], int nrEnts, Clock *clk)
{

    EntityOnCollision(entities, nrEnts, clk);

    for (int i = 0; i < nrEnts; i++)
    {
        //This part is the carculation of the force on the current entity.
        //It uses F=MA and changes it to A=F/M and then applys it on position
        entities[i].Force.x -= entities[i].Force.x * entities[i].Friction;
        entities[i].Force.y -= entities[i].Force.y * entities[i].Friction;
        entities[i].accseleration.x = entities[i].Force.x / entities[i].mass;
        entities[i].accseleration.y = entities[i].Force.y / entities[i].mass;
        entities[i].position.x += entities[i].accseleration.x * ClockGetDeltaTime(clk);
        entities[i].position.y += entities[i].accseleration.y * ClockGetDeltaTime(clk);
        //entities[i].Force = Vec2MulL(&entities[i].Force, *entities[i].Friction);
        //entities[i].accseleration = Vec2DivL(&entities[i].Force, entities[i].mass);

        // for debug plz do not erase me ;_;
        // if (entities[i].id == 0)
        // {
        //     printf("CurrentEntity:\n");
        //     printf("position x: %f\n", entities[i].position.x);
        //     printf("position y: %f\n", entities[i].position.y);
        //     printf("Force x: %f\n", entities[i].Force.x);
        //     printf("Force y: %f\n", entities[i].Force.y);
        //     printf("accseleration x: %f\n", entities[i].accseleration.x);
        //     printf("accseleration y: %f\n", entities[i].accseleration.y);
        //     printf("Friction: %f\n", entities[i].Friction);
        //     printf("mass: %f\n", entities[i].mass);
        // }
    }
}
void EntityDraw(Camera *camera, Entity *entity)
{
    entity->drawable.dst.x = entity->position.x;
    entity->drawable.dst.y = entity->position.y;
    CameraDraw(camera, entity->drawable);
}
// physical forumals in progress
// basic elastic collision the problem is it needs generlasation

// entities[Dominant].accseleration.x -= (((entities[Dominant].mass - entities[Recessive].mass) / (entities[Dominant].mass + entities[Recessive].mass)) * entities[Dominant].accseleration.x) +
//                                       (((2 * entities[Recessive].mass) / (entities[Dominant].mass + entities[Recessive].mass)) * entities[Recessive].accseleration.x);
// entities[Dominant].accseleration.y -= (((entities[Dominant].mass - entities[Recessive].mass) / (entities[Dominant].mass + entities[Recessive].mass)) * entities[Dominant].accseleration.y) +
//                                       (((2 * entities[Recessive].mass) / (entities[Dominant].mass + entities[Recessive].mass)) * entities[Recessive].accseleration.y);

// entities[Recessive].accseleration.x -= (((2 * entities[Recessive].mass) / (entities[Dominant].mass + entities[Recessive].mass)) * entities[Dominant].accseleration.x) +
//                                        (((entities[Recessive].mass - entities[Dominant].mass) / (entities[Dominant].mass + entities[Recessive].mass)) * entities[Recessive].accseleration.x);
// entities[Recessive].accseleration.y -= (((2 * entities[Recessive].mass) / (entities[Dominant].mass + entities[Recessive].mass)) * entities[Dominant].accseleration.y) +
//                                        (((entities[Recessive].mass - entities[Dominant].mass) / (entities[Dominant].mass + entities[Recessive].mass)) * entities[Recessive].accseleration.y);

// printf("Dominant x:%f\n", (((entities[Dominant].mass - entities[Recessive].mass) / (entities[Dominant].mass + entities[Recessive].mass)) * entities[Dominant].accseleration.x) +
//                               (((2 * entities[Recessive].mass) / (entities[Dominant].mass + entities[Recessive].mass)) * entities[Recessive].accseleration.x));
// printf("Dominant y:%f\n", (((entities[Dominant].mass - entities[Recessive].mass) / (entities[Dominant].mass + entities[Recessive].mass)) * entities[Dominant].accseleration.y) +
//                               (((2 * entities[Recessive].mass) / (entities[Dominant].mass + entities[Recessive].mass)) * entities[Recessive].accseleration.y));
// printf("Recessive x:%f\n", (((2 * entities[Recessive].mass) / (entities[Dominant].mass + entities[Recessive].mass)) * entities[Dominant].accseleration.x) +
//                                (((entities[Recessive].mass - entities[Dominant].mass) / (entities[Dominant].mass + entities[Recessive].mass)) * entities[Recessive].accseleration.x));
// printf("Recessive y:%f\n", (((2 * entities[Recessive].mass) / (entities[Dominant].mass + entities[Recessive].mass)) * entities[Dominant].accseleration.y) +
//                                (((entities[Recessive].mass - entities[Dominant].mass) / (entities[Dominant].mass + entities[Recessive].mass)) * entities[Recessive].accseleration.y));