#include "Player.h"

#include <SDL2/SDL_mouse.h>

Player PlayerCreate()
{
    Player ret;
    ret.entity = EntityCreate((Vec2){0.0f, 0.0f}, 120, 0, EntityWoman, 10);
    return ret;
}

void PlayerDestroy(Player *player)
{
}

void PlayerUpdate(Player *player, Input *input, Clock *clock)
{
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
    player->entity.posVec = Vec2Add(&player->entity.posVec, &delta);
}

void PlayerDraw(Player *player, Camera *camera)
{
    EntityDraw(camera, &player->entity);
}