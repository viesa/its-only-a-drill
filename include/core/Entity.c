#include "Entity.h"

Entity EntityCreate(int x, int y, int moveSpeed, int rotSpeed, EntityPresets preset, SDL_bool isCollider, SDL_bool isMovable){
    Entity e;
    e.isCollider = isCollider;
    e.isMovable = isMovable;
    e.move_x = 0;
    e.move_y = 0;
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
void EntityUpdate(Entity *entity, Clock *clk){
    if (entity->move_x > 0){
        entity->drawable.dst.x += entity->moveSpeed * ClockGetDeltaTime(clk);
        entity->move_x -= 1;
    }
    if (entity->move_x < 0){
        entity->drawable.dst.x -= entity->moveSpeed * ClockGetDeltaTime(clk);
        entity->move_x -= 1;
    }
    if (entity->move_y > 0){
        entity->drawable.dst.y += entity->moveSpeed * ClockGetDeltaTime(clk);
        entity->move_y -= 1;
    }
    if (entity->move_y < 0){
        entity->drawable.dst.y -= entity->moveSpeed * ClockGetDeltaTime(clk);
        entity->move_y -= 1;
    }
    if (entity->rot > 0){
        entity->drawable.rot += entity->rotSpeed * ClockGetDeltaTime(clk);
        entity->rot -=1;
    }
    if (entity->rot < 0){
        entity->drawable.rot -= entity->rotSpeed * ClockGetDeltaTime(clk);
        entity->rot -=1;
    }
}
SDL_bool EntityOnCollision(Entity entities[], int nrEnts, Entity user, int nrSelfIndex, Clock *clk){
    Entity test = user;
    if (test.move_x > 0){
        test.drawable.dst.x += test.moveSpeed * ClockGetDeltaTime(clk);
        test.move_x -= 1;

        for(int i = 0; i < nrEnts; i++){
            if (i != nrSelfIndex && entities[i].isCollider){
                if (SDL_IntersectRect(&entities[i].drawable.dst, &test.drawable.dst, &(SDL_Rect){0,0,0,0})){
                    //connects
                    if (entities[i].isMovable){
                        entities[i].drawable.dst.x = test.drawable.dst.x + test.drawable.dst.w;
                    }
                }
            }
        }
    }
    if (test.move_x < 0){
        test.drawable.dst.x -= test.moveSpeed * ClockGetDeltaTime(clk);
        test.move_x -= 1;

        for(int i = 0; i < nrEnts; i++){
            if (i != nrSelfIndex && entities[i].isCollider){
                if (SDL_IntersectRect(&entities[i].drawable.dst, &test.drawable.dst, &(SDL_Rect){0,0,0,0})){
                    //connects
                    if (entities[i].isMovable){
                        entities[i].drawable.dst.x = test.drawable.dst.x;
                    }
                }
            }
        }
    }
    if (test.move_y > 0){
        test.drawable.dst.y += test.moveSpeed * ClockGetDeltaTime(clk);
        test.move_y -= 1;

        for(int i = 0; i < nrEnts; i++){
            if (i != nrSelfIndex && entities[i].isCollider){
                if (SDL_IntersectRect(&entities[i].drawable.dst, &test.drawable.dst, &(SDL_Rect){0,0,0,0})){
                    //connects
                    if (entities[i].isMovable){
                        entities[i].drawable.dst.y = test.drawable.dst.y + test.drawable.dst.h;
                    }
                }
            }
        }
    }
    if (test.move_y < 0){
        test.drawable.dst.y -= test.moveSpeed * ClockGetDeltaTime(clk);
        test.move_y -= 1;

        for(int i = 0; i < nrEnts; i++){
            if (i != nrSelfIndex && entities[i].isCollider){
                if (SDL_IntersectRect(&entities[i].drawable.dst, &test.drawable.dst, &(SDL_Rect){0,0,0,0})){
                    //connects
                    if (entities[i].isMovable){
                        entities[i].drawable.dst.y = test.drawable.dst.y;
                    }
                }
            }
        }
    }
    for(int i = 0; i < nrEnts; i++){
        if (i != nrSelfIndex && entities[i].isCollider){
            if (SDL_IntersectRect(&entities[i].drawable.dst, &test.drawable.dst, &(SDL_Rect){0,0,0,0})){
                //connects
                return SDL_TRUE;
            }
        }
    }
    return SDL_FALSE;
}
void EntityUpdateWithCollision(Entity entities[], int nrEnts, Entity *user, int nrSelfIndex, Clock *clk){
    if (user->isCollider || user->isMovable){
        if (EntityOnCollision(entities, nrEnts, *user, nrSelfIndex, clk))
            return;
    }
    EntityUpdate(user, clk);
}
void EntityDraw(Camera *camera, Entity entity){
    CameraDraw(camera, entity.drawable);
}