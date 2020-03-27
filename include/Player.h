#ifndef PLAYER_H
#define PLAYER_H

#include "core/Entity.h"
#include "core/Input.h"
#include "core/Clock.h"
#include "core/Camera.h"

#define RADIUS 100

typedef struct Player
{
    Entity entity;
    Vec2 aimFollow;
} Player;

Player PlayerCreate();
void PlayerDestroy(Player *player);

void PlayerUpdate(Player *player, Input *input, Clock *clock, Graphics *gfx);
void PlayerDraw(Player *player, Camera *camera);

#endif