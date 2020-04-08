#ifndef DRAWABLE_H
#define DRAWABLE_H
#include <SDL2/SDL_rect.h>
#include "SpriteSheet.h"
#include "../math/Vec2.h"

typedef struct Drawable
{
    SDL_Rect src;
    SDL_Rect dst;
    float rot;
    Vec2 rot_anchor;
    SpriteSheet spriteSheet;

} Drawable;

Drawable DrawableCreate(SDL_Rect src, SDL_Rect dst, SpriteSheet spriteSheet);

#endif