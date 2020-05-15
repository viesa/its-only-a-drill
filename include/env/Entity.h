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
    ET_Woman,
    //Used only by Map.c
    ET_PlayerSpawn,
    //Used only by Map.c
    ET_MapObject,
    ET_None,
    //Used only by weapon.c
    ET_Bullet,
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
    Vec2 Acceleration; // currently unused
    Vec2 Velocity;
    float Friction; // don't make it larger than 10
    float mass;

    SDL_bool isCollider;
    unsigned int hitboxIndex;

    int health;
} Entity;

///Creates a entity
///\param type: an entity preset
Entity EntityCreate(Vec2 position, EntityType type, int id);

// makes entity ready to be send over the internet
CompressedEntity EntityCompress(Entity *entity);
// handels the data over server transfer
Entity EntityDecompress(CompressedEntity *cEntity);

void EntityAddCompressed(Entity *entity, CompressedEntity *cEntity);

// draws the entity on the screen relativ to the camera
void EntityDraw(Entity *entity, Camera *camera);
// draws the entity on the screen with index
void EntityDrawIndex(EntityIndexP index, Camera *camera);

// calculates the net forces after friction and collision
void EntityCalculateNetForces(Entity *entity);

void EntityRotateAll(EntityIndexP index, float degrees);

#endif