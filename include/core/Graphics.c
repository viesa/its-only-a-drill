#include "Graphics.h"

#include "Log.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define DegBug

Graphics *GraphicsCreate()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        log_error("Could not initialize video: %s", SDL_GetError());
    }

    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetSwapInterval(1);

    //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "8");

    Graphics *gfx_ret = (Graphics *)SDL_malloc(sizeof(Graphics));

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    gfx_ret->windowWidth = DM.w;
    gfx_ret->windowHeight = DM.h - 50; // Remove 50 pixels to account for window not being in fullscreen, and compensate for menu bars.
#ifdef DegBug
    gfx_ret->windowWidth = 640;
    gfx_ret->windowHeight = 480;
#endif
    gfx_ret->mapWidth = 2000;
    gfx_ret->mapHeight = 2000;
    gfx_ret->isFullscreen = 0;
    gfx_ret->vsync = SDL_TRUE;

    gfx_ret->m_mainWindow = SDL_CreateWindow("It's only a drill", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gfx_ret->windowWidth, gfx_ret->windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!gfx_ret->m_mainWindow)
        log_fatal("Could not create window: %s", SDL_GetError());

    gfx_ret->m_renderer = SDL_CreateRenderer(gfx_ret->m_mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(gfx_ret->m_renderer, SDL_BLENDMODE_BLEND);

    //Sets Window-icon
    SDL_Surface *win_icon = SDL_LoadBMP("assets/window_icon.bmp");
    SDL_SetWindowIcon(gfx_ret->m_mainWindow, win_icon);
    SDL_FreeSurface(win_icon);

    //INIT ALL TEXTURES
    SDL_Texture *texture;
    texture = IMG_LoadTexture(gfx_ret->m_renderer, "docs/spritesheets/menu.png");
    if (!texture)
        log_warn("Could not load menu.png");
    gfx_ret->m_allTextures[SS_Menu] = texture;

    texture = IMG_LoadTexture(gfx_ret->m_renderer, "docs/spritesheets/old/tilemap.png");
    if (!texture)
        log_warn("Could not load texture.png");
    gfx_ret->m_allTextures[SS_Legacy] = texture;

    texture = IMG_LoadTexture(gfx_ret->m_renderer, "docs/spritesheets/old/spritesheet_tiles.png");
    if (!texture)
        log_warn("Could not load spritesheet_tiles.png");
    gfx_ret->m_allTextures[SS_Tiles] = texture;

    texture = IMG_LoadTexture(gfx_ret->m_renderer, "docs/spritesheets/old/spritesheet_characters.png");
    if (!texture)
        log_warn("Could not load spritesheet_characters.png");
    gfx_ret->m_allTextures[SS_Characters] = texture;

    texture = IMG_LoadTexture(gfx_ret->m_renderer, "docs/spritesheets/old/tools.png");
    if (!texture)
        log_warn("Could not load tools.png");
    gfx_ret->m_allTextures[SS_Tools] = texture;

    texture = IMG_LoadTexture(gfx_ret->m_renderer, "docs/spritesheets/weapons.png");
    if (!texture)
        log_warn("Could not load weapons.png");
    gfx_ret->m_allTextures[SS_Weapons] = texture;

    texture = IMG_LoadTexture(gfx_ret->m_renderer, "docs/spritesheets/background-tiles.png");
    if (!texture)
        log_warn("Could not load background-tiles.png");
    gfx_ret->m_allTextures[SS_BackgroundTiles] = texture;

    texture = IMG_LoadTexture(gfx_ret->m_renderer, "assets/img/red_circle.png");
    if (!texture)
        log_warn("Could not load red_circle.png");
    gfx_ret->m_allTextures[SS_RedCircle] = texture;

    return gfx_ret;
}

void GraphicsDestroy(Graphics *gfx)
{
    for (int i = 0; i < SS_Count; i++)
    {
        SDL_DestroyTexture(gfx->m_allTextures[i]);
    }
    SDL_DestroyRenderer(gfx->m_renderer);
    SDL_DestroyWindow(gfx->m_mainWindow);
    SDL_free(gfx);
}

void GraphicsClearScreen(Graphics *gfx)
{
    SDL_SetRenderDrawColor(gfx->m_renderer, 0, 0, 0, 0);
    SDL_RenderClear(gfx->m_renderer);
}
void GraphicsPresentScreen(Graphics *gfx)
{
    SDL_RenderPresent(gfx->m_renderer);
}

void GraphicsDraw(Graphics *gfx, Drawable drawable)
{
    SDL_Point rot_point = (SDL_Point){(int)drawable.rot_anchor.x, (int)drawable.rot_anchor.y};
    SDL_RenderCopyEx(gfx->m_renderer,
                     gfx->m_allTextures[drawable.spriteSheet],
                     &drawable.src,
                     &drawable.dst,
                     drawable.rot,
                     &rot_point,
                     SDL_FLIP_NONE);
}

void GraphicsDrawRect(Graphics *gfx, SDL_Rect rect, SDL_Color color)
{
    SDL_SetRenderDrawColor(gfx->m_renderer, color.r, color.g, color.g, color.a);
    SDL_RenderFillRect(gfx->m_renderer, &rect);
}

void GraphicsDrawPoint(Graphics *gfx, Vec2 pos, size_t radius)
{
    GraphicsDraw(gfx, DrawableCreate((SDL_Rect){0, 0, 2000, 2000}, (SDL_Rect){(int)pos.x, (int)pos.y, radius * 2, radius * 2}, SS_RedCircle));
}

SDL_Renderer *GraphicsGetRenderer(Graphics *gfx)
{
    return gfx->m_renderer;
}