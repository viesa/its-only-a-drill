#ifndef ENTITY_H
#define ENTITY_H
#include "Camera.h"
#include "Clock.h"
#include "math.h"
#include "../Items.h"
#include "../math/Vec2.h"
#include <float.h>
#include "Library.h"

typedef enum EntityPresets
{
    EntityWoman,
    //Used only by Map.c
    EntityPlayerSpawn,
    //Used only by Map.c
    EntityMapObject
} EntityPresets;
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

typedef struct Entity
{
    int rot, id; // currently unused
    SDL_bool isCollider;
    Drawable drawable;
    Vec2 position;
    Vec2 Force;
    Vec2 Accseleration; // currently unused
    Vec2 Velosity;
    float Friction; // don't make it Lager then 10
    float mass;
    InventoryListItems inventory;
    int health;
    EntityState entityState;
    Vec2 desiredPoint;
    int indexPoint;
} Entity;

///Creates a entity
///\param preset: what entity is being created
///makes a entity and create it so it exists
Entity EntityCreate(Vec2 position, EntityPresets preset, int id);

///Waring only one
///\param entites: ALL entitys are needed
///@IMPORTANT the first entity is dominant whitch means it does the pyhsics first.
void EntityUpdate(Entity entities[], int nrEnts, Clock *clk);
void EntityDraw(Camera *camera, Entity *entity);

void EntityUpdateMovment(Entity entities[], int nrEnts, Clock *clk);
/// finds the collision and fixes the problem with rules
SDL_bool EntityOnCollision(Entity entities[], int nrEnts, Clock *clk);
// carculates the net Froces after friction and collision
Entity EntityNetForces(Entity entity, int nrEnts, Clock *clk);
#endif