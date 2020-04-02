#include "Items.h"

Item ItemCreate(ItemType type){
    Item i;
    switch (type)
    {
        case ItemWoodenSword:
            i.drawable = DrawableCreate((SDL_Rect){0, 0, 16, 16}, (SDL_Rect){50, 50, 30, 30}, SS_Weapons);
            i.type = ItemWoodenSword;
            break;
        default:
            break;
    }
    return i;
}
void ItemDraw(Camera *camera, Item *item,int x,int y){
    item->PosX = x;
    item->PosY = y;
    item->drawable.dst.x = item->PosX;
    item->drawable.dst.y = item->PosY;
    CameraDraw(camera,item->drawable);
}
void ItemPickup(int EntityX, int EntityY) //Plocka up item
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


