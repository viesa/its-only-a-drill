#ifndef ENTITY_H
#define ENTITY_H
#include "Graphics.h"
#include "Camera.h"
#include "Clock.h"
typedef enum EntityPresets{
    EntityWoman
}EntityPresets;
typedef struct Entity{
    int move_x, move_y, rot, rotSpeed, moveSpeed;
    SDL_bool isCollider;
    SDL_bool isMovable;
    Drawable drawable;
}Entity;
//Returns an entity struct that contains the nessecary arguments for a basic funktional entity
Entity EntityCreate(int x, int, int moveSpeed, int rotSpeed, EntityPresets preset, SDL_bool isCollider, SDL_bool isMovable);
//Draw the entity relative to the camera
void EntityDraw(Camera *camera, Entity entity);
//Updates the targeted entity and ignores collision with any other entity
void EntityUpdate(Entity *entity, Clock *clk);
//Updates the targeted entity using the EntityOnCollision method
void EntityUpdateWithCollision(Entity entities[], int nrEnts, Entity *user, int nrSelfIndex, Clock *clk);
//Returns a SDL_bool based on if the user entity is colliding with anything else in the entities array, nrEnts are length of array, nrSelfIndex is the index in the array in which the entity is located at (if it doesnt exist in that context simply put a higher number than the nrEnts)
SDL_bool EntityOnCollision(Entity entities[], int nrEnts, Entity user, int nrSelfIndex, Clock *clk);
#endif