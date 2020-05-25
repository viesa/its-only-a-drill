#include "Player.h"

#include "Library.h"
#include "Client.h"

typedef struct Player
{
    EntityIndexP entity; // Players borrows an entity to control
    PlayerState state;
    InventoryListItems inventory;
    SpriteSheet spriteSheet;

    Vec2 forward;
    Vec2 aimFollow;
    float aimFollowRadius;

    Anim leg;
    Anim body;

    int score;
    float respawnTimer;
    float respawnCooldown;

    Vec2 spawnPoint;
} Player;

static Player *player;

void PlayerInitialize()
{
    player = MALLOC(Player);
    ALLOC_ERROR_CHECK(player);

    player->entity = EntityManagerAdd(ET_Player, Vec2Create(0.0f, 0.0f));
    player->inventory = InventoryCreate();
    player->spriteSheet = SS_Character_Prisoner;
    player->forward = Vec2Create(1.0f, 0.0f);
    player->aimFollow = Vec2Create(0.0f, 0.0f);
    player->aimFollowRadius = 50.0f;
    player->leg = AnimCreate(AN_PlayerLegs, ANRO_RepeatFromEnd, SS_Character_Prisoner, 4, 0.05f);
    player->body = AnimCreate(AN_PlayerBody, ANRO_RepeatFromEnd, SS_Character_Prisoner, 4, 0.05f);
    player->score = 0;
    player->respawnCooldown = 500.0f;
    player->respawnTimer = player->respawnCooldown;
    player->spawnPoint = Vec2Create(0.0f, 0.0f);
}

void PlayerUninitialize()
{
    FREE(player);
}

void PlayerUpdate()
{
    Entity *entity = &ENTITY_ARRAY[*player->entity];
    PlayerCameraUpdate(player);
    if (entity->health <= 0)
    {
        if (player->state != PL_Dead)
            ClientTCPSend(PT_PlayerDead, NULL, 0);
        PlayerKill(player);
    }
    else
    {
        PlayerMomventUpdate(player);
        PlayerAnimationUpdate(player);
        PlayerRotateToCamera(player);
        if (InputIsMousePressed(BUTTON_LEFT))
        {
            if (player->inventory.contents[player->inventory.top - 1].Stats.ammo > 0)
                PlayerShoot(player);
        }
    }
    WeaponUpdate(&player->inventory.contents[player->inventory.top - 1]);
}

void PlayerDraw()
{
    EntityDrawIndex(player->entity);
}

void PlayerCameraUpdate()
{
    // camera
    Vec2 mousePos = InputLastMousePos();
    Vec2 cameraPos = CameraGetPos();
    Vec2 playerPos = Vec2Sub(RectMid(ENTITY_ARRAY[*player->entity].drawables[0].dst), cameraPos);

    Vec2 playerToMouse = Vec2Sub(mousePos, playerPos);

    player->forward = Vec2Unit(playerToMouse);
    Vec2 aim = Vec2MulL(player->forward, player->aimFollowRadius);

    player->aimFollow = Vec2Add(aim, ENTITY_ARRAY[*player->entity].position);
}

void PlayerMomventUpdate()
{
    // Movment
    ENTITY_ARRAY[*player->entity].Force.y += 500 * ((InputIsKeyDown(SDL_SCANCODE_S) || InputIsKeyDown(SDL_SCANCODE_DOWN)) -
                                                    (InputIsKeyDown(SDL_SCANCODE_W) || InputIsKeyDown(SDL_SCANCODE_UP)));
    ENTITY_ARRAY[*player->entity].Force.x += 500 * ((InputIsKeyDown(SDL_SCANCODE_D) || InputIsKeyDown(SDL_SCANCODE_RIGHT)) -
                                                    (InputIsKeyDown(SDL_SCANCODE_A) || InputIsKeyDown(SDL_SCANCODE_LEFT)));
}

void PlayerAnimationUpdate()
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

void PlayerRotateToCamera()
{
    float vecAngle = toDegrees(Vec2Ang(Vec2Create(1.0f, 0.0f), player->forward));
    float degrees = player->forward.y > 0.0f ? vecAngle : 360 - vecAngle;
    EntityRotateAll(player->entity, degrees);
}

void PlayerShoot()
{
    Item *item = &player->inventory.contents[player->inventory.top - 1];
#ifdef PLAYER_DEBUG
    log_debug("current cooldown %f", item->Stats.currentTime);
#endif
    if (item->Stats.currentTime <= 0)
    {
        Entity *entity = PlayerGetEntity(player);

        item->Stats.currentTime = item->Stats.cooldownMS;
        Vec2 mousePos = InputLastMousePos();
        Vec2 cameraPos = CameraGetPos();
        Vec2 playerPos = Vec2Sub(RectMid(entity->drawables[0].dst), cameraPos);

        Vec2 playerToMouse = Vec2Sub(mousePos, playerPos);
        Vec2 unitPlayerToMouse = Vec2Unit(playerToMouse);

        // push back
        entity->Force.x -= item->Stats.falloff;
        entity->Force.y -= item->Stats.falloff;
        //bullet(index, mousePos, point, item, unitPlayerToMouse);
        void *func = WeaponMultiplayerHandler;
        WeaponRayScanClosest(player->entity, &unitPlayerToMouse, &item->Stats, func);
    }
}

void PlayerKill()
{
    Entity *entity = &ENTITY_ARRAY[*player->entity];
    player->respawnTimer = player->respawnCooldown;
    player->state = PL_Dead;
    entity->drawables[0] = DrawableCreate((SDL_Rect){0, 0, 70, 70}, (SDL_Rect){entity->position.x, entity->position.y, 77, 63}, player->spriteSheet);
    entity->nDrawables = 1;
    entity->isCollider = SDL_FALSE;
}

void PlayerRevive()
{
    Entity *entity = &ENTITY_ARRAY[*player->entity];
    player->respawnTimer = player->respawnCooldown;
    player->state = PL_Alive;
    entity->drawables[0] = DrawableCreateDefaultConfig();
    entity->drawables[1] = DrawableCreateDefaultConfig();
    entity->nDrawables = 2;
    entity->isCollider = SDL_TRUE;
    entity->health = 100;
}

void PlayerSetSpriteSheet(SpriteSheet spriteSheet)
{
    Entity *entity = &ENTITY_ARRAY[*player->entity];
    for (int i = 0; i < entity->nDrawables; i++)
        entity->drawables[i].spriteSheet = spriteSheet;
    AnimChangeSpriteSheet(&player->body, spriteSheet);
    AnimChangeSpriteSheet(&player->leg, spriteSheet);
    player->spriteSheet = spriteSheet;
}

Entity *PlayerGetEntity()
{
    return &ENTITY_ARRAY[*player->entity];
}

SpriteSheet PlayerGetSkin()
{
    return player->spriteSheet;
}

Vec2 *PlayerGetAimFollowP()
{
    return &player->aimFollow;
}

void PlayerSetSpawnPoint(Vec2 spawnPoint)
{
    player->spawnPoint = spawnPoint;
}