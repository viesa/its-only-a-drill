#ifndef FONTS_H
#define FONTS_H

#include "Dependencies.h"
#include "Graphics.h"

typedef enum FontSheet
{
    TTF_Antilles_XS,
    TTF_Antilles_S,
    TTF_Antilles_M,
    TTF_Antilles_L,
    TTF_Antilles_XL,
    TTF_Antilles_XXL,
    // TTF_Robot_Crush, Används inte och skapar därför seg-fault när du freear
    TTF_Arial,
    TTF_Count
} FontSheet;

// Font align enum.
// FAL_(X) [Left, Center, Right]
typedef enum FontAlign
{
    FAL_L,
    FAL_C,
    FAL_R,
    FAL_MENUSIDE
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

// FontDraw() - Draws one line of text from string.
// Warning: x is dependent on which alignment setting is used.
//
// fontEnum: choose a font in fontSheet to display.
// text[]: string to display
// x, y: position to display at on screen. This is impacted by align.
// align: aligns text to asked alignment (Left, Centered, Right).
// boxWidth: allows maximum textbox width, enables wrapping.
// color: color to display text in.

void FontDraw(Font *font, FontSheet fontEnum, char text[], float x, float y, FontAlign align, int boxWidth, SDL_Color color);

// Queries the predicted size of the font to be printed, enabling center and right alignment.
SDL_Rect FontGetSize(Font *font, FontSheet fontEnum, char text[]);

// Enables 3d text display. Requires a 3d direction.
// Draws from the furthest-back layer to the first, following the color array.
//
// fontEnum: choose a font in fontSheet to display.
// text[]: string to display
// x, y: position to display at on screen. This is impacted by align.
// align: aligns text to asked alignment (Left, Centered, Right).
// boxWidth: allows maximum textbox width, enables wrapping.
// color: color to display text in.
//
// 3d specific attributes:
//
// offset: how long should each layer be spaced with
// direction: direction of the offset, from the POV of viewing text.
// layers: length of color[]
// color[]: array of all colors to be displayed on text layers.
void FontDraw3D(Font *font, FontSheet fontEnum, char text[], float x, float y, FontAlign align, int boxWidth, float offset, Font3dDirection dir, int layers, SDL_Color color[]);

void FontDraw3DCustom(Font *font, FontSheet fontEnum, char text[], float x, float y, FontAlign align, int boxWidth, float offsetX, float offsetY, int layers, SDL_Color color[]);

FontSheet FontGetDynamicSizing(Font *font);
int FontGetWidth(Font *font, FontSheet fontEnum, char text[]);
int FontGetHeight(FontSheet fontEnum);

void FontDestroy(Font *font);

#endif