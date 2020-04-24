#ifndef ENTITY_H
#define ENTITY_H

#include "Dependencies.h"

#include "Camera.h"
#include "Clock.h"
#include "Library.h"
#include "../Items.h"
#include "../math/Vec2.h"
#include "Library.h"

#define MAX_DRAWABLES 3

typedef size_t *EntityIndexP;

typedef enum EntityType
{
    ET_Player,
    ET_Woman,
    //Used only by Map.c
    ET_PlayerSpawn,
    //Used only by Map.c
    ET_MapObject,
    ET_None,
    ET_Count
} EntityType;

typedef enum entityState
{
    GoForward,
    GoBack,
    Fight,
    Aggressive,
    EntityDead,
    // used only for player so that the player can take control
    EntityPlayer
} EntityState;

typedef struct CompressedEntity
{
    EntityType type;
    Vec2 position;
    int health;
    int id;
} CompressedEntity;
typedef struct Entity
{
    EntityType type;

    SDL_bool isNPC;

    int id; // currently unused
    Drawable drawables[MAX_DRAWABLES];
    int nDrawables;

    Vec2 position;
    Vec2 Force;
    Vec2 Acceleration; // currently unused
    Vec2 Velocity;
    float Friction; // don't make it larger than 10
    float mass;

    SDL_bool isCollider;
    unsigned int hitboxIndex;

    int health;
    EntityState entityState;
    Vec2 desiredPoint;
    int indexPoint;

    InventoryListItems inventory;
} Entity;

///Creates a entity
///\param type: an entity preset
Entity EntityCreate(Vec2 position, EntityType type, int id);

CompressedEntity EntityCompress(Entity *entity);

Entity EntityDecompress(CompressedEntity *cEntity);

void EntityAddCompressed(Entity *entity, CompressedEntity *cEntity);

void EntityDraw(Entity *entity, Camera *camera);
void EntityDrawIndex(EntityIndexP index, Camera *camera);

// calculates the net forces after friction and collision
void EntityCalculateNetForces(Entity *entity);

void EntityRotateAll(EntityIndexP index, float degrees);

#endif