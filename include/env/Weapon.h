#ifndef Weapon_H
#define Weapon_H

#include "Dependencies.h"
#include "Library.h"
#include "Input.h"
#include "Items.h"
#include "Player.h"
#include "../net/Packager.h"
#include "../net/Client.h"

typedef struct data
{
    int id;
    int damage;
} Data;
//*****************[shooting function]*****************
// playershoot räknar ut unit vektor och senare gör den längre till falloff
void playerShoot(EntityIndexP index, Camera *camera, Item *item, SDL_Renderer *renderer);
// updateras varje frame så måste skrivas anurlonda
void entityShoot(int *index, Vec2 Desierdpoint, Item *item, SDL_Renderer *renderer, Camera *camera);

//**************[Diffrent shooting types]**************
// casts a ray test what collides with it (sends data to server)
void RayScan(EntityIndexP index, Vec2 *direction, Camera *camera, SDL_Renderer *renderer, WeaponStats *stats);
// casts a ray test what collides with it (change is local)
void RayScanSingelplayer(EntityIndexP index, Vec2 *direction, Camera *camera, SDL_Renderer *renderer, WeaponStats *stats);
// takes steps intil collision
void rayMarchingTest(EntityIndexP index, Vec2 *direction, Camera *camera, SDL_Renderer *renderer, WeaponStats *stats);
/// gives the maximum distans before collision
///\param index: index == zero to not test with anything
/// otherwise ignore index entity
float maxDistenBeforeColision(Vec2 point, EntityIndexP index, float maxDistance);
/// returns 1 if a hit and changes data, returns 0 if failed
SDL_bool testLineWithEntitys(Vec2 start, Vec2 end, EntityIndexP ignoreEntity, int *damage);
// casts a ray and changes data on closest hit
void RayScanClosest(EntityIndexP index, Vec2 *direction, Camera *camera, SDL_Renderer *renderer, WeaponStats *stats);
// skapar en projektil *unsused/not updated
// void bullet(int index, Vec2 Destination, SDL_Point point, Item *item, Vec2 Direction);

// draws a color with a preset color (red currently)
void DrawLineOnCanvas(SDL_Renderer *renderer, int x1, int y1, int x2, int y2);
// reserved for project based weapons
void DectectIntersectionKeep();
// updates weapon logik thats needed efter frame like cooldown
void weaponUpdate(Item *item);

#endif