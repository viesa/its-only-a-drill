#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Drawable.h"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_mouse.h>

typedef enum Cursor
{
    CU_Normal,
    CU_Crossair,
    CU_None,
    CU_Count
} Cursor;

typedef struct Graphics
{
    int windowWidth;
    int windowHeight;
    int mapWidth;
    int mapHeight;
    int isFullscreen;
    SDL_bool vsync;
    Cursor currentCursorType;
    SDL_Cursor *currentCursor;

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
void GraphicsChangeCursor(Graphics *gfx, Cursor cursor);

void GraphicsDrawRect(Graphics *gfx, SDL_Rect rect, SDL_Color color);
void GraphicsDrawPoint(Graphics *gfx, Vec2 pos, size_t radius);

void GraphicsDrawGradient(Graphics *gfx, SDL_Rect area, SDL_Color start, SDL_Color end);

SDL_Renderer *GraphicsGetRenderer(Graphics *gfx);

#endif