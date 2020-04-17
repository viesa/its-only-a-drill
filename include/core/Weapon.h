#ifndef Weapon_H
#define Weapon_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../Items.h"
#include "../Player.h"
#include <SDL2/SDL_mouse.h>

#include "Library.h"

void shoot(Player *player, Camera *camera, Entity e[], Item item);

#endif