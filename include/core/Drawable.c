#include "Drawable.h"

Drawable DrawableCreate(SDL_Rect src, SDL_Rect dst, SpriteSheet spriteSheet)
{
    Drawable ret;
    ret.src = src;
    ret.dst = dst;
    ret.spriteSheet = spriteSheet;
    ret.rot = 0.0f;
    ret.rot_anchor = Vec2Create(0.0f, 0.0f);
    return ret;
}