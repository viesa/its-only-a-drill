#include "Player.h"

#include <SDL2/SDL_mouse.h>
#include <stdio.h>

#include "core/Library.h"

Player PlayerCreate(Camera *camera, EntityManager *entityManager)
{
    Player ret;
    ret.entity = EntityManagerAdd(entityManager, ET_Player, Vec2Create(0.0f, 0.0f));
    ret.walkAnim = AnimCreate(AN_PlayerWalk, ANRO_RepeatFromEnd, SS_Character_Prisoner, 4, 0.05f);
    ret.aimFollow = Vec2Create(0.0f, 0.0f);
    ret.forward = Vec2Create(1.0f, 0.0f);
    return ret;
}

void PlayerDestroy(Player *player)
{
}

void PlayerUpdate(Player *player, Input *input, Clock *clock, Camera *camera)
{
    AnimUpdate(&player->walkAnim, ClockGetDeltaTime(clock));
    if (!InputIsKeyDown(input, SDL_SCANCODE_A) &&
        !InputIsKeyDown(input, SDL_SCANCODE_W) &&
        !InputIsKeyDown(input, SDL_SCANCODE_D) &&
        !InputIsKeyDown(input, SDL_SCANCODE_S) &&
        !InputIsKeyDown(input, SDL_SCANCODE_LEFT) &&
        !InputIsKeyDown(input, SDL_SCANCODE_UP) &&
        !InputIsKeyDown(input, SDL_SCANCODE_RIGHT) &&
        !InputIsKeyDown(input, SDL_SCANCODE_DOWN))
    {
        AnimStop(&player->walkAnim);
    }
    else
    {
        AnimResume(&player->walkAnim);
    }

    Drawable *a = player->walkAnim.active;
    player->entity->drawables[0].dst.w = a->dst.w * 1.5f;
    player->entity->drawables[0].dst.h = a->dst.h * 1.5f;
    player->entity->drawables[0].src = a->src;
    player->entity->drawables[0].spriteSheet = a->spriteSheet;

    int pos_x = 0;
    int pos_y = 0;
    SDL_GetMouseState(&pos_x, &pos_y);

    Vec2 mousePos = Vec2Create((float)pos_x, (float)pos_y);
    Vec2 cameraPos = CameraGetPos(camera);
    Vec2 playerPos = Vec2Sub(RectMid(player->entity->drawables[0].dst), cameraPos);

    Vec2 playerToMouse = Vec2Sub(mousePos, playerPos);

    player->forward = Vec2Unit(playerToMouse);
    Vec2 aim = Vec2MulL(player->forward, RADIUS);

    player->aimFollow = Vec2Add(aim, player->entity->position);

    SDL_Rect dstMid = {0, 0, player->entity->drawables[0].dst.w, player->entity->drawables[0].dst.w};
    player->entity->drawables[0].rot_anchor = RectMid(dstMid);

    if (player->forward.y > 0.0f)
        player->entity->drawables[0].rot = toDegrees(Vec2Ang((Vec2){1.0f, 0.0f}, player->forward));
    else
        player->entity->drawables[0].rot = 360 - toDegrees(Vec2Ang((Vec2){1.0f, 0.0f}, player->forward));

    player->entity->Force.y += 500 * ((InputIsKeyDown(input, SDL_SCANCODE_S) || InputIsKeyDown(input, SDL_SCANCODE_DOWN)) -
                                      (InputIsKeyDown(input, SDL_SCANCODE_W) || InputIsKeyDown(input, SDL_SCANCODE_UP)));
    player->entity->Force.x += 500 * ((InputIsKeyDown(input, SDL_SCANCODE_D) || InputIsKeyDown(input, SDL_SCANCODE_RIGHT)) -
                                      (InputIsKeyDown(input, SDL_SCANCODE_A) || InputIsKeyDown(input, SDL_SCANCODE_LEFT)));
}

void PlayerDraw(Player *player, Camera *camera)
{
    EntityDraw(player->entity, camera);
}