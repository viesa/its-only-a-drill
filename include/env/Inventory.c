#include "Inventory.h"

void InventoryDisplay(InventoryListItems *i)
{
    Drawable backround = DrawableCreate((SDL_Rect){0, 0, 50, 50}, (SDL_Rect){0, ((WindowGetHeight() * 0.5) - 75), 300, 150}, SS_Tiles);
    GraphicsDraw(backround);
    for (int j = 0; j < i->top; j++)
    {
        if (j < 5)
        {
            ItemPocketDraw(&i->contents[j], ((Vec2){25 + (j * 50), (WindowGetHeight() * 0.5) - 50}));
        }
        else
        {
            ItemPocketDraw(&i->contents[j], ((Vec2){25 + ((j - 5) * 50), (WindowGetHeight() * 0.5)}));
        }
    }
}

void InventorySelectItem(InventoryListItems *i, int item)
{
    if (i->top >= item)
    {
        item--;
        for (int j = 0; j < i->top; j++)
        {
            i->contents[j].equiped = 0;
        }
        i->contents[item].equiped = 1;
    }
}

void InventoryDisplayEquiped(InventoryListItems *i, Vec2 PlayerPos)
{
    for (int j = 0; j < i->top; j++)
    {
        if ((i->contents[j].equiped) == 1)
        {
            //Item held = ItemCreate(i->contents[j].type,PlayerPos);
            //held.postion = PlayerPos;
            //held.postion.x = 300;
            //held.postion.y = 500;
            //GraphicsDraw(gfx,held.drawable);
            ItemEquipDraw(&i->contents[j], ((Vec2){PlayerPos.x + 20, PlayerPos.y}));
        }
    }
}
