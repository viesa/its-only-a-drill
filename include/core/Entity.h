#ifndef ENTITY_H
#define ENTITY_H
#include "Camera.h"
#include "Clock.h"
#include "math.h"
typedef enum EntityPresets
{
    EntityWoman,
    //Used only by Map.c
    EntityPlayerSpawn,
    //Used only by Map.c
    EntityMapObject
} EntityPresets;
typedef struct Entity
{
    int rot, id;
    SDL_bool isCollider;
    Drawable drawable;
    Vec2 position;
    Vec2 Force;
    Vec2 accseleration;
    float Friction, mass;
} Entity;

///Creates a entity
///\param preset: what entity is being created
///makes a entity and create it so it exists
Entity EntityCreate(Vec2 position, EntityPresets preset, int id);

///Waring only one
///\param entites: ALL entitys are needed
///updates the state of alla entitys
void EntityUpdate(Entity entities[], int nrEnts, Clock *clk);
void EntityDraw(Camera *camera, Entity *entity);

/// finds the collision and fixes the problem with rules
SDL_bool EntityOnCollision(Entity entities[], int nrEnts, Clock *clk);
#endif