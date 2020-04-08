#include "Library.h"

float min(float f1, float f2)
{
    if (f2 < f1)
    {
        return f2;
    }
    else
    {
        return f1;
    }
}
float max(float f1, float f2)
{
    if (f2 > f1)
    {
        return f2;
    }
    else
    {
        return f1;
    }
}
float sq(float x)
{
    return x * x;
}

Vec2 RectMid(SDL_Rect rect)
{
    return Vec2Create((float)rect.x + (float)rect.w / 2.0f, (float)rect.y + (float)rect.h / 2.0f);
}

double toDegrees(double radians)
{
    return (radians * 180) / PI_D;
}