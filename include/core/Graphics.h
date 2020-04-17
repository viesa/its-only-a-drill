#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Drawable.h"

#include <SDL2/SDL_render.h>

typedef struct Graphics
{
    int windowWidth;
    int windowHeight;
    int mapWidth;
    int mapHeight;
    int isFullscreen;
    SDL_bool vsync;

    SDL_Window *m_mainWindow;
    SDL_Renderer *m_renderer;

    SDL_Texture *m_allTextures[SS_Count];
} Graphics;

Graphics *GraphicsCreate();
void GraphicsDestroy(Graphics *gfx);

/// Removes everything on screen
void GraphicsClearScreen(Graphics *gfx);
/// Swap back-buffer and front-buffer
/// to display everything on screen
void GraphicsPresentScreen(Graphics *gfx);

/// Draw texture into the back-buffer
void GraphicsDraw(Graphics *gfx, Drawable drawable);
SDL_Renderer *GraphicsGetRenderer(Graphics *gfx);

#endif