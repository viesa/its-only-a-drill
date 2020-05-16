#include "Entity.h"

#include "EntityManager.h"

Entity EntityCreate(Vec2 position, EntityType type, int id)
{
    Entity entity;
    entity.isNPC = SDL_FALSE;
    entity.type = type;
    entity.id = id;
    entity.position = position;
    entity.Force = Vec2Create(0.0f, 0.0f);
    // Default hitbox offset if not set
    EntityChangeHitboxOffset(&entity, 0, 0);
    // Default hitbox size if not set
    EntityChangeHitboxSize(&entity, 0, 0);
    switch (type)
    {
    case ET_MapObject:
        entity.drawables[0].spriteSheet = SS_BackgroundTiles;
        break;

    case ET_Player:
        entity.Friction = 9.7f;
        entity.mass = 96.0f;
        entity.health = 100;
        entity.isCollider = SDL_TRUE;
        entity.isMovable = SDL_TRUE;
        entity.drawables[0] = DrawableCreateDefaultConfig();
        entity.drawables[1] = DrawableCreateDefaultConfig();
        entity.nDrawables = 2;
        EntityChangeHitboxOffset(&entity, 5, 5);
        EntityChangeHitboxSize(&entity, 39, 39);
        break;
    case ET_NPC:
        entity.Friction = 9.7f;
        entity.mass = 96.0f;
        entity.health = 100;
        entity.isCollider = SDL_TRUE;
        entity.isMovable = SDL_TRUE;
        entity.drawables[0] = DrawableCreateDefaultConfig();
        entity.drawables[1] = DrawableCreateDefaultConfig();
        entity.nDrawables = 2;
        EntityChangeHitboxOffset(&entity, 5, 5);
        EntityChangeHitboxSize(&entity, 39, 39);
        break;
    case ET_Bullet:
        entity.Friction = 0.0f;
        entity.mass = 5.0f;
        entity.isCollider = SDL_TRUE;
        entity.isMovable = SDL_TRUE;
        entity.drawables[0].spriteSheet = SS_BackgroundTiles;
        break;

    default:
        break;
    }
    return entity;
}

CompressedEntity EntityCompress(Entity *entity)
{
    CompressedEntity cEntity;
    cEntity.id = entity->id;
    cEntity.health = entity->health;
    cEntity.position = entity->position;
    for (int i = 0; i < MAX_DRAWABLES; i++)
    {
        cEntity.src[i] = entity->drawables[i].src;
        cEntity.rot[i] = entity->drawables[i].rot;
        cEntity.rot_anchor[i] = entity->drawables[i].rot_anchor;
    }
    return cEntity;
}

Entity EntityDecompress(CompressedEntity *cEntity)
{
    Entity entity;
    entity.id = cEntity->id;
    entity.health = cEntity->health;
    entity.position = cEntity->position;
    for (int i = 0; i < MAX_DRAWABLES; i++)
    {
        entity.drawables[i].src = cEntity->src[i];
        entity.drawables[i].rot = cEntity->rot[i];
        entity.drawables[i].rot_anchor = cEntity->rot_anchor[i];
    }
    return entity;
}

void EntityAddCompressed(Entity *entity, CompressedEntity *cEntity)
{
    entity->id = cEntity->id;
    entity->health = cEntity->health;
    entity->position = cEntity->position;
    for (int i = 0; i < MAX_DRAWABLES; i++)
    {
        entity->drawables[i].src = cEntity->src[i];
        entity->drawables[i].rot = cEntity->rot[i];
        entity->drawables[i].rot_anchor = cEntity->rot_anchor[i];
    }
}

void EntityDraw(Entity *entity)
{
    for (int i = 0; i < entity->nDrawables; i++)
    {
        entity->drawables[i].dst.x = entity->position.x;
        entity->drawables[i].dst.y = entity->position.y;
        CameraDraw(entity->drawables[i]);
    }
}

void EntityDrawIndex(EntityIndexP index)
{
    EntityDraw(&ENTITY_ARRAY[*index]);
}

void EntityCalculateNetForces(Entity *entity)
{
    // first get momentum
    float ComputedFormula = 1 - entity->mass / 4096.0f;
    entity->Velocity = Vec2MulL(entity->Velocity, ComputedFormula);

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

void EntityRotateAll(EntityIndexP index, float degrees)
{
    for (int i = 0; i < ENTITY_ARRAY[*index].nDrawables; i++)
    {
        ENTITY_ARRAY[*index].drawables[i].rot_anchor = Vec2Create(0.5f, 0.5f);
        ENTITY_ARRAY[*index].drawables[i].rot = degrees;
    }
}

SDL_Rect EntityGetHitbox(Entity *entity)
{
    return (SDL_Rect){entity->position.x + entity->hitboxOffset.x,
                      entity->position.y + entity->hitboxOffset.y,
                      entity->hitboxSize.x,
                      entity->hitboxSize.y};
}

void EntityChangeHitboxOffset(Entity *entity, int x, int y)
{
    entity->hitboxOffset.x = (float)x;
    entity->hitboxOffset.y = (float)y;
}

void EntityChangeHitboxSize(Entity *entity, int width, int height)
{
    entity->hitboxSize.x = (float)width;
    entity->hitboxSize.y = (float)height;
}