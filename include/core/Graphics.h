#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Drawable.h"

#include <SDL2/SDL_render.h>

typedef struct Graphics Graphics;

Graphics *GraphicsCreate();
void GraphicsDestroy(Graphics *gfx);

void GraphicsClearScreen(Graphics *gfx);
void GraphicsPresentScreen(Graphics *gfx);

void GraphicsDraw(Graphics *gfx, Drawable drawable);
SDL_Renderer *GraphicsGetRenderer(Graphics *gfx);

#endif