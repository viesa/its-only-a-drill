#include "Player.h"

#include <SDL2/SDL_mouse.h>
#include <stdio.h>

#include "core/Library.h"

Player PlayerCreate(Camera *camera, EntityManager *entityManager)
{
    Player ret;
    ret.entity = EntityManagerAdd(entityManager, ET_Player, Vec2Create(0.0f, 0.0f));
    ret.leg = AnimCreate(AN_PlayerLegs, ANRO_RepeatFromEnd, SS_Character_Prisoner, 4, 0.05f);
    ret.body = AnimCreate(AN_PlayerBody, ANRO_RepeatFromEnd, SS_Character_Prisoner, 4, 0.05f);
    ret.aimFollow = Vec2Create(0.0f, 0.0f);
    ret.forward = Vec2Create(1.0f, 0.0f);
    return ret;
}

void PlayerDestroy(Player *player)
{
}

void PlayerUpdate(Player *player, Input *input, Clock *clock, Camera *camera)
{
    AnimUpdate(&player->leg, ClockGetDeltaTime(clock));
    AnimUpdate(&player->body, ClockGetDeltaTime(clock));
    if (!InputIsKeyDown(input, SDL_SCANCODE_A) &&
        !InputIsKeyDown(input, SDL_SCANCODE_W) &&
        !InputIsKeyDown(input, SDL_SCANCODE_D) &&
        !InputIsKeyDown(input, SDL_SCANCODE_S) &&
        !InputIsKeyDown(input, SDL_SCANCODE_LEFT) &&
        !InputIsKeyDown(input, SDL_SCANCODE_UP) &&
        !InputIsKeyDown(input, SDL_SCANCODE_RIGHT) &&
        !InputIsKeyDown(input, SDL_SCANCODE_DOWN))
    {
        AnimStop(&player->leg);
        AnimPause(&player->body);
        int randomNumber = rand() % 100;
        if (randomNumber < 3)
        {
            AnimRandomFrame(&player->body);
        }
    }
    else
    {
        AnimResume(&player->leg);
        AnimResume(&player->body);
    }

    Drawable *a_leg = player->leg.active;
    player->entity->drawables[0].dst.w = a_leg->dst.w * 1.5f;
    player->entity->drawables[0].dst.h = a_leg->dst.h * 1.5f;
    player->entity->drawables[0].src = a_leg->src;
    player->entity->drawables[0].spriteSheet = a_leg->spriteSheet;
    Drawable *a_body = player->body.active;
    player->entity->drawables[1].dst.w = a_body->dst.w * 1.5f;
    player->entity->drawables[1].dst.h = a_body->dst.h * 1.5f;
    player->entity->drawables[1].src = a_body->src;
    player->entity->drawables[1].spriteSheet = a_body->spriteSheet;

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

    float degrees = 0.0f;
    if (player->forward.y > 0.0f)
        degrees = toDegrees(Vec2Ang((Vec2){1.0f, 0.0f}, player->forward));
    else
        degrees = 360 - toDegrees(Vec2Ang((Vec2){1.0f, 0.0f}, player->forward));
    EntityRotateAll(player->entity, degrees);

    player->entity->Force.y += 500 * ((InputIsKeyDown(input, SDL_SCANCODE_S) || InputIsKeyDown(input, SDL_SCANCODE_DOWN)) -
                                      (InputIsKeyDown(input, SDL_SCANCODE_W) || InputIsKeyDown(input, SDL_SCANCODE_UP)));
    player->entity->Force.x += 500 * ((InputIsKeyDown(input, SDL_SCANCODE_D) || InputIsKeyDown(input, SDL_SCANCODE_RIGHT)) -
                                      (InputIsKeyDown(input, SDL_SCANCODE_A) || InputIsKeyDown(input, SDL_SCANCODE_LEFT)));
}

void PlayerDraw(Player *player, Camera *camera)
{
    EntityDraw(player->entity, camera);
}