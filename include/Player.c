#include "Player.h"

#include <SDL2/SDL_mouse.h>
#include <stdio.h>

Player PlayerCreate()
{
    Player ret;
    ret.entity = EntityCreate((Vec2){0.0f, 0.0f}, EntityWoman, 10);
    ret.aimFollow = Vec2Create(0.0f, 0.0f);
    return ret;
}

void PlayerDestroy(Player *player)
{
}

void PlayerUpdate(Player *player, Input *input, Clock *clock, Camera *camera)
{
    int pos_x = 0;
    int pos_y = 0;
    SDL_GetMouseState(&pos_x, &pos_y);

    Vec2 mousePos = Vec2Create((float)pos_x, (float)pos_y);
    Vec2 cameraPos = CameraGetPos(camera);
    Vec2 playerPos = Vec2Sub(player->entity.position, cameraPos);

    Vec2 aim = Vec2Sub(mousePos, playerPos);

    aim = Vec2Unit(aim);
    aim = Vec2MulL(aim, RADIUS);

    player->aimFollow = Vec2Add(aim, player->entity.position);

    if (InputIsKeyDown(input, SDL_SCANCODE_D))
        player->entity.Force.x += 750;
    if (InputIsKeyDown(input, SDL_SCANCODE_A))
        player->entity.Force.x -= 750;
    if (InputIsKeyDown(input, SDL_SCANCODE_W))
        player->entity.Force.y -= 750;
    if (InputIsKeyDown(input, SDL_SCANCODE_S))
        player->entity.Force.y += 750;
    // Vec2 delta = {0.0f, 0.0f};
    // if (InputIsKeyDown(input, SDL_SCANCODE_A))
    // {
    //     delta.x -= player->entity.moveSpeed * ClockGetDeltaTime(clock);
    // }
    // if (InputIsKeyDown(input, SDL_SCANCODE_W))
    // {
    //     delta.y -= player->entity.moveSpeed * ClockGetDeltaTime(clock);
    // }
    // if (InputIsKeyDown(input, SDL_SCANCODE_D))
    // {
    //     delta.x += player->entity.moveSpeed * ClockGetDeltaTime(clock);
    // }
    // if (InputIsKeyDown(input, SDL_SCANCODE_S))
    // {
    //     delta.y += player->entity.moveSpeed * ClockGetDeltaTime(clock);
    // }
    // player->entity.position = Vec2Add(&player->entity.position, &delta);
}

void PlayerDraw(Player *player, Camera *camera)
{
    EntityDraw(camera, &player->entity);
}