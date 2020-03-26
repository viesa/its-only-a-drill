#include "Entity.h"

Entity EntityCreate(Vec2 vector, int moveSpeed, int rotSpeed, EntityPresets preset, SDL_bool isCollider, SDL_bool isMovable){
    Entity e;
    e.vector = vector;
    e.isCollider = isCollider;
    e.isMovable = isMovable;
    e.move_x = 0;
    e.move_y = 0;
    switch (preset)
    {
        case EntityWoman:
            e.moveSpeed = moveSpeed;
            e.rotSpeed = rotSpeed;
            e.drawable = DrawableCreate((SDL_Rect){0, 44, 57, 43}, (SDL_Rect){vector.x, vector.y, 57, 43}, SS_Characters);
            break;
        
        default:
            break;
    }
    return e;
}
SDL_bool EntityOnCollision(Entity entities[], int nrEnts, Entity user, int nrSelfIndex, Clock *clk){
    Entity test = user;
    if (test.move_x > 0){
        test.vector.x += test.moveSpeed * ClockGetDeltaTime(clk);
        test.move_x -= 1;

        for(int i = 0; i < nrEnts; i++){
            if (i != nrSelfIndex && entities[i].isCollider){
                if (SDL_IntersectRect(&entities[i].drawable.dst, &test.drawable.dst, &(SDL_Rect){0,0,0,0})){
                    //connects
                    if (entities[i].isMovable){
                        entities[i].vector.x = test.vector.x + test.drawable.dst.w;
                    }
                }
            }
        }
    }
    if (test.move_x < 0){
        test.vector.x -= test.moveSpeed * ClockGetDeltaTime(clk);
        test.move_x += 1;

        for(int i = 0; i < nrEnts; i++){
            if (i != nrSelfIndex && entities[i].isCollider){
                if (SDL_IntersectRect(&entities[i].drawable.dst, &test.drawable.dst, &(SDL_Rect){0,0,0,0})){
                    //connects
                    if (entities[i].isMovable){
                        entities[i].vector.x = test.vector.x - test.drawable.dst.w;
                    }
                }
            }
        }
    }
    if (test.move_y > 0){
        test.vector.y += test.moveSpeed * ClockGetDeltaTime(clk);
        test.move_y -= 1;

        for(int i = 0; i < nrEnts; i++){
            if (i != nrSelfIndex && entities[i].isCollider){
                if (SDL_IntersectRect(&entities[i].drawable.dst, &test.drawable.dst, &(SDL_Rect){0,0,0,0})){
                    //connects
                    if (entities[i].isMovable){
                        entities[i].vector.y = test.vector.y + test.drawable.dst.h;
                    }
                }
            }
        }
    }
    if (test.move_y < 0){
        test.vector.y -= test.moveSpeed * ClockGetDeltaTime(clk);
        test.move_y += 1;

        for(int i = 0; i < nrEnts; i++){
            if (i != nrSelfIndex && entities[i].isCollider){
                if (SDL_IntersectRect(&entities[i].drawable.dst, &test.drawable.dst, &(SDL_Rect){0,0,0,0})){
                    //connects
                    if (entities[i].isMovable){
                        entities[i].vector.y = test.vector.y - test.drawable.dst.h;
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
        user->vector.x += user->moveSpeed * ClockGetDeltaTime(clk);
        user->move_x -= 1;
    }
    if (user->move_x < 0){
        user->vector.x -= user->moveSpeed * ClockGetDeltaTime(clk);
        user->move_x += 1;
    }
    if (user->move_y > 0){
        user->vector.y += user->moveSpeed * ClockGetDeltaTime(clk);
        user->move_y -= 1;
    }
    if (user->move_y < 0){
        user->vector.y -= user->moveSpeed * ClockGetDeltaTime(clk);
        user->move_y += 1;
    }
    if (user->rot > 0){
        user->drawable.rot += user->rotSpeed * ClockGetDeltaTime(clk);
        user->rot -=1;
    }
    if (user->rot < 0){
        user->drawable.rot -= user->rotSpeed * ClockGetDeltaTime(clk);
        user->rot +=1;
    }
}
void EntityDraw(Camera *camera, Entity *entity){
    entity->drawable.dst.x = entity->vector.x;
    entity->drawable.dst.y = entity->vector.y;
    CameraDraw(camera, entity->drawable);
}