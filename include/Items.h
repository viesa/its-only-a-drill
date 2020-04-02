#ifndef ITEM_H
#define ITEM_H

#include "core/Graphics.h"
#include "core/Camera.h"
typedef enum ItemType{
    ItemWoodenSword,
    ItemMetalSword
}ItemType;
typedef struct Item{
    Drawable drawable;
    ItemType type;
    Vec2 postion;
    int picked;
}Item;
Item ItemCreate(ItemType type);
void ItemDraw(Camera *camera, Item *item,Vec2 position);
#endif