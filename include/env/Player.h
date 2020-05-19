#ifndef PLAYER_H
#define PLAYER_H

#include "Dependencies.h"
#include "Input.h"
#include "Vec2.h"
#include "EntityManager.h"
#include "Anim.h"
#include "Weapon.h"
#include "Scoreboard.h"

typedef enum PlayerState
{
    PL_Alive,
    PL_Dead,
    PL_Count
} PlayerState;

void PlayerInitialize();
void PlayerUninitialize();

// updates player logic like movement and camera position
void PlayerUpdate();
// draws the player on the canves
void PlayerDraw();
// moves the camera relative to the player and mouse
void PlayerCameraUpdate();
// moves the player with keys
void PlayerMomventUpdate();
// updates the animation
void PlayerAnimationUpdate();
// rotates the player relative to the camera
void PlayerRotateToCamera();
// Player shoot
void PlayerShoot();
// Sets players HP to 0 and state PL_Dead
void PlayerKill();
// Sets players HP to max and state PL_Alie
void PlayerRevive();
// Change spritesheet of all animations
void PlayerSetSpriteSheet(SpriteSheet spriteSheet);
// Sets the spawnpoint of player
void PlayerSetSpawnPoint(Vec2 spawnPoint);

Entity *PlayerGetEntity();
SpriteSheet PlayerGetSkin();
Vec2 *PlayerGetAimFollowP();

#endif