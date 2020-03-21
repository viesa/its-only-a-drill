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
    Drawable drawable;
}Entity;
Entity EntityCreate(int x, int, int moveSpeed, int rotSpeed, EntityPresets preset);
void EntityDraw(Camera *camera, Entity entity);
//Entity moves move_x and move_y is amount of times to step, moveSpeed is how large the footstep is
void UpdateEntity(Entity *entity, Clock *clk);
#endif