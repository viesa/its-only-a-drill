#ifndef PLAYER_H
#define PLAYER_H

#include "core/Entity.h"
#include "core/Input.h"
#include "math/Vec2.h"

#define RADIUS 25

typedef struct Player
{
    Vec2 aimFollow;
    Vec2 forward;
} Player;

Player PlayerCreate();
void PlayerDestroy(Player *player);

void PlayerUpdate(Player *player, Entity *entity, Input *input, Clock *clock, Camera *camera);

#endif