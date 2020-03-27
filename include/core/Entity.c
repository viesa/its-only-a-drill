#include "Entity.h"

Entity EntityCreate(Vec2 posVec, int moveSpeed, int rotSpeed, EntityPresets preset, int uniqueIdentifier){
    Entity e;
    e.id;
    e.posVec = posVec;
    e.moveVec.x = 0;
    e.moveVec.x = 0;
    switch (preset)
    {
        case EntityWoman:
            e.moveSpeed = moveSpeed;
            e.rotSpeed = rotSpeed;
            e.isCollider = SDL_TRUE;
            e.isMovable = SDL_TRUE;
            e.drawable = DrawableCreate((SDL_Rect){0, 44, 57, 43}, (SDL_Rect){e.posVec.x, e.posVec.y, 57, 43}, SS_Characters);
            break;
        
        default:
            break;
    }
    return e;
}
SDL_bool EntityOnCollision(Entity entities[], int nrEnts, Entity user, Clock *clk){
    Entity test = user;
    if (test.moveVec.x > 0){
        test.posVec.x += test.moveSpeed * ClockGetDeltaTime(clk);
        test.moveVec.x -= 1;

        for(int i = 0; i < nrEnts; i++){
            if (entities[i].id != user.id && entities[i].isCollider){
                if (SDL_IntersectRect(&entities[i].drawable.dst, &test.drawable.dst, &(SDL_Rect){0,0,0,0})){
                    //connects with first thing
                    if (entities[i].isMovable){
                        entities[i].posVec.x = test.posVec.x - test.drawable.dst.w;
                    }
                }
            }
        }
    }
    if (test.moveVec.x < 0){
        test.posVec.x -= test.moveSpeed * ClockGetDeltaTime(clk);
        test.moveVec.x += 1;

        for(int i = 0; i < nrEnts; i++){
            if (entities[i].id != user.id && entities[i].isCollider){
                if (SDL_IntersectRect(&entities[i].drawable.dst, &test.drawable.dst, &(SDL_Rect){0,0,0,0})){
                    //connects
                    if (entities[i].isMovable){
                        entities[i].posVec.x = test.posVec.x - test.drawable.dst.w;
                    }
                }
            }
        }
    }
    if (test.moveVec.y > 0){
        test.posVec.y += test.moveSpeed * ClockGetDeltaTime(clk);
        test.moveVec.y -= 1;

        for(int i = 0; i < nrEnts; i++){
            if (entities[i].id != user.id && entities[i].isCollider){
                if (SDL_IntersectRect(&entities[i].drawable.dst, &test.drawable.dst, &(SDL_Rect){0,0,0,0})){
                    //connects
                    if (entities[i].isMovable){
                        entities[i].posVec.y = test.posVec.y + test.drawable.dst.h;
                    }
                }
            }
        }
    }
    if (test.moveVec.y < 0){
        test.posVec.y -= test.moveSpeed * ClockGetDeltaTime(clk);
        test.moveVec.y += 1;

        for(int i = 0; i < nrEnts; i++){
            if (entities[i].id != user.id && entities[i].isCollider){
                if (SDL_IntersectRect(&entities[i].drawable.dst, &test.drawable.dst, &(SDL_Rect){0,0,0,0})){
                    //connects
                    if (entities[i].isMovable){
                        entities[i].posVec.y = test.posVec.y - test.drawable.dst.h;
                    }
                }
            }
        }
    }
    for(int i = 0; i < nrEnts; i++){
        if (entities[i].id != user.id && entities[i].isCollider){
            if (SDL_IntersectRect(&entities[i].drawable.dst, &test.drawable.dst, &(SDL_Rect){0,0,0,0})){
                //connects
                return SDL_TRUE;
            }
        }
    }
    return SDL_FALSE;
}
void EntityUpdate(Entity entities[], int nrEnts, Entity *user, Clock *clk){
    if (entities){
        if (user->isCollider || user->isMovable){
            if (EntityOnCollision(entities, nrEnts, *user, clk))
                return;
        }
    }
    
    if (user->moveVec.x > 0){
        user->posVec.x += user->moveSpeed * ClockGetDeltaTime(clk);
        user->moveVec.x -= 1;
    }
    if (user->moveVec.x < 0){
        user->posVec.x -= user->moveSpeed * ClockGetDeltaTime(clk);
        user->moveVec.x += 1;
    }
    if (user->moveVec.x > 0){
        user->posVec.y += user->moveSpeed * ClockGetDeltaTime(clk);
        user->moveVec.x -= 1;
    }
    if (user->moveVec.x < 0){
        user->posVec.y -= user->moveSpeed * ClockGetDeltaTime(clk);
        user->moveVec.x += 1;
    }
    if (user->rot > 0){
        user->drawable.rot += user->rotSpeed * ClockGetDeltaTime(clk);
        user->rot -=1;
    }
    if (user->rot < 0){
        user->drawable.rot -= user->rotSpeed * ClockGetDeltaTime(clk);
        user->rot +=1;
    }
    printf("x, y: %d %d\n", user->posVec.x, user->posVec.y);
}
void EntityDraw(Camera *camera, Entity *entity){
    entity->drawable.dst.x = entity->posVec.x;
    entity->drawable.dst.y = entity->posVec.y;
    CameraDraw(camera, entity->drawable);
}