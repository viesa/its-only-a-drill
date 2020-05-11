#include "Graphics.h"

Graphics *GraphicsCreate()
{
    Graphics *gfx = MALLOC(Graphics);

    gfx->window = WindowCreate("It's only a drill");
    gfx->mapWidth = 2000;
    gfx->mapHeight = 2000;

    //INIT ALL TEXTURES
    for (int i = 0; i < SS_Count; i++)
        gfx->m_allTextures[i] = NULL;
    gfx->m_allTextures[SS_Menu] = GraphicsLoadTexture(gfx, "docs/spritesheets/menu.png");
    gfx->m_allTextures[SS_Lobby] = GraphicsLoadTexture(gfx, "docs/spritesheets/lobby.png");
    gfx->m_allTextures[SS_Legacy] = GraphicsLoadTexture(gfx, "docs/spritesheets/old/tilemap.png");
    gfx->m_allTextures[SS_Tiles] = GraphicsLoadTexture(gfx, "docs/spritesheets/old/spritesheet_tiles.png");
    gfx->m_allTextures[SS_Characters] = GraphicsLoadTexture(gfx, "docs/spritesheets/old/spritesheet_characters.png");
    gfx->m_allTextures[SS_Character_Prisoner] = GraphicsLoadTexture(gfx, "docs/spritesheets/Prisoner.png");
    gfx->m_allTextures[SS_Character_ChernobylWorker] = GraphicsLoadTexture(gfx, "docs/spritesheets/ChernobylWorker.png");
    gfx->m_allTextures[SS_Character_IronMan] = GraphicsLoadTexture(gfx, "docs/spritesheets/IronMan.png");
    gfx->m_allTextures[SS_Character_iDubbbz] = GraphicsLoadTexture(gfx, "docs/spritesheets/iDubbbz.png");
    gfx->m_allTextures[SS_Character_OldMan] = GraphicsLoadTexture(gfx, "docs/spritesheets/OldMan.png");
    gfx->m_allTextures[SS_Character_Sonic] = GraphicsLoadTexture(gfx, "docs/spritesheets/Sonic.png");
    gfx->m_allTextures[SS_Tools] = GraphicsLoadTexture(gfx, "docs/spritesheets/old/tools.png");
    gfx->m_allTextures[SS_Weapons] = GraphicsLoadTexture(gfx, "docs/spritesheets/weapons.png");
    gfx->m_allTextures[SS_BackgroundTiles] = GraphicsLoadTexture(gfx, "docs/spritesheets/background-tiles.png");
    gfx->m_allTextures[SS_RedCircle] = GraphicsLoadTexture(gfx, "assets/img/red_circle.png");
    gfx->m_allTextures[SS_Scanline] = GraphicsLoadTexture(gfx, "assets/img/scanlines.png");

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

void GraphicsDrawGradientX(Graphics *gfx, SDL_Rect rect, SDL_Color start, SDL_Color end)
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

void GraphicsDrawGradientY(Graphics *gfx, SDL_Rect rect, SDL_Color start, SDL_Color end)
{
    //Current color channels
    float CurR = (float)start.r;
    float CurG = (float)start.g;
    float CurB = (float)start.b;
    float CurA = (float)start.a;

    //Calculate delta increments for each color channel
    float dR = (float)(end.r - start.r) / (float)(rect.h);
    float dG = (float)(end.g - start.g) / (float)(rect.h);
    float dB = (float)(end.b - start.b) / (float)(rect.h);
    float dA = (float)(end.a - start.a) / (float)(rect.h);

    for (float y = rect.y; y < rect.y + rect.h; y++)
    {
        SDL_Rect drawRect = {rect.x, y, rect.w, 1};
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