#include "Entity.h"
#include <stdio.h>

//#define DegBug
#define FrictionMode 

Entity EntityCreate(Vec2 position, EntityPresets preset, int uniqueIdentifier)
{
    Entity e;
    e.id = uniqueIdentifier;
    e.position = position;
    e.Force = Vec2Create(0.0f, 0.0f);
    switch (preset)
    {
    case EntityWoman:
        e.Friction = 7.7f;
        e.mass = 50.0f;
        e.drawable = DrawableCreate((SDL_Rect){0, 44, 57, 43}, (SDL_Rect){e.position.x, e.position.y, 57, 43}, SS_Characters);
        e.health = 100;
        e.isCollider = SDL_TRUE;
        break;
    case EntityPlayerSpawn:
        e.drawable.spriteSheet = SS_BackgroundTiles;
        break;
    case EntityMapObject:
        e.drawable.spriteSheet = SS_BackgroundTiles;
        e.mass = FLT_MAX;
        e.isCollider = SDL_TRUE;
        break;
    default:
        break;
    }
    return e;
}
// the update logic
void EntityUpdate(Entity entities[], int nrEnts, Clock *clk)
{
    EntityUpdateMovment(entities, nrEnts, clk);
    EntityOnCollision(entities, nrEnts, clk);
}

void EntityUpdateMovment(Entity entities[], int nrEnts, Clock *clk)
{

    for (int i = 0; i < nrEnts; i++)
    {
        // carculate Net_force so friction & collision & the other forces is handle before
        entities[i] = EntityNetForces(entities[i], nrEnts, clk);

        // update new position
        entities[i].position.x += entities[i].Velosity.x * ClockGetDeltaTime(clk);
        entities[i].position.y += entities[i].Velosity.y * ClockGetDeltaTime(clk);
#ifdef DegBug
        if (entities[i].id == 0)
        {
            log_debug("CurrentEntity:");
            log_debug("position x: %f", entities[i].position.x);
            log_debug("position y: %f", entities[i].position.y);
            log_debug("Force x: %f", entities[i].Force.x);
            log_debug("Force y: %f", entities[i].Force.y);
            log_debug("Accseleration x: %f", entities[i].Accseleration.x);
            log_debug("Accseleration y: %f", entities[i].Accseleration.y);
            log_debug("Velosity x: %f", entities[i].Velosity.x);
            log_debug("Velosity y: %f", entities[i].Velosity.y);
            log_debug("Friction: %f", entities[i].Friction);
            log_debug("mass: %f", entities[i].mass);
        }
#endif
    }
}

Entity EntityNetForces(Entity entity, int nrEnts, Clock *clk)
{
    // first get momentum
    entity.Velosity = Vec2DivL(entity.Force, entity.mass);

    // Friction formula (-1 * m * n * v(u)) v(u)= velocity unit vector
    Vec2 FrictionVector = Vec2Unit(entity.Velosity);
    FrictionVector = Vec2MulL(FrictionVector, -1.0F);
    FrictionVector = Vec2MulL(FrictionVector, entity.mass);
    FrictionVector = Vec2MulL(FrictionVector, entity.Friction);
    entity.Force.x += FrictionVector.x;
    entity.Force.y += FrictionVector.y;

    // Garante stop Cause float
    entity.Velosity = Vec2DivL(entity.Force, entity.mass);
    entity.Velosity.x = (fabs(entity.Velosity.x) < 5.1f) ? 0 : entity.Velosity.x;
    entity.Velosity.y = (fabs(entity.Velosity.y) < 5.1f) ? 0 : entity.Velosity.y;

    return entity;
}

SDL_bool EntityOnCollision(Entity entities[], int nrEnts, Clock *clk)
{
    SDL_Rect result;
    for (int Dominant = 0; Dominant < nrEnts; Dominant++)
    {
        for (int Recessive = 0; Recessive < nrEnts; Recessive++)
        {
            if (entities[Recessive].isCollider)
            {
                if (Dominant != Recessive)
                {
                    if (SDL_IntersectRect(&entities[Dominant].drawable.dst, &entities[Recessive].drawable.dst, &result))
                    {
                        Vec2 DominantCenter; // carculating center
                        DominantCenter.x = (float)entities[Dominant].drawable.dst.x + (float)entities[Dominant].drawable.dst.w / 2.0f;
                        DominantCenter.y = (float)entities[Dominant].drawable.dst.y + (float)entities[Dominant].drawable.dst.h / 2.0f;

                        Vec2 RecessiveCenter; // carculating center
                        RecessiveCenter.x = (float)entities[Recessive].drawable.dst.x + (float)entities[Recessive].drawable.dst.w / 2.0f;
                        RecessiveCenter.y = (float)entities[Recessive].drawable.dst.y + (float)entities[Recessive].drawable.dst.h / 2.0f;

                        Vec2 ResultDistance; // Carculating distance
                        ResultDistance.x = DominantCenter.x - RecessiveCenter.x;
                        ResultDistance.y = DominantCenter.y - RecessiveCenter.y;

                        entities[Recessive].position.x -= ResultDistance.x / 17.0f; // the reason it's divide in to 17 pices it's to avoid the heartbeat affect
                        entities[Recessive].position.y -= ResultDistance.y / 17.0f;
                    }
                }
            }
        }
    }
    return SDL_FALSE;
}
void EntityDraw(Camera *camera, Entity *entity)
{
    entity->drawable.dst.x = entity->position.x;
    entity->drawable.dst.y = entity->position.y;
    CameraDraw(camera, entity->drawable);
}