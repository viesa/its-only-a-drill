#include "Entity.h"

Entity EntityCreate(Vec2 position, EntityType type, int id)
{
    Entity entity;
    entity.isNPC = SDL_FALSE;
    entity.type = type;
    entity.id = id;
    entity.position = position;
    entity.Force = Vec2Create(0.0f, 0.0f);
    switch (type)
    {
    case ET_Woman:
        entity.Friction = 7.7f;
        entity.mass = 50.0f;
        entity.drawables[0] = DrawableCreate((SDL_Rect){0, 44, 57, 43}, (SDL_Rect){entity.position.x, entity.position.y, 57, 43}, SS_Characters);
        entity.hitboxIndex = 0;
        entity.nDrawables = 1;
        entity.health = 100;
        entity.isCollider = SDL_TRUE;
        entity.entityState = GoForward;
        break;
    case ET_PlayerSpawn:
        entity.drawables[0].spriteSheet = SS_BackgroundTiles;
        entity.hitboxIndex = 0;
        break;
    case ET_MapObject:
        entity.drawables[0].spriteSheet = SS_BackgroundTiles;
        entity.hitboxIndex = 0;
        break;
    case ET_Player:
        entity.Friction = 9.7f;
        entity.mass = 50.0f;
        entity.health = 100;
        entity.isCollider = SDL_TRUE;
        entity.drawables[0] = DrawableCreate((SDL_Rect){872, 952 - 33 * 4, 33, 33}, (SDL_Rect){0, 0, 33, 33}, SS_Character_Prisoner);
        entity.drawables[1] = DrawableCreate((SDL_Rect){872, 952 - 33 * 4, 33, 33}, (SDL_Rect){0, 0, 33, 33}, SS_Character_Prisoner);
        entity.drawables[2] = DrawableCreate((SDL_Rect){872, 952 - 33 * 4, 33, 33}, (SDL_Rect){0, 0, 33, 33}, SS_Character_Prisoner);
        entity.hitboxIndex = 0;
        entity.nDrawables = 3;
        break;
    default:
        break;
    }
    return entity;
}

CompressedEntity EntityCompress(Entity ent)
{
    CompressedEntity comp;
    comp.health = ent.health;
    comp.id = ent.id;
    comp.position = ent.position;
    comp.type = ent.type;
    return comp;
}
Entity EntityUnCompress(CompressedEntity comp)
{
    Entity ent;
    ent.health = comp.health;
    ent.id = comp.id;
    ent.position = comp.position;
    ent.type = comp.type;
    return ent;
}
void EntityAddCompressed(CompressedEntity comp, Entity *ent)
{
    ent->health = comp.health;
    ent->id = comp.id;
    ent->position = comp.position;
    ent->type = comp.type;
}
void EntityDraw(Entity *entity, Camera *camera)
{
    for (int i = 0; i < entity->nDrawables; i++)
    {
        entity->drawables[i].dst.x = entity->position.x;
        entity->drawables[i].dst.y = entity->position.y;
        CameraDraw(camera, entity->drawables[i]);
    }
}

void EntityCalculateNetForces(Entity *entity)
{
    // first get momentum
    entity->Velocity = Vec2DivL(entity->Force, entity->mass);

// compute friendly
#ifndef frictionReal
    entity->Force = Vec2MulL(entity->Force, 0.95f);
#endif
// Friction formula (-1 * m * n * v(u)) v(u)= velocity unit vector
#ifdef frictionReal
    Vec2 FrictionVector = Vec2Unit(entity->Velocity);
    FrictionVector = Vec2MulL(FrictionVector, -1.0F);
    FrictionVector = Vec2MulL(FrictionVector, entity->mass);
    FrictionVector = Vec2MulL(FrictionVector, entity->Friction);
    entity->Force.x += FrictionVector.x;
    entity->Force.y += FrictionVector.y;
#endif
}

void EntityRotateAll(Entity *entity, float degrees)
{
    for (int i = 0; i < entity->nDrawables; i++)
    {
        SDL_Rect dstMid = {0, 0, entity->drawables[i].dst.w, entity->drawables[i].dst.w};
        entity->drawables[i].rot_anchor = RectMid(dstMid);
        entity->drawables[i].rot = degrees;
    }
}