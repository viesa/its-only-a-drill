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
    EntityIndexP entity; // player barrows a entity for the player to contole
    Anim leg;
    Anim body;
    Vec2 aimFollow;
    Vec2 forward;
    InventoryListItems inventory;
} Player;

//creates a player and a entity
Player PlayerCreate(Camera *camera);

// updates player logic like movement and camera position
void PlayerUpdate(Player *player, Camera *camera);
// draws the player on the canves
void PlayerDraw(Player *player, Camera *camera);

#endif