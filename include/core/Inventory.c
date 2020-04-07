#include "Inventory.h"
#include "Graphics.h"
#include "../Items.h"
#include <stdio.h>

void InventoryDisplay(Graphics *gfx) 
{
    Drawable backround = DrawableCreate((SDL_Rect){0, 0, 50, 50}, (SDL_Rect){(gfx->gfxWindowWidth * 0.2), (gfx->gfxWindowHeight * 0.2 ), (gfx->gfxWindowWidth * 0.6), (gfx->gfxWindowHeight * 0.6 )}, SS_Tiles);
    GraphicsDraw(gfx,backround);
    /*for(int i = 0; i < 16 max inventory ; i++ ) 
    {
        
        ItemDraw(camera ,)
    }*/
}