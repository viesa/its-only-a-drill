#ifndef PLAYER_H
#define PLAYER_H

#include "Dependencies.h"
#include "Input.h"
#include "Vec2.h"
#include "EntityManager.h"
#include "Anim.h"

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

void PlayerUpdate(Player *player, Camera *camera);
void PlayerDraw(Player *player, Camera *camera);

#endif