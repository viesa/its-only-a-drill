#include "Font.h"

Font *FontCreate(Graphics *gfx)
{
    Font *font = MALLOC(Font);
    font->gfx = gfx;

    FontLoadInAllFonts(font);

    return font;
}

void FontDraw(Font *font, FontSheet fontEnum, char text[], float x, float y, FC_AlignEnum align, int boxWidth, SDL_Color color)
{
    FC_SetDefaultColor(font->fonts[fontEnum], (color));
    FC_DrawAlign(font->fonts[fontEnum], font->gfx->window->renderer, x, y, align, text);
}

int FontGetWidth(Font *font, FontSheet fontEnum, char text[])
{
    return FC_GetWidth(font->fonts[fontEnum], text);
}

int FontGetHeight(Font *font, FontSheet fontEnum, char text[])
{
    return FC_GetHeight(font->fonts[fontEnum], text);
}

int FontGetMaxHeight(Font *font, FontSheet fontEnum)
{
    return FontGetHeight(font, fontEnum, "A");
}

void FontDraw3D(Font *font, FontSheet fontEnum, char text[], float x, float y, FC_AlignEnum align, int boxWidth, float offset, Font3dDirection dir, int layers, SDL_Color color[])
{
    for (size_t i = 0; i < layers; i++)
    {
        switch (dir)
        {
        case F3D_TL:
            FontDraw(font, fontEnum, text, x - offset * i, y - offset * i, align, boxWidth, color[i]);
            break;
        case F3D_TC:
            FontDraw(font, fontEnum, text, x, y - offset * i, align, boxWidth, color[i]);
            break;
        case F3D_TR:
            FontDraw(font, fontEnum, text, x + offset * i, y - offset * i, align, boxWidth, color[i]);
            break;
        case F3D_CL:
            FontDraw(font, fontEnum, text, x - offset * i, y, align, boxWidth, color[i]);
            break;
        case F3D_CC:
            FontDraw(font, fontEnum, text, x, y, align, boxWidth, color[i]);
            break;
        case F3D_CR:
            FontDraw(font, fontEnum, text, x + offset * i, y, align, boxWidth, color[i]);
            break;
        case F3D_BL:
            FontDraw(font, fontEnum, text, x - offset * i, y + offset * i, align, boxWidth, color[i]);
            break;
        case F3D_BC:
            FontDraw(font, fontEnum, text, x, y + offset * i, align, boxWidth, color[i]);
            break;
        case F3D_BR:
            FontDraw(font, fontEnum, text, x + offset * i, y + offset * i, align, boxWidth, color[i]);
            break;
        }
    }
}

void FontDraw3DCustom(Font *font, FontSheet fontEnum, char text[], float x, float y, FC_AlignEnum align, int boxWidth, float offsetX, float offsetY, int layers, SDL_Color color[])
{
    for (size_t i = 0; i < layers; i++)
    {
        FontDraw(font, fontEnum, text, x + offsetX * i, y + offsetY * i, align, boxWidth, color[i]);
    }
}

FontSheet FontGetDynamicSizing(Font *font)
{
    //Dynamic sizes: XS, S(50), M, L, XL
    //Screen resolutions:
    //640x480: XS     307 200
    //1280x720: S     921 600
    // small 1080:  1 830 000
    //1920x1080: M  2 073 600
    //1920x1200: L  2 304 000
    //2560x1440: XL 3 686 400

    //XS:   500 000
    //S:  1 000 000
    //M:  1 500 000
    //L:  2 500 000
    //XL: 3 500 000

    int w = font->gfx->window->width;
    int h = font->gfx->window->height;
    int px = w * h;

    if (px > 3800000)
        return TTF_Antilles_XL;
    if (px > 2500000)
        return TTF_Antilles_L;
    if (px > 1850000)
        return TTF_Antilles_M;
    if (px > 500000)
        return TTF_Antilles_S;

    return TTF_Antilles_XS;
}

void FontReload(Font *font)
{
    FontFreeAllFonts(font);
    FontLoadInAllFonts(font);
}

void FontLoadInAllFonts(Font *font)
{
    // Initialize all font caches
    for (int i = 0; i < TTF_Count; i++)
        font->fonts[i] = FC_CreateFont();

    //Debug font
    FC_LoadFont(font->fonts[TTF_Arial], font->gfx->window->renderer, "./assets/fonts/arial.ttf", 25, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);

    //Dynamic sizing fonts
    FC_LoadFont(font->fonts[TTF_Antilles_XS], font->gfx->window->renderer, "./assets/fonts/antillesoutital.ttf", 40, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
    FC_LoadFont(font->fonts[TTF_Antilles_S], font->gfx->window->renderer, "./assets/fonts/antillesoutital.ttf", 50, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
    FC_LoadFont(font->fonts[TTF_Antilles_M], font->gfx->window->renderer, "./assets/fonts/antillesoutital.ttf", 75, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
    FC_LoadFont(font->fonts[TTF_Antilles_L], font->gfx->window->renderer, "./assets/fonts/antillesoutital.ttf", 100, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
    FC_LoadFont(font->fonts[TTF_Antilles_XL], font->gfx->window->renderer, "./assets/fonts/antillesoutital.ttf", 125, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
    FC_LoadFont(font->fonts[TTF_Antilles_XXL], font->gfx->window->renderer, "./assets/fonts/antillesoutital.ttf", 175, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);

    // Sets correct spacing between every letter
    for (int i = 0; i < TTF_Count; i++)
        FC_SetSpacing(font->fonts[i], -5);
}

void FontFreeAllFonts(Font *font)
{
    for (size_t i = 0; i < TTF_Count; i++)
    {
        FC_FreeFont(font->fonts[i]);
        font->fonts[i] = NULL;
    }
}

void FontDestroy(Font *font)
{
    FontFreeAllFonts(font);
    SDL_free(font);
}
