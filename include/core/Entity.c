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
        entity.mass = 50.0f;
        entity.drawables[0] = DrawableCreate((SDL_Rect){0, 44, 57, 43}, (SDL_Rect){entity.position.x, entity.position.y, 57, 43}, SS_Characters);
        entity.hitboxIndex = 0;
        entity.nDrawables = 1;
        entity.health = 100;
        entity.isCollider = SDL_TRUE;
        entity.entityState = GoForward;

        enemyPos = RectMid(entity.drawables[0].dst);
        entity.desiredPoint = Vec2AddL(enemyPos, 200);
        entity.indexPoint = 0;
        entity.inventory = InventoryCreate();
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
    cEntity.health = entity->health;
    cEntity.id = entity->id;
    cEntity.position = entity->position;
    cEntity.type = entity->type;
    return cEntity;
}
Entity EntityDecompress(CompressedEntity *cEntity)
{
    Entity entity;
    entity.health = cEntity->health;
    entity.id = cEntity->id;
    entity.position = cEntity->position;
    entity.type = cEntity->type;
    return entity;
}
void EntityAddCompressed(Entity *entity, CompressedEntity *cEntity)
{
    entity->health = cEntity->health;
    entity->id = cEntity->id;
    entity->position = cEntity->position;
    entity->type = cEntity->type;
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
    float ComputedFormula = powf((1.0f + (2.0f / entity->mass)), entity->mass) / 8 + 0.059f;
    entity->Velocity = Vec2DivL(entity->Force, ComputedFormula);

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
        SDL_Rect dstMid = {0, 0, ENTITY_ARRAY[*index].drawables[i].dst.w, ENTITY_ARRAY[*index].drawables[i].dst.w};
        ENTITY_ARRAY[*index].drawables[i].rot_anchor = RectMid(dstMid);
        ENTITY_ARRAY[*index].drawables[i].rot = degrees;
    }
}