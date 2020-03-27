#ifndef PLAYER_H
#define PLAYER_H

#include "core/Entity.h"
#include "core/Input.h"
#include "core/Clock.h"
#include "core/Camera.h"

#define RADIUS 20

typedef struct Player
{
    Entity entity;
} Player;

Player PlayerCreate();
void PlayerDestroy(Player *player);

void PlayerUpdate(Player *player, Input *input, Clock *clock);
void PlayerDraw(Player *player, Camera *camera);

#endif