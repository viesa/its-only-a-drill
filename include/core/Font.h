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

// Font align enum.
// FAL_(X) [Left, Center, Right]
typedef enum FontAlign
{
    FAL_L,
    FAL_C,
    FAL_R
} FontAlign;

// Font 3d drawing direction.
// F3D_[1][2]: 1[(T)Top, (C)Center, (B)Bottom]
// F3D_[1][2]: 2[(L)Left, (C)Center, (R)Right]
typedef enum Font3dDirection
{
    F3D_TL,
    F3D_TC,
    F3D_TR,
    F3D_CL,
    F3D_CC,
    F3D_CR,
    F3D_BL,
    F3D_BC,
    F3D_BR
} Font3dDirection;

typedef struct Font
{
    Graphics *gfx;
    TTF_Font *fonts[TTF_Count];
} Font;

Font *FontCreate(Graphics *gfx);
void FontDraw(Font *font, FontSheet fontEnum, char text[], int x, int y, FontAlign align, SDL_Color color);
SDL_Rect FontGetSize(Font *font, FontSheet fontEnum, char text[]);
void FontDraw3D(Font *font, FontSheet fontEnum, char text[], int x, int y, FontAlign align, int offset, Font3dDirection dir, int layers, SDL_Color color[]);
void FontDestroy(Font *font);

#endif