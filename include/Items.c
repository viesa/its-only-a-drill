#include "Items.h"

Item ItemCreate(ItemType type)
{
    Item i;
    switch (type)
    {
    case ItemWoodenSword:
        i.drawable = DrawableCreate((SDL_Rect){0, 0, 16, 16}, (SDL_Rect){50, 50, 30, 30}, SS_Weapons);
        i.type = ItemWoodenSword;
        
        break;
    case ItemMetalSword:
        i.drawable = DrawableCreate((SDL_Rect){16, 16, 16, 16}, (SDL_Rect){50, 50, 30, 30}, SS_Weapons);
        i.type = ItemMetalSword;
    default:
        break;
    }
    return i;
}
void ItemDraw(Camera *camera, Item *item, Vec2 pos)
{
    item->postion = pos;
    item->drawable.dst.x = item->postion.x;
    item->drawable.dst.y = item->postion.y;
    CameraDraw(camera, item->drawable);
}
void ItemPickup(Item *i) //Plocka up item
{
    
}
void ItemDrop(Camera *camera, Item *item) //Droppa item
{
    CameraDraw(camera, item->drawable);
    //remove from inventory, add to ground list
}
void ItemCreateGroundList() //Lista med item som ligger på marken
{
}
void ItemCreateInventoryList() //Lista med items som ligger i invemtory
{
}
