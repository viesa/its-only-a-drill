#include "Library.h"

Vec2 RectMid(SDL_Rect rect)
{
    return Vec2Create((float)rect.x + (float)rect.w / 2.0f, (float)rect.y + (float)rect.h / 2.0f);
}

double toDegrees(double radians)
{
    return (radians * 180) / PI_D;
}