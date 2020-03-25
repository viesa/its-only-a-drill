#include "Font.h"

#include "Log.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

struct Font
{
    Graphics *gfx;
    TTF_Font *fonts[TTF_Count];
};

Font *FontCreate(Graphics *gfx)
{
    if (TTF_Init() < 0)
    {
        log_error("Could not initialize fonts: %s", SDL_GetError());
    }

    Font *font = (Font *)SDL_malloc(sizeof(font));
    font->gfx = gfx;
    font->fonts[TTF_Arial] = TTF_OpenFont("./assets/fonts/arial.ttf", 25); //filepath, size
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

void FontDestroy(Font *font)
{
    for (size_t i = 0; i < TTF_Count; i++)
    {
        TTF_CloseFont(font->fonts[i]);
    }

    SDL_free(font);
    TTF_Quit();
}
