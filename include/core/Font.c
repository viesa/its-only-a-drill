#include "Font.h"

Font *FontCreate(Graphics *gfx)
{
    if (TTF_Init() < 0)
    {
        log_error("Could not initialize fonts: %s", SDL_GetError());
    }

    Font *font = MALLOC(Font);
    font->gfx = gfx;

    font->fonts[TTF_Arial] = TTF_OpenFont("./assets/fonts/arial.ttf", 25); //filepath, size
    font->fonts[TTF_Robot_Crush] = TTF_OpenFont("./assets/fonts/Robot Crush.ttf", 50);
    font->fonts[TTF_Antilles] = TTF_OpenFont("./assets/fonts/antillesoutital.ttf", 50);

    return font;
}

void FontDraw(Font *font, FontSheet fontEnum, char text[], float x, float y, FontAlign align, int boxWidth, SDL_Color color)
{
    int alignOffsetX = 0;

    //Calculate destination from alignment
    SDL_Rect drawSize = FontGetSize(font, fontEnum, text);

    switch (align)
    {
    case FAL_L:
        break;
    case FAL_C:
        alignOffsetX = drawSize.w / 2;
        break;
    case FAL_R:
        alignOffsetX = drawSize.w;
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

    SDL_RenderCopy(font->gfx->window->renderer, texture, NULL, &dstrect);

    //SDL_RenderCopyEx(GraphicsGetRenderer(font->gfx), texture, NULL, &dstrect, 90, &(SDL_Point){0, 0}, SDL_FLIP_NONE);
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

void FontDestroy(Font *font)
{
    for (size_t i = 0; i < TTF_Count; i++)
    {
        TTF_CloseFont(font->fonts[i]);
    }

    SDL_free(font);
    TTF_Quit();
}
