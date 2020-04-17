#include "Inventory.h"

#include <stdio.h>

void InventoryDisplay(Graphics *gfx,Camera *camera, InventoryListItems *i, Vec2 PlayerPos) 
{
    Drawable backround = DrawableCreate((SDL_Rect){0, 0, 50, 50}, (SDL_Rect){0,((gfx->windowHeight * 0.5) - 75 ), 300,150}, SS_Tiles);
    GraphicsDraw(gfx,backround);
    for (int j = 0; j < i->top; j++) 
    {
        if(j < 5) 
        {
            ItemPocketDraw(gfx , &i->contents[j], ((Vec2){ 25 + (j * 50), (gfx->windowHeight * 0.5) - 50}));
        }
        else 
        {
            ItemPocketDraw(gfx , &i->contents[j], ((Vec2){ 25 + ((j - 5) * 50), (gfx->windowHeight * 0.5)}));
        }
       
    }
}


