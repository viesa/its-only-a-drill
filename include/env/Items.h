#ifndef ITEM_H
#define ITEM_H

#include "Dependencies.h"
#include "Camera.h"
#define MAX_PLYER_ITEMS 10
#define MAX_GROUND_ITEMS 50

typedef enum ItemType
{
    ItemWoodenSword,
    ItemMetalSword,
    Pistol,
    Sniper,
    MechineGun,
    ItemEmpty
} ItemType;

typedef struct Weapon
{
    int Damage;
    int falloff;
    float accuracy;
    int ammo;
    int captivity;
    float cooldownMS;   // in milisecondes
    float currentTime;  // in milisecondes
    float ReloadTimeMS; // in milisecondes
} WeaponStats;

typedef struct Item
{
    Drawable drawable;
    ItemType type;
    Vec2 postion;
    int picked;
    WeaponStats Stats;
    int equiped;
} Item;

////Lista med items som ligger i invemtory
typedef struct inventoryListItems
{
    Item contents[MAX_PLYER_ITEMS];
    int top;
} InventoryListItems;

typedef struct groundListItems
{
    Item contents[MAX_GROUND_ITEMS];
    int top;
} GroundListItems;

Item ItemCreate(ItemType type, Vec2 postion);
void ItemDraw(Camera *camera, Item *item, Vec2 position);
//Draws items in invemtory
void ItemPocketDraw(Graphics *gfx, Item *item, Vec2 pos);
//Draws the equiped item
void ItemEquipDraw(Camera *camera, Item *item, Vec2 pos);

InventoryListItems InventoryCreate(void);
void ItemPickup(InventoryListItems *i, Item *y, GroundListItems *g, int tmp);

GroundListItems GroundListCreate(void);
void ItemDrop(GroundListItems *g, InventoryListItems *i, Vec2 playerPos);
void groundListAdd(GroundListItems *g, InventoryListItems *i);
void inventoryPop(InventoryListItems *i);
void UpdateItemDraw(InventoryListItems *Inventory, GroundListItems *Ground, Camera *camera);

void ItemDynamicDrop(GroundListItems *g, InventoryListItems *i, Vec2 playerPos, int item);

#endif