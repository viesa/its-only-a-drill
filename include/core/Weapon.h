#ifndef Weapon_H
#define Weapon_H

#include "Dependencies.h"
#include "Input.h"
#include "../Items.h"
#include "../Player.h"

#include "Library.h"

void playerShoot(EntityIndexP index, Camera *camera, Item *item);
void entityShoot(int *index, Vec2 Desierdpoint, Item *item);

//diffrent types of shooting
void RayScan(int index, Vec2 makeDestination, SDL_Point point, Item *item, Vec2 ForceDir);
// void bullet(int index, Vec2 Destination, SDL_Point point, Item *item, Vec2 Direction);

void weaponUpdate(Item *item);

#endif