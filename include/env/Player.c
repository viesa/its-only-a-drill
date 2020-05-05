#include "Player.h"

#include "Library.h"

Player PlayerCreate(Camera *camera)
{
    Player ret;
    ret.entity = EntityManagerAdd(ET_Player, Vec2Create(1000.0f + rand() % 200 - 100, 1000.0f + rand() % 200 - 100));
    ret.leg = AnimCreate(AN_PlayerLegs, ANRO_RepeatFromEnd, SS_Character_Prisoner, 4, 0.05f);
    ret.body = AnimCreate(AN_PlayerBody, ANRO_RepeatFromEnd, SS_Character_Prisoner, 4, 0.05f);
    ret.aimFollow = Vec2Create(0.0f, 0.0f);
    ret.forward = Vec2Create(1.0f, 0.0f);
    ENTITY_ARRAY[*ret.entity].inventory = InventoryCreate();
    return ret;
}

void PlayerUpdate(Player *player, Camera *camera)
{
    // camera
    Vec2 mousePos = InputLastMousePos();
    Vec2 cameraPos = CameraGetPos(camera);
    Vec2 playerPos = Vec2Sub(RectMid(ENTITY_ARRAY[*player->entity].drawables[0].dst), cameraPos);

    Vec2 playerToMouse = Vec2Sub(mousePos, playerPos);

    player->forward = Vec2Unit(playerToMouse);
    Vec2 aim = Vec2MulL(player->forward, RADIUS);

    player->aimFollow = Vec2Add(aim, ENTITY_ARRAY[*player->entity].position);

    float vecAngle = toDegrees(Vec2Ang(Vec2Create(1.0f, 0.0f), player->forward));
    float degrees = player->forward.y > 0.0f ? vecAngle : 360 - vecAngle;
    EntityRotateAll(player->entity, degrees);

    // Movment
    ENTITY_ARRAY[*player->entity].Force.y += 500 * ((InputIsKeyDown(SDL_SCANCODE_S) || InputIsKeyDown(SDL_SCANCODE_DOWN)) -
                                                    (InputIsKeyDown(SDL_SCANCODE_W) || InputIsKeyDown(SDL_SCANCODE_UP)));
    ENTITY_ARRAY[*player->entity].Force.x += 500 * ((InputIsKeyDown(SDL_SCANCODE_D) || InputIsKeyDown(SDL_SCANCODE_RIGHT)) -
                                                    (InputIsKeyDown(SDL_SCANCODE_A) || InputIsKeyDown(SDL_SCANCODE_LEFT)));
    // animation
    AnimUpdate(&player->leg, ClockGetDeltaTime());
    AnimUpdate(&player->body, ClockGetDeltaTime());
    if (!InputIsKeyDown(SDL_SCANCODE_A) &&
        !InputIsKeyDown(SDL_SCANCODE_W) &&
        !InputIsKeyDown(SDL_SCANCODE_D) &&
        !InputIsKeyDown(SDL_SCANCODE_S) &&
        !InputIsKeyDown(SDL_SCANCODE_LEFT) &&
        !InputIsKeyDown(SDL_SCANCODE_UP) &&
        !InputIsKeyDown(SDL_SCANCODE_RIGHT) &&
        !InputIsKeyDown(SDL_SCANCODE_DOWN))
    {
        AnimStop(&player->leg);
        AnimPause(&player->body);
        if (rand() % 100 < 3)
            AnimRandomFrame(&player->body);
    }
    else
    {
        AnimResume(&player->leg);
        AnimResume(&player->body);
    }

    AnimApplyToDrawable(&player->leg, &ENTITY_ARRAY[*player->entity].drawables[0], 1.5f);
    AnimApplyToDrawable(&player->body, &ENTITY_ARRAY[*player->entity].drawables[1], 1.5f);
}

void PlayerDraw(Player *player, Camera *camera)
{
    EntityDrawIndex(player->entity, camera);
}