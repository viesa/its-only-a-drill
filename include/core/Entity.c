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
void EntityUpdate(Entity entities[], int nrEnts, Entity *user, int nrSelfIndex, Clock *clk){
    if (entities){
        if (user->isCollider || user->isMovable){
            if (EntityOnCollision(entities, nrEnts, *user, nrSelfIndex, clk))
                return;
        }
    }
    if (user->move_x > 0){
        user->drawable.dst.x += user->moveSpeed * ClockGetDeltaTime(clk);
        user->move_x -= 1;
    }
    if (user->move_x < 0){
        user->drawable.dst.x -= user->moveSpeed * ClockGetDeltaTime(clk);
        user->move_x -= 1;
    }
    if (user->move_y > 0){
        user->drawable.dst.y += user->moveSpeed * ClockGetDeltaTime(clk);
        user->move_y -= 1;
    }
    if (user->move_y < 0){
        user->drawable.dst.y -= user->moveSpeed * ClockGetDeltaTime(clk);
        user->move_y -= 1;
    }
    if (user->rot > 0){
        user->drawable.rot += user->rotSpeed * ClockGetDeltaTime(clk);
        user->rot -=1;
    }
    if (user->rot < 0){
        user->drawable.rot -= user->rotSpeed * ClockGetDeltaTime(clk);
        user->rot -=1;
    }
}
void EntityDraw(Camera *camera, Entity entity){
    CameraDraw(camera, entity.drawable);
}