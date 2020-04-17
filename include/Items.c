#include "Items.h"
#include "core/Log.h"
#define MAX_PLYER_ITEMS 10
#define MAX_GROUND_ITEMS 50

Item ItemCreate(ItemType type, Vec2 Position)
{
    Item i = {0};
    switch (type)
    {
    case ItemWoodenSword:
        i.drawable = DrawableCreate((SDL_Rect){0, 0, 16, 16}, (SDL_Rect){50, 50, 30, 30}, SS_Weapons);
        i.type = ItemWoodenSword;
        i.postion = Position;
        i.picked = 0;
        i.Stats.accuracy = 1.0f;
        i.Stats.ammo = 12;
        i.Stats.captivity = 12;
        i.Stats.Damage = 50;
        i.Stats.falloff = 500;
        break;
    case ItemMetalSword:
        i.drawable = DrawableCreate((SDL_Rect){16, 16, 16, 16}, (SDL_Rect){50, 50, 30, 30}, SS_Weapons);
        i.type = ItemMetalSword;
        i.postion = Position;
        i.picked = 0;
        i.Stats.accuracy = 1.0f;
        i.Stats.ammo = 12;
        i.Stats.captivity = 12;
        i.Stats.Damage = 50;
        i.Stats.falloff = 500;
        break;
    case ItemEmpty:
        i.picked = 1;
        i.type = ItemEmpty;
        i.Stats.accuracy = 0.0f;
        i.Stats.ammo = 0;
        i.Stats.captivity = 0;
        i.Stats.Damage = 0;
        i.Stats.falloff = 0;
        break;
    default:
        i.Stats.accuracy = 0.0f;
        i.Stats.ammo = 0;
        i.Stats.captivity = 0;
        i.Stats.Damage = 0;
        i.Stats.falloff = 0;
        break;
    }
    return i;
}

void ItemDraw(Camera *camera, Item *item, Vec2 pos)
{
    if (item->picked)
    {
        return;
    }
    else
    {
        item->postion = pos;
        item->drawable.dst.x = item->postion.x;
        item->drawable.dst.y = item->postion.y;
        CameraDraw(camera, item->drawable);
    }
}

void ItemPocketDraw(Graphics *gfx, Item *item, Vec2 pos) 
{
   if (item->picked)
    {
        item->postion = pos;
        item->drawable.dst.x = item->postion.x;
        item->drawable.dst.y = item->postion.y;
        GraphicsDraw(gfx, item->drawable);
    }
}

//Plocka up item
void ItemPickup(InventoryListItems *i, Item *y, GroundListItems *g, int tmp)
{
    y->picked = 1;
    i->contents[i->top].drawable = y->drawable;
    i->contents[i->top].picked = y->picked;
    i->contents[i->top].postion = y->postion;
    i->contents[i->top].type = y->type;
    i->contents[i->top].Stats.accuracy = y->Stats.accuracy;
    i->contents[i->top].Stats.ammo = y->Stats.ammo;
    i->contents[i->top].Stats.captivity = y->Stats.captivity;
    i->contents[i->top].Stats.Damage = y->Stats.Damage;
    i->contents[i->top].Stats.falloff = y->Stats.falloff;

    i->top++;
    for (int x = tmp; x < g->top; x++)
    {
        int y = x + 1;
        g->contents[x].drawable = g->contents[y].drawable;
        g->contents[x].picked = g->contents[y].picked;
        g->contents[x].postion = g->contents[y].postion;
        g->contents[x].type = g->contents[y].type;
        g->contents[x].Stats.accuracy = g->contents[y].Stats.accuracy;
        g->contents[x].Stats.ammo = g->contents[y].Stats.ammo;
        g->contents[x].Stats.captivity = g->contents[y].Stats.captivity;
        g->contents[x].Stats.Damage = g->contents[y].Stats.Damage;
        g->contents[x].Stats.falloff = g->contents[y].Stats.falloff;
    }
    g->top--;
}

void ItemDrop(GroundListItems *g, InventoryListItems *i, Vec2 playerPos) //Drop item
{
    i->contents[i->top - 1].picked = 0;
    i->contents[i->top - 1].postion.x = playerPos.x;
    i->contents[i->top - 1].postion.y = playerPos.y;
    groundListAdd(g, i);
    inventoryPop(i);
}
GroundListItems GroundListCreate(void)
{
    GroundListItems ground;
    ground.contents[0] = ItemCreate(ItemWoodenSword, (Vec2){200, 200});
    ground.contents[1] = ItemCreate(ItemMetalSword, (Vec2){200, 400});
    ground.top = 2;
    return ground;
}

InventoryListItems InventoryCreate(void)
{
    InventoryListItems i;
    i.contents[0] = ItemCreate(ItemWoodenSword, (Vec2){200, 200}); // to have the default case.
    i.contents[0].picked = 1;
    i.top = 1;
    for (int tmp = 1; tmp < MAX_PLYER_ITEMS; tmp++)
    {
        i.contents[tmp] = ItemCreate(ItemEmpty, (Vec2){999, 999});
    }

    return i;
}

void groundListAdd(GroundListItems *g, InventoryListItems *i)
{
    g->contents[g->top].drawable = i->contents[i->top - 1].drawable;
    g->contents[g->top].picked = i->contents[i->top - 1].picked;
    g->contents[g->top].postion = i->contents[i->top - 1].postion;
    g->contents[g->top].type = i->contents[i->top - 1].type;
    g->top++;
}

void inventoryPop(InventoryListItems *i)
{
    // the item that will be drop must be in the last available place in the investory list array!
    i->top--;
}
void UpdateItemDraw(InventoryListItems *Inventory, GroundListItems *Ground, Camera *camera)
{
    for (int i = 0; i < Ground->top; i++)
    {
        ItemDraw(camera, &Ground->contents[i], Ground->contents[i].postion);
    }

    return;
}
