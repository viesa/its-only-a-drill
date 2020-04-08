#ifndef ITEM_H
#define ITEM_H

#include "core/Camera.h"
#define MAX_PLYER_ITEMS 10
#define MAX_GROUND_ITEMS 50

typedef enum ItemType{
    ItemWoodenSword,
    ItemMetalSword,
    ItemEmpty
}ItemType;

typedef struct Item{
    Drawable drawable;
    ItemType type;
    Vec2 postion;
    int picked;
}Item;

////Lista med items som ligger i invemtory
typedef struct inventoryListItems{
    Item contents[MAX_PLYER_ITEMS];
    int top;
}InventoryListItems;

typedef struct groundListItems
{
    Item contents[MAX_GROUND_ITEMS];
    int top;
}GroundListItems;


Item ItemCreate(ItemType type);
void ItemDraw(Camera *camera, Item *item,Vec2 position);

InventoryListItems InventoryCreate(void);
void ItemPickup(InventoryListItems *i,Item *y,GroundListItems *g,int tmp);

GroundListItems GroundListCreate(void);
void ItemDrop(GroundListItems *g,InventoryListItems *i, Vec2 playerPos);
void groundListAdd(GroundListItems *g,InventoryListItems *i);
void inventoryPop(InventoryListItems *i);

#endif