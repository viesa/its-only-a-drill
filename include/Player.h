#ifndef PLAYER_H
#define PLAYER_H

#include "core/Entity.h"
#include "core/Input.h"

#define RADIUS 25

typedef struct Player
{
    Entity entity;
    Vec2 aimFollow;
} Player;

Player PlayerCreate();
void PlayerDestroy(Player *player);

void PlayerUpdate(Player *player, Input *input, Clock *clock, Camera *camera);
void PlayerDraw(Player *player, Camera *camera);

#endif