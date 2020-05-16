#ifndef ENTITY_H
#define ENTITY_H

#include "Dependencies.h"

#include "Camera.h"
#include "Clock.h"
#include "Library.h"
#include "Vec2.h"
#include "Library.h"
#include "Items.h"

#define MAX_DRAWABLES 3

typedef size_t *EntityIndexP;

typedef enum EntityType
{
    ET_Player,
    ET_NPC,
    //Used only by Map.c
    ET_PlayerSpawn,
    //Used only by Map.c
    ET_MapObject,
    //Used only by weapon.c
    ET_Bullet,
    ET_None,
    ET_Count,
} EntityType;

typedef struct CompressedEntity
{
    int id;
    int health;
    Vec2 position;
    SDL_Rect src[MAX_DRAWABLES];
    float rot[MAX_DRAWABLES];
    Vec2 rot_anchor[MAX_DRAWABLES];
} CompressedEntity;

typedef struct Entity
{
    EntityType type;

    SDL_bool isNPC;

    int id;
    Drawable drawables[MAX_DRAWABLES];
    int nDrawables;

    Vec2 position;
    Vec2 Force;
    Vec2 Acceleration;
    Vec2 Velocity;
    float Friction;
    float mass;

    // Use EntityGetHitbox to get hitbox rectangle!
    Vec2 hitboxOffset;
    // Use EntityGetHitbox to get hitbox rectangle!
    Vec2 hitboxSize;

    SDL_bool isCollider;
    SDL_bool isMovable;

    int health;
} Entity;

///Creates a entity
///\param type: an entity preset
Entity EntityCreate(Vec2 position, EntityType type, int id);

// Extract core parts of Entity to a CompressedEntity
// Used by the network component
CompressedEntity EntityCompress(Entity *entity);
// Default creates an entity with the core parts of Compressed Entity
// This function usaually needs complementary setters (such as EntityType)
Entity EntityDecompress(CompressedEntity *cEntity);
// Adds the core parts of a Compressed Entity onto a Entity
// Only modifies the parts CompressedEntity and Entity shares
void EntityAddCompressed(Entity *entity, CompressedEntity *cEntity);
// Draws every drawable in the entity
void EntityDraw(Entity *entity);
// Draws every drawable in the entity
void EntityDrawIndex(EntityIndexP index);
// Calculates the net forces after friction and collision
/// This function does NOT use ClockGetDeltaTime
/// Instead it uses parameter dt to allow for smaller timesteps
void EntityCalculateNetForces(Entity *entity);
// Rotate all drawables in the entity around their centers
void EntityRotateAll(EntityIndexP index, float degrees);
// Returns the entity hitbox according to the offset specified
SDL_Rect EntityGetHitbox(Entity *entity);
// Moves the upper right corner of the entity hitbox
///\param x: offset in x from position x to hitbox x
///\param y: offset in y from position.y to hitbox.y
void EntityChangeHitboxOffset(Entity *entity, int x, int y);
// Changes the size of the entity hitbox
void EntityChangeHitboxSize(Entity *entity, int width, int height);

#endif