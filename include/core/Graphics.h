#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Dependencies.h"

#include "Drawable.h"
#include "Window.h"

void GraphicsInitialize();
void GraphicsUninitialize();

int GraphicsGetMapWidth();
int GraphicsGetMapHeight();

/// Draw texture into the back-buffer
void GraphicsDraw(Drawable drawable);
void GraphicsDrawRect(SDL_Rect rect, SDL_Color color, SDL_bool filled);
void GraphicsDrawPoint(Vec2 pos, size_t radius);
void GraphicsDrawGradientX(SDL_Rect area, SDL_Color start, SDL_Color end);
void GraphicsDrawGradientY(SDL_Rect area, SDL_Color start, SDL_Color end);
void GraphicsDrawLine(int x1, int y1, int x2, int y2, SDL_Color color);

SDL_Texture *GraphicsLoadTexture(char *path);

#endif