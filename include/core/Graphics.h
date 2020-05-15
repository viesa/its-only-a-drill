#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Dependencies.h"

#include "Window.h"
#include "Drawable.h"

typedef struct Graphics
{
    Window *window;
    SDL_Texture *m_allTextures[SS_Count];

    int mapWidth;
    int mapHeight;
} Graphics;

Graphics *GraphicsCreate();
void GraphicsDestroy(Graphics *gfx);

/// Draw texture into the back-buffer
void GraphicsDraw(Graphics *gfx, Drawable drawable);
void GraphicsDrawRect(Graphics *gfx, SDL_Rect rect, SDL_Color color, SDL_bool filled);
void GraphicsDrawPoint(Graphics *gfx, Vec2 pos, size_t radius);
void GraphicsDrawGradientX(Graphics *gfx, SDL_Rect area, SDL_Color start, SDL_Color end);
void GraphicsDrawGradientY(Graphics *gfx, SDL_Rect area, SDL_Color start, SDL_Color end);
void GraphicsDrawLine(Graphics *gfx, int x1, int y1, int x2, int y2, SDL_Color color);

SDL_Texture *GraphicsLoadTexture(Graphics *gfx, char *path);

#endif