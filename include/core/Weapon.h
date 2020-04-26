#ifndef Weapon_H
#define Weapon_H

#include "Dependencies.h"
#include "Input.h"
#include "../Items.h"
#include "../Player.h"

#include "Library.h"

void playerShoot(EntityIndexP index, Camera *camera, Input *input, Item item);

#endif