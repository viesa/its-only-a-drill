#include "Entity.h"

#include "EntityManager.h"

Entity EntityCreate(Vec2 position, EntityType type, int id)
{
    Entity entity;
    Vec2 enemyPos;
    entity.isNPC = SDL_FALSE;
    entity.type = type;
    entity.id = id;
    entity.position = position;
    entity.Force = Vec2Create(0.0f, 0.0f);
    switch (type)
    {
    case ET_Woman:
        entity.Friction = 7.7f;
        entity.mass = 96.0f;
        entity.drawables[0] = DrawableCreate((SDL_Rect){0, 44, 57, 43}, (SDL_Rect){entity.position.x, entity.position.y, 57, 43}, SS_Characters);
        entity.hitboxIndex = 0;
        entity.nDrawables = 1;
        entity.health = 100;
        entity.isCollider = SDL_TRUE;
        entity.entityState = Neutral;

        enemyPos = RectMid(entity.drawables[0].dst);
        entity.desiredPoint = Vec2AddL(enemyPos, -50);
        entity.indexPoint = 0;
        break;

    case ET_WomanP:
        entity.Friction = 7.7f;
        entity.mass = 96.0f;
        entity.drawables[0] = DrawableCreate((SDL_Rect){0, 44, 57, 43}, (SDL_Rect){entity.position.x, entity.position.y, 57, 43}, SS_Characters);
        entity.hitboxIndex = 0;
        entity.nDrawables = 1;
        entity.health = 100;
        entity.isCollider = SDL_TRUE;
        entity.entityState = Passive;

        enemyPos = RectMid(entity.drawables[0].dst);
        entity.desiredPoint = Vec2AddL(enemyPos, -50);
        entity.indexPoint = 0;
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
        entity.mass = 96.0f;
        entity.health = 100;
        entity.isCollider = SDL_TRUE;
        entity.drawables[0] = DrawableCreateDefaultConfig();
        entity.drawables[1] = DrawableCreateDefaultConfig();
        entity.hitboxIndex = 0;
        entity.nDrawables = 2;
        break;
    case ET_Bullet:
        entity.Friction = 0.0f;
        entity.mass = 5.0f;
        entity.isCollider = SDL_TRUE;
        entity.drawables[0].spriteSheet = SS_BackgroundTiles;
        entity.hitboxIndex = 0;
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

void EntityDraw(Entity *entity, Camera *camera)
{
    for (int i = 0; i < entity->nDrawables; i++)
    {
        entity->drawables[i].dst.x = entity->position.x;
        entity->drawables[i].dst.y = entity->position.y;
        CameraDraw(camera, entity->drawables[i]);
    }
}

void EntityDrawIndex(EntityIndexP index, Camera *camera)
{
    EntityDraw(&ENTITY_ARRAY[*index], camera);
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