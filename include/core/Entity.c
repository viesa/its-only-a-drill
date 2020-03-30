#include "Entity.h"
#include <stdio.h>

Entity EntityCreate(Vec2 position, int moveSpeed, int rotSpeed, EntityPresets preset, int uniqueIdentifier)
{
    Entity e;
    e.id = uniqueIdentifier;
    e.position = position;
    e.Force.x = 0;
    e.Force.x = 0;
    switch (preset)
    {
    case EntityWoman:
        e.moveSpeed = moveSpeed;
        e.rotSpeed = rotSpeed;
        e.Friction = 0.05f;
        e.mass = 100.0f;
        e.Force.x = 0;
        e.Force.y = 0;
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
                            if (result.w < result.h) // assume the smallest number is the interaction point
                            {
                                // ent i --> ent j
                                // i -> F -> j
                                //entities[Recessive].Force.x += (entities[Dominant].Force.x / 2);
                                //entities[Dominant].Force.x -= (entities[Dominant].Force.x / 2);
                                //entities[Dominant].Force.x = (entities[Dominant].Force.x - (entities[Dominant].Force.x * entities[Dominant].Friction));
                                entities[Dominant].Force.x = entities[Dominant].Force.x * -1;
                                entities[Recessive].Force.x += entities[Dominant].Force.x;
                                //entities[j].Force.x = entities[j].Force.x / 2;
                            }
                            else
                            {
                                //entities[Recessive].Force.y += result.h;
                                entities[Dominant].Force.y = entities[Dominant].Force.y * -1;
                                entities[Recessive].Force.y += entities[Dominant].Force.y;
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

    for (int i; i < nrEnts; i++)
    {
        //This part is the carculation of the force on the current entity.
        //It uses F=MA and changes it to A=F/M and then applys it on position           it wont be perfect but it dose what it should do
        entities[i].Force.x -= entities[i].Force.x * entities[i].Friction; // <--- if delta time will be added add here
        entities[i].Force.y -= entities[i].Force.y * entities[i].Friction; // <--- if delta time will be added add here
        entities[i].accseleration.x = entities[i].Force.x / entities[i].mass;
        entities[i].accseleration.y = entities[i].Force.y / entities[i].mass;
        entities[i].position.x += entities[i].accseleration.x;
        entities[i].position.y += entities[i].accseleration.y;
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

/*

    ent +=1;
    ent.vol += 1; // f=ma

*/