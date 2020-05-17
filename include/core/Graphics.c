#include "Graphics.h"

typedef struct Graphics
{
    SDL_Texture *m_allTextures[SS_Count];

    int mapWidth;
    int mapHeight;
} Graphics;

static Graphics *gfx;

void GraphicsInitialize()
{
    gfx = MALLOC(Graphics);
    ALLOC_ERROR_CHECK(gfx);

    WindowInitialize("It's only a drill");
    gfx->mapWidth = 2000;
    gfx->mapHeight = 2000;

    //INIT ALL TEXTURES
    for (int i = 0; i < SS_Count; i++)
        gfx->m_allTextures[i] = NULL;
    gfx->m_allTextures[SS_Menu] = GraphicsLoadTexture("docs/spritesheets/menu.png");
    gfx->m_allTextures[SS_Lobby] = GraphicsLoadTexture("docs/spritesheets/lobby.png");
    gfx->m_allTextures[SS_Legacy] = GraphicsLoadTexture("docs/spritesheets/old/tilemap.png");
    gfx->m_allTextures[SS_Tiles] = GraphicsLoadTexture("docs/spritesheets/old/spritesheet_tiles.png");
    gfx->m_allTextures[SS_Characters] = GraphicsLoadTexture("docs/spritesheets/old/spritesheet_characters.png");
    gfx->m_allTextures[SS_Character_Prisoner] = GraphicsLoadTexture("docs/spritesheets/Prisoner.png");
    gfx->m_allTextures[SS_Character_ChernobylWorker] = GraphicsLoadTexture("docs/spritesheets/ChernobylWorker.png");
    gfx->m_allTextures[SS_Character_IronMan] = GraphicsLoadTexture("docs/spritesheets/IronMan.png");
    gfx->m_allTextures[SS_Character_iDubbbz] = GraphicsLoadTexture("docs/spritesheets/iDubbbz.png");
    gfx->m_allTextures[SS_Character_OldMan] = GraphicsLoadTexture("docs/spritesheets/OldMan.png");
    gfx->m_allTextures[SS_Character_Sonic] = GraphicsLoadTexture("docs/spritesheets/Sonic.png");
    gfx->m_allTextures[SS_Tools] = GraphicsLoadTexture("docs/spritesheets/old/tools.png");
    gfx->m_allTextures[SS_Weapons] = GraphicsLoadTexture("docs/spritesheets/weapons.png");
    gfx->m_allTextures[SS_BackgroundTiles] = GraphicsLoadTexture("docs/spritesheets/background-tiles.png");
    gfx->m_allTextures[SS_RedCircle] = GraphicsLoadTexture("assets/img/red_circle.png");
    gfx->m_allTextures[SS_Scanline] = GraphicsLoadTexture("assets/img/scanlines.png");
}

void GraphicsUninitialize()
{
    for (int i = 0; i < SS_Count; i++)
    {
        SDL_DestroyTexture(gfx->m_allTextures[i]);
    }
    WindowUninitialize();
    SDL_free(gfx);
}

int GraphicsGetMapWidth()
{
    return gfx->mapWidth;
}

int GraphicsGetMapHeight()
{
    return gfx->mapHeight;
}

void GraphicsDraw(Drawable drawable)
{
    SDL_Point rot_point = (SDL_Point){drawable.rot_anchor.x * (float)drawable.dst.w,
                                      drawable.rot_anchor.y * (float)drawable.dst.h};
    drawable.spriteSheet %= SS_Count;
    SDL_RenderCopyEx(WindowGetRenderer(),
                     gfx->m_allTextures[drawable.spriteSheet],
                     &drawable.src,
                     &drawable.dst,
                     drawable.rot,
                     &rot_point,
                     SDL_FLIP_NONE);
}

void GraphicsDrawRect(SDL_Rect rect, SDL_Color color, SDL_bool filled)
{
    SDL_SetRenderDrawColor(WindowGetRenderer(), color.r, color.g, color.g, color.a);

    if (filled)
    {
        SDL_RenderFillRect(WindowGetRenderer(), &rect);
    }
    else
    {
        SDL_RenderDrawRect(WindowGetRenderer(), &rect);
    }
}

void GraphicsDrawPoint(Vec2 pos, size_t radius)
{
    GraphicsDraw(DrawableCreate((SDL_Rect){0, 0, 2000, 2000}, (SDL_Rect){(int)pos.x, (int)pos.y, radius * 2, radius * 2}, SS_RedCircle));
}

void GraphicsDrawLine(int x1, int y1, int x2, int y2, SDL_Color color)
{
    SDL_SetRenderDrawColor(WindowGetRenderer(), color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(WindowGetRenderer(), x1, y1, x2, y2);
}

void GraphicsDrawGradientX(SDL_Rect rect, SDL_Color start, SDL_Color end)
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
        SDL_Color color = {(int)CurR, (int)CurG, (int)CurB, (int)CurA};
        GraphicsDrawRect((SDL_Rect){x, rect.y, 1, rect.h}, color, SDL_TRUE);
        //Add an increment to the color channels
        CurR += dR;
        CurG += dG;
        CurB += dB;
        CurA += dA;
    }
}

void GraphicsDrawGradientY(SDL_Rect rect, SDL_Color start, SDL_Color end)
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
        SDL_Color color = {(int)CurR, (int)CurG, (int)CurB, (int)CurA};
        GraphicsDrawRect((SDL_Rect){rect.x, y, rect.w, 1}, color, SDL_TRUE);

        //Add an increment to the color channels
        CurR += dR;
        CurG += dG;
        CurB += dB;
        CurA += dA;
    }
}

SDL_Texture *GraphicsLoadTexture(char *path)
{
    SDL_Texture *texture = IMG_LoadTexture(WindowGetRenderer(), path);
    if (!texture)
        log_warn("Could not load texture: [%s]", path);
    return texture;
}