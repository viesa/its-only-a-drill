#include "Graphics.h"

#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

struct Graphics
{
    SDL_Window *m_mainWindow;
    SDL_Renderer *m_renderer;

    SDL_Texture *m_allTextures[SS_Count];
};

Graphics *GraphicsCreate()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        fprintf(stderr, "Error: Could not initialize video");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetSwapInterval(1);

    //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "8");

    Graphics *gfx_ret = (Graphics *)SDL_malloc(sizeof(Graphics));
    gfx_ret->m_mainWindow = SDL_CreateWindow("It's only a drill...", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!gfx_ret->m_mainWindow)
        fprintf(stderr, "Unable to create window");
    gfx_ret->m_renderer = SDL_CreateRenderer(gfx_ret->m_mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    //INIT ALL TEXTURES
    SDL_Texture *tilemap = IMG_LoadTexture(gfx_ret->m_renderer, "assets/tilemap.png");
    if (!tilemap)
        fprintf(stderr, "Error: Could not load tilemap.png\n");
    gfx_ret->m_allTextures[SS_Legacy] = tilemap;

    //INIT ALL TEXTURES
    tilemap = IMG_LoadTexture(gfx_ret->m_renderer, "assets/spritesheet_tiles.png");
    if (!tilemap)
        fprintf(stderr, "Error: Could not load spritesheet_tiles.png\n");
    gfx_ret->m_allTextures[SS_Tiles] = tilemap;

    //INIT ALL TEXTURES
    tilemap = IMG_LoadTexture(gfx_ret->m_renderer, "assets/spritesheet_characters.png");
    if (!tilemap)
        fprintf(stderr, "Error: Could not load spritesheet_characters.png\n");
    gfx_ret->m_allTextures[SS_Characters] = tilemap;

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
    SDL_RenderCopyEx(gfx->m_renderer,
                     gfx->m_allTextures[drawable.spriteSheet],
                     &drawable.src,
                     &drawable.dst,
                     drawable.rot,
                     &drawable.rot_anchor,
                     SDL_FLIP_NONE);
}