#ifndef ITEM_H
#define ITEM_H

#include "core/Graphics.h"
#include "core/Camera.h"
typedef enum ItemType{
    ItemWoodenSword
}ItemType;
typedef struct Item{
    Drawable drawable;
    ItemType type;
}Item;
Item ItemCreate(ItemType type);
void ItemDraw(Camera *camera, Item *item);
#endif