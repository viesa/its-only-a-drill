#include "Player.h"

#include <SDL2/SDL_mouse.h>
#include <stdio.h>

#include "core/Library.h"

Player PlayerCreate(Camera *camera)
{
    Player ret;
    ret.entity = EntityCreate((Vec2){0.0f, 0.0f}, EntityWoman, 10);
    ret.aimFollow = Vec2Create(0.0f, 0.0f);
    ret.forward = Vec2Create(1.0f, 0.0f);
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
    Vec2 playerPos = Vec2Sub(RectMid(player->entity.drawable.dst), cameraPos);

    Vec2 playerToMouse = Vec2Sub(mousePos, playerPos);

    player->forward = Vec2Unit(playerToMouse);
    Vec2 aim = Vec2MulL(player->forward, RADIUS);

    player->aimFollow = Vec2Add(aim, player->entity.position);

    SDL_Rect dstMid = {0, 0, player->entity.drawable.dst.w, player->entity.drawable.dst.w};
    player->entity.drawable.rot_anchor = RectMid(dstMid);

    if (player->forward.y > 0.0f)
        player->entity.drawable.rot = toDegrees(Vec2Ang((Vec2){1.0f, 0.0f}, player->forward));
    else
        player->entity.drawable.rot = 360 - toDegrees(Vec2Ang((Vec2){1.0f, 0.0f}, player->forward));

    player->entity.Force.y += 500 * ((InputIsKeyDown(input, SDL_SCANCODE_S) || InputIsKeyDown(input, SDL_SCANCODE_DOWN)) -
                                     (InputIsKeyDown(input, SDL_SCANCODE_W) || InputIsKeyDown(input, SDL_SCANCODE_UP)));
    player->entity.Force.x += 500 * ((InputIsKeyDown(input, SDL_SCANCODE_D) || InputIsKeyDown(input, SDL_SCANCODE_RIGHT)) -
                                     (InputIsKeyDown(input, SDL_SCANCODE_A) || InputIsKeyDown(input, SDL_SCANCODE_LEFT)));
}

void PlayerDraw(Player *player, Camera *camera)
{
    EntityDraw(camera, &player->entity);
}