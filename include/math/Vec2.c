#include "Vec2.h"
#include "../core/Library.h"

Vec2 Vec2Create(float x, float y)
{
    return (Vec2){x, y};
}

Vec2 Vec2Add(const Vec2 *v1, const Vec2 *v2)
{
    return (Vec2){v1->x + v2->x,
                  v1->y + v2->y};
}
Vec2 Vec2Sub(const Vec2 *v1, const Vec2 *v2)
{
    return (Vec2){v1->x - v2->x,
                  v1->y - v2->y};
}
SDL_bool Vec2Equ(const Vec2 *v1, const Vec2 *v2)
{
    return (SDL_bool)(int){v1->x == v2->x &&
                           v1->y == v2->y};
}

Vec2 Vec2AddL(const Vec2 *v, const float rhs)
{
    return (Vec2){v->x + rhs,
                  v->y + rhs};
}
Vec2 Vec2SubL(const Vec2 *v, const float rhs)
{
    return (Vec2){v->x - rhs,
                  v->y - rhs};
}
Vec2 Vec2MulL(const Vec2 *v, const float rhs)
{
    return (Vec2){v->x * rhs,
                  v->y * rhs};
}
Vec2 Vec2DivL(const Vec2 *v, const float rhs)
{
    return (Vec2){v->x / rhs,
                  v->y / rhs};
}

float Vec2Dot(const Vec2 *v1, const Vec2 *v2)
{
    return v1->x * v2->x + v1->y * v2->y;
}
float Vec2Len(const Vec2 *v)
{
    return sqrt(Vec2LenSq(v));
}
float Vec2LenSq(const Vec2 *v)
{
    return sq(v->x) + sq(v->y);
}
Vec2 Vec2Unit(const Vec2 *v)
{
    return Vec2DivL(v, Vec2Len(v));
}