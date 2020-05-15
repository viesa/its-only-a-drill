#ifndef PLAYER_H
#define PLAYER_H

#include "Dependencies.h"
#include "Input.h"
#include "Vec2.h"
#include "EntityManager.h"
#include "Anim.h"
#include "Weapon.h"

typedef enum PlayerState
{
    PL_Alive,
    PL_Dead,
    PL_Count
} PlayerState;

typedef struct Player Player;

//creates a player and a entity
Player *PlayerCreate(Camera *camera);

void PlayerDestroy(Player *player);

// updates player logic like movement and camera position
void PlayerUpdate(Player *player, Camera *camera);
// draws the player on the canves
void PlayerDraw(Player *player, Camera *camera);
// moves the camera relative to the player and mouse
void PlayerCameraUpdate(Player *player, Camera *camera);
// moves the player with keys
void PlayerMomventUpdate(Player *player);
// updates the animation
void PlayerAnimationUpdate(Player *player);
// rotates the player relative to the camera
void PlayerRotateToCamera(Player *player);
// Player shoot
void PlayerShoot(Player *player, Camera *camera);
// Sets players HP to 0 and state PL_Dead
void PlayerKill(Player *player);
// Sets players HP to max and state PL_Alie
void PlayerRevive(Player *player);
// Change spritesheet of all animations
void PlayerSetSpriteSheet(Player *player, SpriteSheet spriteSheet);

Entity *PlayerGetEntity(Player *player);
Vec2 *PlayerGetAimFollowP(Player *player);

#endif