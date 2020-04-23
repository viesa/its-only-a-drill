#ifndef ENTITY_H
#define ENTITY_H

#include "Dependencies.h"

#include "Camera.h"
#include "Clock.h"
#include "Library.h"
#include "../Items.h"
#include "../math/Vec2.h"

#define MAX_DRAWABLES 3

typedef enum EntityType
{
    ET_Player,
    ET_Woman,
    //Used only by Map.c
    ET_PlayerSpawn,
    //Used only by Map.c
    ET_MapObject
} EntityType;
typedef enum entityState
{
    GoForward,
    GoBack,
    Fight,
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
    EntityState entityState;

    SDL_bool isNPC;

    int id; // currently unused
    Drawable drawables[MAX_DRAWABLES];
    int nDrawables;

    Vec2 position;
    Vec2 Force;
    Vec2 Acceleration; // currently unused
    Vec2 Velocity;
    float Friction; // don't make it Lager then 10
    float mass;

    SDL_bool isCollider;
    unsigned int hitboxIndex;

    int health;

    InventoryListItems inventory;
} Entity;

///Creates a entity
///\param type: what entity is being created
Entity EntityCreate(Vec2 position, EntityType type, int id);

CompressedEntity EntityCompress(Entity ent);

Entity EntityUnCompress(CompressedEntity ent);

void EntityAddCompressed(CompressedEntity comp, Entity *ent);

void EntityDraw(Entity *entity, Camera *camera);

// carculates the net Froces after friction and collision
void EntityCalculateNetForces(Entity *entity);

void EntityRotateAll(Entity *entity, float degrees);

#endif