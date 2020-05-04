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

void playerShoot(EntityIndexP index, Camera *camera, Item *item);
void entityShoot(int *index, Vec2 Desierdpoint, Item *item);

//diffrent types of shooting
void RayScan(int index, Vec2 makeDestination, SDL_Point point, Item *item, Vec2 ForceDir);
void RayScanSingelplayer(int index, Vec2 makeDestination, SDL_Point point, Item *item, Vec2 ForceDir);
// void bullet(int index, Vec2 Destination, SDL_Point point, Item *item, Vec2 Direction);

void weaponUpdate(Item *item);

#endif