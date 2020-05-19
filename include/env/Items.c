#include "Items.h"

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
        i.equiped = 0;
        i.Stats.accuracy = 1.0f;
        i.Stats.ammo = 12;
        i.Stats.captivity = 12;
        i.Stats.Damage = 50;
        i.Stats.falloff = 500;
        i.Stats.cooldownMS = 500.0f;
        i.Stats.currentTime = 10.0f;
        i.itemAction = SoundCreate(SF_SwordSwing);
        break;
    case ItemMetalSword:
        i.drawable = DrawableCreate((SDL_Rect){16, 16, 16, 16}, (SDL_Rect){50, 50, 30, 30}, SS_Weapons);
        i.type = ItemMetalSword;
        i.postion = Position;
        i.picked = 0;
        i.equiped = 0;
        i.Stats.accuracy = 1.0f;
        i.Stats.ammo = 12;
        i.Stats.captivity = 12;
        i.Stats.Damage = 50;
        i.Stats.falloff = 200;
        i.Stats.cooldownMS = 250.0f;
        i.Stats.currentTime = 10.0f;
        i.itemAction = SoundCreate(SF_SwordSwing);
        break;
    case Pistol:
        i.drawable = DrawableCreate((SDL_Rect){0, 0, 260, 90}, (SDL_Rect){25, 25, 25, 15}, SS_Real_Weapons);
        i.type = Pistol;
        i.postion = Position;
        i.picked = 0;
        i.equiped = 0;
        i.Stats.accuracy = 1.0f;
        i.Stats.ammo = 12;
        i.Stats.captivity = 12;
        i.Stats.Damage = 50;
        i.Stats.falloff = 200;
        i.Stats.cooldownMS = 250.0f;
        i.Stats.currentTime = 10.0f;
        i.itemAction = SoundCreate(SF_PistolShoot);
        i.itemReload = SoundCreate(SF_ReloadMag);
        break;
    case ItemEmpty:
        i.picked = 1;
        i.type = ItemEmpty;
        i.Stats.accuracy = 0.0f;
        i.Stats.ammo = 0;
        i.Stats.captivity = 0;
        i.Stats.Damage = 0;
        i.Stats.falloff = 0;
        i.Stats.cooldownMS = 0.0f;
        i.Stats.currentTime = 0.0f;
        break;
    default:
        i.Stats.accuracy = 0.0f;
        i.Stats.ammo = 0;
        i.Stats.captivity = 0;
        i.Stats.Damage = 0;
        i.Stats.falloff = 0;
        i.equiped = 0;
        i.Stats.cooldownMS = 0.0f;
        i.Stats.currentTime = 0.0f;
        i.itemAction = SoundCreate(SF_Nothing);
        i.itemReload = SoundCreate(SF_Nothing);
        break;
    }
    return i;
}

void ItemDraw(Item *item, Vec2 pos)
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
        CameraDraw(item->drawable);
    }
}

void ItemPocketDraw(Item *item, Vec2 pos)
{
    if (item->picked)
    {
        item->postion = pos;
        item->drawable.dst.x = item->postion.x;
        item->drawable.dst.y = item->postion.y;
        GraphicsDraw(item->drawable);
    }
}

void ItemEquipDraw(Item *item, Vec2 pos)
{
    if (item->picked)
    {
        item->postion = pos;
        item->drawable.dst.x = item->postion.x;
        item->drawable.dst.y = item->postion.y;

        Vec2 mousePos = InputLastMousePos();
        Vec2 cameraPos = CameraGetPos();
        Vec2 playerPos = Vec2Sub(pos, cameraPos);

        Vec2 playerToMouse = Vec2Sub(mousePos, playerPos);

        Vec2 forward = Vec2Unit(playerToMouse);

        float vecAngle = toDegrees(Vec2Ang(Vec2Create(1.0f, 0.0f), forward));
        float degrees = forward.y > 0.0f ? vecAngle : 360 - vecAngle;
        item->drawable.rot = degrees;
        item->drawable.rot_anchor = (Vec2){(item->drawable.src.x / 2), (item->drawable.src.y / 2)};
        CameraDraw(item->drawable);
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
    i->contents[i->top - 1].equiped = 0;
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
    ground.contents[4] = ItemCreate(ItemWoodenSword, (Vec2){300, 200});
    ground.contents[3] = ItemCreate(ItemMetalSword, (Vec2){300, 400});
    ground.contents[2] = ItemCreate(Pistol, (Vec2){300, 300});
    ground.top = 5;
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

    if (item <= i->top)
    {
        item--;
        i->contents[item].picked = 0;
        i->contents[item].equiped = 0;
        i->contents[item].postion.x = playerPos.x;
        i->contents[item].postion.y = playerPos.y;

        g->contents[g->top].drawable = i->contents[item].drawable;
        g->contents[g->top].picked = i->contents[item].picked;
        g->contents[g->top].postion = i->contents[item].postion;
        g->contents[g->top].type = i->contents[item].type;
        g->contents[g->top].equiped = i->contents[item].equiped;
        g->top++;

        int incitem = item;
        incitem++;
        for (int k = item; k < i->top; k++)
        {
            i->contents[k].drawable = i->contents[incitem].drawable;
            i->contents[k].picked = i->contents[incitem].picked;
            i->contents[k].postion = i->contents[incitem].postion;
            i->contents[k].type = i->contents[incitem].type;
            i->contents[k].equiped = i->contents[incitem].equiped;
            incitem++;
        }
        i->top--;
    }
}

void inventoryPop(InventoryListItems *i)
{
    // the item that will be drop must be in the last available place in the investory list array!
    i->top--;
}
void UpdateItemDraw(InventoryListItems *Inventory, GroundListItems *Ground)
{
    for (int i = 0; i < Ground->top; i++)
    {
        ItemDraw(&Ground->contents[i], Ground->contents[i].postion);
    }

    return;
}