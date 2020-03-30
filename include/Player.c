#include "Player.h"

#include <SDL2/SDL_mouse.h>
#include <stdio.h>

Player PlayerCreate()
{
    Player ret;
    ret.entity = EntityCreate((Vec2){0.0f, 0.0f}, 120, 0, EntityWoman, 10);
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
    Vec2 playerPos = Vec2Sub(&player->entity.posVec, &cameraPos);

    Vec2 aim = Vec2Sub(&mousePos, &playerPos);

    aim = Vec2Unit(&aim);
    aim = Vec2MulL(&aim, RADIUS);

    player->aimFollow = Vec2Add(&aim, &player->entity.posVec);

    Vec2 delta = {0.0f, 0.0f};
    if (InputGet(input, KEY_A))
    {
        delta.x -= player->entity.moveSpeed * ClockGetDeltaTime(clock);
    }
    if (InputGet(input, KEY_W))
    {
        delta.y -= player->entity.moveSpeed * ClockGetDeltaTime(clock);
    }
    if (InputGet(input, KEY_D))
    {
        delta.x += player->entity.moveSpeed * ClockGetDeltaTime(clock);
    }
    if (InputGet(input, KEY_S))
    {
        delta.y += player->entity.moveSpeed * ClockGetDeltaTime(clock);
    }
    player->entity.position = Vec2Add(&player->entity.position, &delta);
}

void PlayerDraw(Player *player, Camera *camera)
{
    EntityDraw(camera, &player->entity);
}