#ifndef FONTS_H
#define FONTS_H

#include "Graphics.h"

#include <SDL2/SDL_ttf.h>

typedef enum FontSheet
{
    TTF_Robot_Crush,
    TTF_Arial,
    TTF_Count
} FontSheet;

typedef struct Font
{
    Graphics *gfx;
    TTF_Font *fonts[TTF_Count];
} Font;

Font *FontCreate(Graphics *gfx);
void FontDraw(Font *font, FontSheet fontEnum, char text[], int x, int y, SDL_Color color);
SDL_Rect FontGetSize(Font *font, FontSheet fontEnum, char text[]);
void FontDraw3D(Font *font, FontSheet fontEnum, char text[], int x, int y, int offset, SDL_Color color1, SDL_Color color2);
void FontDestroy(Font *font);

#endif