#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Drawable.h"

#include <SDL2/SDL_render.h>

typedef struct Graphics
{
    int gfxWindowWidth;
    int gfxWindowHeight;
    int isFullscreen;

    SDL_Window *m_mainWindow;
    SDL_Renderer *m_renderer;

    SDL_Texture *m_allTextures[SS_Count];
} Graphics;

Graphics *GraphicsCreate();
void GraphicsDestroy(Graphics *gfx);

void GraphicsClearScreen(Graphics *gfx);
void GraphicsPresentScreen(Graphics *gfx);

void GraphicsDraw(Graphics *gfx, Drawable drawable);
SDL_Renderer *GraphicsGetRenderer(Graphics *gfx);

#endif