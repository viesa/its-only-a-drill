#ifndef ENTITY_H
#define ENTITY_H
#include "Graphics.h"
#include "Camera.h"
#include "Clock.h"
#include "math.h"
typedef enum EntityPresets
{
    EntityWoman
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
//Returns an entity struct that contains the nessecary arguments for a basic funktional entity
Entity EntityCreate(Vec2 vec, int moveSpeed, int rotSpeed, EntityPresets preset, int id);
//Draw the entity relative to the camera
void EntityDraw(Camera *camera, Entity *entity);
//If entities == NULL it will ignore other entities, if entities != NULL it will update the targeted user with respect to the entities
void EntityUpdate(Entity entities[], int nrEnts, Clock *clk);
//Returns a SDL_bool based on if the user entity is illegally colliding and should not be moved
SDL_bool EntityOnCollision(Entity entities[], int nrEnts, Clock *clk);
#endif

//drawable följer en vec2 istället