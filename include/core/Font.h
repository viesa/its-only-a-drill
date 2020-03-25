#ifndef FONTS_H
#define FONTS_H

#include "Graphics.h"

typedef struct Font Font;

typedef enum FontSheet
{
    TTF_Arial,
    TTF_Count
} FontSheet;

Font *FontCreate(Graphics *gfx);
void FontDraw(Font *font, FontSheet fontEnum, char text[], int x, int y, SDL_Color color);
void FontDestroy(Font *font);

#endif