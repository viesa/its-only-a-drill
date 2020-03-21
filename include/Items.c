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
void ItemDraw(Camera *camera, Item *item){
    CameraDraw(camera, item->drawable);
}