#ifndef ENTITY_H
#define ENTITY_H
#include "Graphics.h"
#include "Camera.h"
#include "Clock.h"
#include "math.h"
typedef enum EntityPresets{
    EntityWoman
}EntityPresets;
typedef struct Entity{
    int rot, rotSpeed, moveSpeed;
    SDL_bool isCollider;
    SDL_bool isMovable;
    Drawable drawable;
    Vec2 posVec;
    Vec2 moveVec;
}Entity;
//Returns an entity struct that contains the nessecary arguments for a basic funktional entity
Entity EntityCreate(Vec2 vec, int moveSpeed, int rotSpeed, EntityPresets preset);
//Draw the entity relative to the camera
void EntityDraw(Camera *camera, Entity *entity);
//If entities == NULL it will ignore other entities, if entities != NULL it will update the targeted user with respect to the entities
void EntityUpdate(Entity entities[], int nrEnts, Entity *user, int nrSelfIndex, Clock *clk);
//Returns a SDL_bool based on if the user entity is colliding with anything else in the entities array, nrEnts are length of array, nrSelfIndex is the index in the array in which the entity is located at (if it doesnt exist in that context simply put a higher number than the nrEnts)
SDL_bool EntityOnCollision(Entity entities[], int nrEnts, Entity user, int nrSelfIndex, Clock *clk);
#endif

//drawable följer en vec2 istället