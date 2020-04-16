#ifndef INVENTORY_H
#define INVENTORY_H
#include "Camera.h"
#include "Graphics.h"
#include "../Items.h"

void InventoryDisplay(Graphics *gfx,Camera *camera, InventoryListItems *i, Vec2 PlayerPos);

#endif