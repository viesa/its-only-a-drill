#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Drawable.h"

typedef struct Graphics Graphics;

Graphics *GraphicsCreate();
void GraphicsDestroy(Graphics *gfx);

void GraphicsClearScreen(Graphics *gfx);
void GraphicsPresentScreen(Graphics *gfx);

void GraphicsDraw(Graphics *gfx, Drawable drawable);

#endif