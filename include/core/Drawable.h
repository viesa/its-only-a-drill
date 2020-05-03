#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "Dependencies.h"
#include "SpriteSheet.h"
#include "Vec2.h"

typedef struct Drawable
{
    SDL_Rect src;
    SDL_Rect dst;
    float rot;
    // Defines rotation anchor in world space NOT camera space
    Vec2 rot_anchor;
    SpriteSheet spriteSheet;

} Drawable;

Drawable DrawableCreate(SDL_Rect src, SDL_Rect dst, SpriteSheet spriteSheet);
Drawable DrawableCreateDefaultConfig();

#endif