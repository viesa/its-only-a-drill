#ifndef INVENTORY_H
#define INVENTORY_H

#include "Dependencies.h"
#include "Camera.h"
#include "Graphics.h"
#include "../Items.h"
#include "../Player.h"
#include "Entity.h"

//Draws the inventory
void InventoryDisplay(Graphics *gfx, InventoryListItems *i);
//Selects the "item"
void InventorySelectItem(InventoryListItems *i, int item);
//Shows the equiped item in the players hand
void InventoryDisplayEquiped(Camera *camera, InventoryListItems *i, Vec2 PlayerPos);

#endif