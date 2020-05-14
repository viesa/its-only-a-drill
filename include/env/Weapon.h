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

// playershoot räknar ut unit vektor och senare gör den längre till falloff
void playerShoot(Graphics *gfx, EntityIndexP index, Camera *camera, Item *item);
// updateras varje frame så måste skrivas anurlonda
void entityShoot(Graphics *gfx, int *index, Vec2 Desierdpoint, Item *item, Camera *camera);

//diffrent types of shooting
// skickar datan över nätet
void RayScan(int index, Vec2 makeDestination, SDL_Point point, Item *item, Vec2 ForceDir);
// hanterar datan i funktionen
void RayScanSingelplayer(int index, Vec2 Destination, SDL_Point point, Item *item, Vec2 ForceDir);

void rayMarchingTest(Graphics *gfx, EntityIndexP index, Vec2 *direction, Camera *camera, WeaponStats *stats);
float maxDistenBeforeColision(Vec2 point, EntityIndexP index, float maxDistance);
SDL_bool testLineWithEntitys(Vec2 start, Vec2 end, EntityIndexP ignoreEntity, int *damage);
void RayScanClosest(Graphics *gfx, EntityIndexP index, Vec2 *direction, Camera *camera, WeaponStats *stats);
// skapar en projektil *unsused/not updated
// void bullet(int index, Vec2 Destination, SDL_Point point, Item *item, Vec2 Direction);
void DectectIntersectionKeep();
void weaponUpdate(Item *item);

#endif