#ifndef Weapon_H
#define Weapon_H

#include "Dependencies.h"
#include "Input.h"
#include "../Items.h"
#include "../Player.h"

#include "Library.h"

void playerShoot(EntityIndexP index, Camera *camera, Input *input, Item item);
void entityShoot(int index, Vec2 Desierdpoint, Item item);

void RayScan(int index, Vec2 makeDestination, SDL_Point point, Item item, Vec2 ForceDir);

#endif