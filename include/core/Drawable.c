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

Drawable DrawableCreateDefaultConfig()
{
    Drawable ret;
    ret.src = (SDL_Rect){0, 0, 0, 0};
    ret.dst = (SDL_Rect){0, 0, 0, 0};
    ret.spriteSheet = SS_None;
    ret.rot = 0.0f;
    ret.rot_anchor = Vec2Create(0.0f, 0.0f);
    return ret;
}