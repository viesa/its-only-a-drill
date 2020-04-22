#include "Graphics.h"

#include <SDL2/SDL_image.h>

#include "Log.h"

Graphics *GraphicsCreate()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        log_error("Could not initialize video: %s", SDL_GetError());
    }

    Graphics *gfx = (Graphics *)SDL_malloc(sizeof(Graphics));

    gfx->window = WindowCreate("It's only a drill");
    gfx->mapWidth = 2000;
    gfx->mapHeight = 2000;

    //INIT ALL TEXTURES
    gfx->m_allTextures[SS_Menu] = GraphicsLoadTexture(gfx, "docs/spritesheets/menu.png");
    gfx->m_allTextures[SS_Legacy] = GraphicsLoadTexture(gfx, "docs/spritesheets/old/tilemap.png");
    gfx->m_allTextures[SS_Tiles] = GraphicsLoadTexture(gfx, "docs/spritesheets/old/spritesheet_tiles.png");
    gfx->m_allTextures[SS_Characters] = GraphicsLoadTexture(gfx, "docs/spritesheets/old/spritesheet_characters.png");
    gfx->m_allTextures[SS_Character_Prisoner] = GraphicsLoadTexture(gfx, "docs/spritesheets/character_prisoner.png");
    gfx->m_allTextures[SS_Tools] = GraphicsLoadTexture(gfx, "docs/spritesheets/old/tools.png");
    gfx->m_allTextures[SS_Weapons] = GraphicsLoadTexture(gfx, "docs/spritesheets/weapons.png");
    gfx->m_allTextures[SS_BackgroundTiles] = GraphicsLoadTexture(gfx, "docs/spritesheets/background-tiles.png");
    gfx->m_allTextures[SS_RedCircle] = GraphicsLoadTexture(gfx, "assets/img/red_circle.png");

    return gfx;
}

void GraphicsDestroy(Graphics *gfx)
{
    for (int i = 0; i < SS_Count; i++)
    {
        SDL_DestroyTexture(gfx->m_allTextures[i]);
    }
    WindowDestroy(gfx->window);
    SDL_free(gfx);
}

void GraphicsDraw(Graphics *gfx, Drawable drawable)
{
    SDL_Point rot_point = (SDL_Point){(int)drawable.rot_anchor.x, (int)drawable.rot_anchor.y};
    SDL_RenderCopyEx(gfx->window->renderer,
                     gfx->m_allTextures[drawable.spriteSheet],
                     &drawable.src,
                     &drawable.dst,
                     drawable.rot,
                     &rot_point,
                     SDL_FLIP_NONE);
}

void GraphicsDrawRect(Graphics *gfx, SDL_Rect rect, SDL_Color color)
{
    SDL_SetRenderDrawColor(gfx->window->renderer, color.r, color.g, color.g, color.a);
    SDL_RenderFillRect(gfx->window->renderer, &rect);
}

void GraphicsDrawPoint(Graphics *gfx, Vec2 pos, size_t radius)
{
    GraphicsDraw(gfx, DrawableCreate((SDL_Rect){0, 0, 2000, 2000}, (SDL_Rect){(int)pos.x, (int)pos.y, radius * 2, radius * 2}, SS_RedCircle));
}

void GraphicsDrawGradient(Graphics *gfx, SDL_Rect rect, SDL_Color start, SDL_Color end)
{
    //Current color channels
    float CurR = (float)start.r;
    float CurG = (float)start.g;
    float CurB = (float)start.b;
    float CurA = (float)start.a;

    //Calculate delta increments for each color channel
    float dR = (float)(end.r - start.r) / (float)(rect.w);
    float dG = (float)(end.g - start.g) / (float)(rect.w);
    float dB = (float)(end.b - start.b) / (float)(rect.w);
    float dA = (float)(end.a - start.a) / (float)(rect.w);

    for (float x = rect.x; x < rect.x + rect.w; x++)
    {
        SDL_Rect drawRect = {x, rect.y, 1, rect.h};
        SDL_SetRenderDrawColor(gfx->window->renderer, (int)CurR, (int)CurG, (int)CurB, (int)CurA);
        SDL_RenderFillRect(gfx->window->renderer, &drawRect);

        //Add an increment to the color channels
        CurR += dR;
        CurG += dG;
        CurB += dB;
        CurA += dA;
    }
}

SDL_Texture *GraphicsLoadTexture(Graphics *gfx, char *path)
{
    SDL_Texture *texture = IMG_LoadTexture(gfx->window->renderer, path);
    if (!texture)
        log_warn("Could not load texture: [%s]", path);
    return texture;
}