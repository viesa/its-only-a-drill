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
Entity EntityCreate(int x, int, int moveSpeed, int rotSpeed, EntityPresets preset, SDL_bool isCollider, SDL_bool isMovable);
void EntityDraw(Camera *camera, Entity entity);
void EntityUpdate(Entity *entity, Clock *clk);
void EntityUpdateWithCollision(Entity entities[], int nrEnts, Entity *user, int nrSelfIndex, Clock *clk);
SDL_bool EntityOnCollision(Entity entities[], int nrEnts, Entity user, int nrSelfIndex, Clock *clk);
#endif