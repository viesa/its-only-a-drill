#include "Entity.h"

Entity EntityCreate(int x, int y, int moveSpeed, int rotSpeed, EntityPresets preset){
    Entity e;
    switch (preset)
    {
        case EntityWoman:
            e.moveSpeed = moveSpeed;
            e.rotSpeed = rotSpeed;
            e.drawable = DrawableCreate((SDL_Rect){0, 44, 57, 43}, (SDL_Rect){x, y, 57, 43}, SS_Characters);
            break;
        
        default:
            break;
    }
    return e;
}
void UpdateEntity(Entity *entity){
    if (entity->move_x > 0){
        entity->drawable.dst.x += entity->moveSpeed;
        entity->move_x -= 1;
    }
    if (entity->move_x < 0){
        entity->drawable.dst.x -= entity->moveSpeed;
        entity->move_x -= 1;
    }
    if (entity->move_y > 0){
        entity->drawable.dst.y += entity->moveSpeed;
        entity->move_y -= 1;
    }
    if (entity->move_y < 0){
        entity->drawable.dst.y -= entity->moveSpeed;
        entity->move_y -= 1;
    }
    if (entity->rot > 0){
        entity->drawable.rot += entity->rotSpeed;
        entity->rot -=1;
    }
    if (entity->rot < 0){
        entity->drawable.rot -= entity->rotSpeed;
        entity->rot -=1;
    }
}
void EntityDraw(Camera *camera, Entity entity){
    CameraDraw(camera, entity.drawable);
}