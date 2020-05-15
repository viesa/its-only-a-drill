#include "Font.h"

Font *FontCreate(Graphics *gfx)
{
    Font *font = MALLOC(Font);
    font->gfx = gfx;

    //Debug font
    font->fonts[TTF_Arial] = TTF_OpenFont("./assets/fonts/arial.ttf", 25); //filepath, size

    //Dynamic sizing fonts
    font->fonts[TTF_Antilles_XS] = TTF_OpenFont("./assets/fonts/antillesoutital.ttf", 40);
    font->fonts[TTF_Antilles_S] = TTF_OpenFont("./assets/fonts/antillesoutital.ttf", 50);
    font->fonts[TTF_Antilles_M] = TTF_OpenFont("./assets/fonts/antillesoutital.ttf", 75);
    font->fonts[TTF_Antilles_L] = TTF_OpenFont("./assets/fonts/antillesoutital.ttf", 100);
    font->fonts[TTF_Antilles_XL] = TTF_OpenFont("./assets/fonts/antillesoutital.ttf", 125);
    font->fonts[TTF_Antilles_XXL] = TTF_OpenFont("./assets/fonts/antillesoutital.ttf", 175);

    return font;
}

void FontDraw(Font *font, FontSheet fontEnum, char text[], float x, float y, FontAlign align, int boxWidth, SDL_Color color)
{
    int alignOffsetX = 0;

    //Calculate destination from alignment
    SDL_Rect drawSize = FontGetSize(font, fontEnum, text);

    switch (align)
    {
    case FAL_MENUSIDE:
    case FAL_L:
        break;
    case FAL_C:
        alignOffsetX = drawSize.w / 2;
        break;
    case FAL_R:
        alignOffsetX = drawSize.w;
        break;
    default:
        break;
    }

    if (!boxWidth)
        boxWidth = drawSize.w;

    SDL_Surface *surface = TTF_RenderText_Blended_Wrapped(font->fonts[fontEnum], text, color, boxWidth);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(font->gfx->window->renderer, surface);
    SDL_FreeSurface(surface);

    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = {x - alignOffsetX, y, texW, texH};

    if (align == FAL_MENUSIDE)
    {
        SDL_RenderCopyEx(font->gfx->window->renderer, texture, NULL, &dstrect, -30, &(SDL_Point){0, 0}, SDL_FLIP_NONE);
    }
    else
    {
        SDL_RenderCopy(font->gfx->window->renderer, texture, NULL, &dstrect);
    }

    SDL_DestroyTexture(texture);
}

// FontGetSize
//
// Get estimated size of text to be printed. Returned as a SDL_Rect.
SDL_Rect FontGetSize(Font *font, FontSheet fontEnum, char text[])
{
    SDL_Surface *surface = TTF_RenderText_Solid(font->fonts[fontEnum], text, (SDL_Color){0, 0, 0});
    SDL_Texture *texture = SDL_CreateTextureFromSurface(font->gfx->window->renderer, surface);
    int x = 0;
    int y = 0;
    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    return (SDL_Rect){x, y, texW, texH};
}

int FontGetWidth(Font *font, FontSheet fontEnum, char text[])
{
    SDL_Surface *surface = TTF_RenderText_Solid(font->fonts[fontEnum], text, (SDL_Color){0, 0, 0});
    SDL_Texture *texture = SDL_CreateTextureFromSurface(font->gfx->window->renderer, surface);
    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    return texW;
}

void FontDraw3D(Font *font, FontSheet fontEnum, char text[], float x, float y, FontAlign align, int boxWidth, float offset, Font3dDirection dir, int layers, SDL_Color color[])
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

void FontDraw3DCustom(Font *font, FontSheet fontEnum, char text[], float x, float y, FontAlign align, int boxWidth, float offsetX, float offsetY, int layers, SDL_Color color[])
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

int FontGetHeight(FontSheet fontEnum)
{
    switch (fontEnum)
    {
    case TTF_Arial:
        return 25;
        break;
    case TTF_Antilles_XS:
        return 40;
        break;
    case TTF_Antilles_S:
        return 50;
        break;
    case TTF_Antilles_M:
        return 75;
        break;
    case TTF_Antilles_L:
        return 100;
        break;
    case TTF_Antilles_XL:
        return 125;
        break;
    case TTF_Antilles_XXL:
        return 175;
        break;

    default:
        return 0;
        break;
    }
}

void FontDestroy(Font *font)
{
    for (size_t i = 0; i < TTF_Count; i++)
    {
        TTF_CloseFont(font->fonts[i]);
    }

    SDL_free(font);
}
