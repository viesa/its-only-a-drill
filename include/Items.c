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
    i->top++;
    for (int x = tmp; x < g->top; x++)
    {
        int y = x + 1;
        g->contents[x].drawable = g->contents[y].drawable;
        g->contents[x].picked = g->contents[y].picked;
        g->contents[x].postion = g->contents[y].postion;
        g->contents[x].type = g->contents[y].type;
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
    ground.contents[2] = ItemCreate(ItemWoodenSword, (Vec2){300, 200});
    ground.contents[3] = ItemCreate(ItemMetalSword, (Vec2){300, 400});
    ground.top = 4;
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

void ItemDynamicDrop(GroundListItems *g, InventoryListItems *i, Vec2 playerPos, int item) 
{
    
    if(item <= i->top) 
    {
        item--;
        i->contents[item].picked = 0;
        i->contents[item].postion.x = playerPos.x;
        i->contents[item].postion.y = playerPos.y;
    
        g->contents[g->top].drawable = i->contents[item].drawable;
        g->contents[g->top].picked = i->contents[item].picked;
        g->contents[g->top].postion = i->contents[item].postion;
        g->contents[g->top].type = i->contents[item].type;
        g->top++;

        int incitem = item;
        incitem++;
        for(int k = item; k < i->top; k++) 
        {
            i->contents[k].drawable = i->contents[incitem].drawable;
            i->contents[k].picked = i->contents[incitem].picked;
            i->contents[k].postion = i->contents[incitem].postion;
            i->contents[k].type = i->contents[incitem].type;
            incitem++;
        }
        i->top--;
    }
    
}


void inventoryPop(InventoryListItems *i)
{
    // the item that will be drop must be in the last available place in the investory list array!
    i->top--;
    // int tmp = 0;
    // for (int x = 0; x <= i->top; x++)
    // {
    //     if (i->contents[x].type == y->type)
    //     {
    //         tmp = x;
    //     }
    // }
    // for (int x = tmp; x < i->top; x++)
    // {
    //     int y = x + 1;
    //     i->contents[x].drawable = i->contents[y].drawable;
    //     i->contents[x].picked = i->contents[y].picked;
    //     i->contents[x].postion = i->contents[y].postion;
    //     i->contents[x].type = i->contents[y].type;
    // }
    // printf("what is \ntemp: %d\ni->top: %d\n", tmp, i->top);
    // if (tmp < 0)
    // {
    //     log_info("warning the the inventory list is fucked!\n");
    // }
}
void UpdateItemDraw(InventoryListItems *Inventory, GroundListItems *Ground, Camera *camera)
{
    for (int i = 0; i < Ground->top; i++)
    {
        ItemDraw(camera, &Ground->contents[i], Ground->contents[i].postion);
    }

    return;
}
