#include "Font.h"

#include "Log.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

Font *FontCreate(Graphics *gfx)
{
    if (TTF_Init() < 0)
    {
        log_error("Could not initialize fonts: %s", SDL_GetError());
    }

    Font *font = (Font *)SDL_malloc(sizeof(font));
    font->gfx = gfx;

    font->fonts[TTF_Arial] = TTF_OpenFont("./assets/fonts/arial.ttf", 25); //filepath, size
    font->fonts[TTF_Robot_Crush] = TTF_OpenFont("./assets/fonts/Robot Crush.ttf", 50);

    return font;
}

void FontDraw(Font *font, FontSheet fontEnum, char text[], int x, int y, SDL_Color color)
{
    SDL_Surface *surface = TTF_RenderText_Solid(font->fonts[fontEnum], text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(GraphicsGetRenderer(font->gfx), surface);

    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = {x, y, texW, texH};

    SDL_RenderCopy(GraphicsGetRenderer(font->gfx), texture, NULL, &dstrect);
}

SDL_Rect FontGetSize(Font *font, FontSheet fontEnum, char text[])
{
    SDL_Surface *surface = TTF_RenderText_Solid(font->fonts[fontEnum], text, (SDL_Color){0, 0, 0});
    SDL_Texture *texture = SDL_CreateTextureFromSurface(GraphicsGetRenderer(font->gfx), surface);
    int x = 0;
    int y = 0;
    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    return (SDL_Rect){x, y, texW, texH};

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void FontDraw3D(Font *font, FontSheet fontEnum, char text[], int x, int y, int offset, SDL_Color color1, SDL_Color color2)
{
    FontDraw(font, TTF_Robot_Crush, text, x - offset, y - offset, color1);
    FontDraw(font, TTF_Robot_Crush, text, x, y, color2);
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
