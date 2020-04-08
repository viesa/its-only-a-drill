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

    player->entity.Force.y += 750 * ((InputIsKeyDown(input, SDL_SCANCODE_S) || InputIsKeyDown(input, SDL_SCANCODE_DOWN)) -
                                     (InputIsKeyDown(input, SDL_SCANCODE_W) || InputIsKeyDown(input, SDL_SCANCODE_UP)));
    player->entity.Force.x += 750 * ((InputIsKeyDown(input, SDL_SCANCODE_D) || InputIsKeyDown(input, SDL_SCANCODE_RIGHT)) -
                                     (InputIsKeyDown(input, SDL_SCANCODE_A) || InputIsKeyDown(input, SDL_SCANCODE_LEFT)));
}

void PlayerDraw(Player *player, Camera *camera)
{
    EntityDraw(camera, &player->entity);
}