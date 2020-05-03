#include "Vec2.h"
#include "Library.h"

Vec2 Vec2Create(float x, float y)
{
    return (Vec2){x, y};
}

Vec2 Vec2Add(Vec2 v1, Vec2 v2)
{
    return (Vec2){v1.x + v2.x,
                  v1.y + v2.y};
}
Vec2 Vec2Sub(Vec2 v1, Vec2 v2)
{
    return (Vec2){v1.x - v2.x,
                  v1.y - v2.y};
}
SDL_bool Vec2Equ(Vec2 v1, Vec2 v2)
{
    return (SDL_bool)(int){v1.x == v2.x &&
                           v1.y == v2.y};
}

Vec2 Vec2AddL(Vec2 v, const float rhs)
{
    return (Vec2){v.x + rhs,
                  v.y + rhs};
}
Vec2 Vec2SubL(Vec2 v, const float rhs)
{
    return (Vec2){v.x - rhs,
                  v.y - rhs};
}
Vec2 Vec2MulL(Vec2 v, const float rhs)
{
    return (Vec2){v.x * rhs,
                  v.y * rhs};
}
Vec2 Vec2DivL(Vec2 v, const float rhs)
{
    if (rhs == 0.0f)
        return Vec2Create(0.0f, 0.0f);
    return (Vec2){v.x / rhs,
                  v.y / rhs};
}

float Vec2Dot(Vec2 v1, Vec2 v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}
float Vec2Len(Vec2 v)
{
    return sqrt(Vec2LenSq(v));
}
float Vec2LenSq(Vec2 v)
{
    return sq(v.x) + sq(v.y);
}
Vec2 Vec2Unit(Vec2 v)
{
    return Vec2DivL(v, Vec2Len(v));
}

Vec2 Vec2Inv(Vec2 v)
{
    return Vec2Create(-v.x, -v.y);
}

double Vec2Ang(Vec2 v1, Vec2 v2)
{
    return acos((Vec2Dot(v1, v2) / (Vec2Len(v1) * Vec2Len(v2))));
}