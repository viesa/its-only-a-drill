#ifndef PLAYER_H
#define PLAYER_H

#include "core/Dependencies.h"
#include "core/EntityManager.h"
#include "core/Input.h"
#include "core/Anim.h"
#include "math/Vec2.h"

#define RADIUS 25

typedef struct Player
{
    EntityIndexP entity;
    Anim leg;
    Anim body;
    Vec2 aimFollow;
    Vec2 forward;
} Player;

Player PlayerCreate(Camera *camera);
void PlayerDestroy(Player *player);

void PlayerUpdate(Player *player, Input *input, Clock *clock, Camera *camera);
void PlayerDraw(Player *player, Camera *camera);

#endif