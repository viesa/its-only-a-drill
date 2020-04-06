#ifndef ENTITY_H
#define ENTITY_H
#include "Camera.h"
#include "Clock.h"
#include "math.h"
typedef enum EntityPresets
{
    EntityWoman,
    EntityPlayerSpawn,
    EntityMapObject
    //--------
} EntityPresets;
typedef struct Entity
{
    int rot, rotSpeed, moveSpeed, id;
    SDL_bool isCollider;
    SDL_bool isMovable;
    Drawable drawable;
    Vec2 position;
    Vec2 Force;
    Vec2 accseleration;
    float Friction, mass;
} Entity;

Entity EntityCreate(Vec2 position, int moveSpeed, int rotSpeed, EntityPresets preset, int id);

///\param entities: if NULL, no collision detection occurs
void EntityUpdate(Entity entities[], int nrEnts, Clock *clk);
void EntityDraw(Camera *camera, Entity *entity);

/// Checks whether entities are colliding or not
SDL_bool EntityOnCollision(Entity entities[], int nrEnts, Clock *clk);
#endif