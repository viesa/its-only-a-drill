#include "Inventory.h"

#include <stdio.h>

void InventoryDisplay(Graphics *gfx,Camera *camera,InventoryListItems *i) 
{
    Drawable backround = DrawableCreate((SDL_Rect){0, 0, 50, 50}, (SDL_Rect){(gfx->gfxWindowWidth * 0.2), 
    (gfx->gfxWindowHeight * 0.2 ), (gfx->gfxWindowWidth * 0.6), (gfx->gfxWindowHeight * 0.6 )}, SS_Tiles);
    GraphicsDraw(gfx,backround);
    int j = 0;
    //ItemDraw(camera , &i->contents[j], ((Vec2){100, 200}));
    //CameraDraw(camera, i->contents[j].drawable);
    //for(int j = 0; j < MAX_PLYER_ITEMS ; j++ ) 
    {
       // if(j < 5) 
        {
            
        }
        /*else 
        {
             ItemDraw(camera , i->contents[j].type,  );
        }*/
        
    }
}