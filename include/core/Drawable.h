#ifndef DRAWABLE_H
#define DRAWABLE_H
#include <SDL2/SDL_shape.h>
#include "SpriteSheet.h"

typedef struct Drawable
{
    SDL_Rect src;
    SDL_Rect dst;
    float rot;
    SDL_Point rot_anchor;
    SpriteSheet spriteSheet;

} Drawable;

Drawable DrawableCreate(SDL_Rect src, SDL_Rect dst, SpriteSheet spriteSheet);

#endif