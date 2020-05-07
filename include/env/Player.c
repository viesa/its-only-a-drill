#include "Player.h"

#include "Library.h"
#define respawnCooldown 500

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
    PlayerCameraUpdate(player, camera);
    if (ENTITY_ARRAY[*player->entity].health > 0)
    {
        PlayerMomventUpdate(player);
        PlayerAnimationUpdate(player);
        RotatePlayerToCamera(player);
    }
    else
    {
        if (ENTITY_ARRAY[*player->entity].entityState != EntityDead)
        {
#ifdef PLAYER_RESPAWN
            log_debug("entity died");
#endif
            player->respawnTimer = respawnCooldown;
            ENTITY_ARRAY[*player->entity].entityState = EntityDead;
            ENTITY_ARRAY[*player->entity].drawables[0] = DrawableCreate((SDL_Rect){0, 0, 70, 70}, (SDL_Rect){ENTITY_ARRAY[*player->entity].position.x, ENTITY_ARRAY[*player->entity].position.y, 77, 63}, SS_Character_Prisoner);
            ENTITY_ARRAY[*player->entity].drawables[1] = DrawableCreate((SDL_Rect){0, 0, 70, 70}, (SDL_Rect){ENTITY_ARRAY[*player->entity].position.x, ENTITY_ARRAY[*player->entity].position.y, 77, 63}, SS_Character_Prisoner);
            ENTITY_ARRAY[*player->entity].isCollider = 0;
        }
        else
        {
            player->respawnTimer -= ClockGetDeltaTime();
#ifdef PLAYER_RESPAWN
            log_debug("time to revive= %dms", player->respawnTimer);
#endif
            if (player->respawnTimer <= 0)
            {
#ifdef PLAYER_RESPAWN
                log_debug("entity revived");
#endif
                ENTITY_ARRAY[*player->entity].entityState = ET_Player;
                ENTITY_ARRAY[*player->entity].health = 100;
                ENTITY_ARRAY[*player->entity].drawables[0] = DrawableCreateDefaultConfig();
                ENTITY_ARRAY[*player->entity].drawables[1] = DrawableCreateDefaultConfig();
                ENTITY_ARRAY[*player->entity].isCollider = 1; 
                // set player position to spawnpoint
            }
        }
    }
}

void PlayerCameraUpdate(Player *player, Camera *camera)
{
    // camera
    Vec2 mousePos = InputLastMousePos();
    Vec2 cameraPos = CameraGetPos(camera);
    Vec2 playerPos = Vec2Sub(RectMid(ENTITY_ARRAY[*player->entity].drawables[0].dst), cameraPos);

    Vec2 playerToMouse = Vec2Sub(mousePos, playerPos);

    player->forward = Vec2Unit(playerToMouse);
    Vec2 aim = Vec2MulL(player->forward, RADIUS);

    player->aimFollow = Vec2Add(aim, ENTITY_ARRAY[*player->entity].position);
}

void PlayerMomventUpdate(Player *player)
{
    // Movment
    ENTITY_ARRAY[*player->entity].Force.y += 500 * ((InputIsKeyDown(SDL_SCANCODE_S) || InputIsKeyDown(SDL_SCANCODE_DOWN)) -
                                                    (InputIsKeyDown(SDL_SCANCODE_W) || InputIsKeyDown(SDL_SCANCODE_UP)));
    ENTITY_ARRAY[*player->entity].Force.x += 500 * ((InputIsKeyDown(SDL_SCANCODE_D) || InputIsKeyDown(SDL_SCANCODE_RIGHT)) -
                                                    (InputIsKeyDown(SDL_SCANCODE_A) || InputIsKeyDown(SDL_SCANCODE_LEFT)));
}

void PlayerAnimationUpdate(Player *player)
{
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

void RotatePlayerToCamera(Player *player)
{
    float vecAngle = toDegrees(Vec2Ang(Vec2Create(1.0f, 0.0f), player->forward));
    float degrees = player->forward.y > 0.0f ? vecAngle : 360 - vecAngle;
    EntityRotateAll(player->entity, degrees);
}

void PlayerDraw(Player *player, Camera *camera)
{
    EntityDrawIndex(player->entity, camera);
}