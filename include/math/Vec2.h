#ifndef VEC2_H
#define VEC2_H

#include "SDL2/SDL_bits.h"

typedef struct Vec2
{
    float x;
    float y;
} Vec2;

Vec2 Vec2Create(float x, float y);

Vec2 Vec2Add(const Vec2 *v1, const Vec2 *v2);
Vec2 Vec2Sub(const Vec2 *v1, const Vec2 *v2);
SDL_bool Vec2Equ(const Vec2 *v1, const Vec2 *v2);

Vec2 Vec2AddL(const Vec2 *v, const float rhs);
Vec2 Vec2SubL(const Vec2 *v, const float rhs);
Vec2 Vec2MulL(const Vec2 *v, const float rhs);
Vec2 Vec2DivL(const Vec2 *v, const float rhs);

float Vec2Dot(const Vec2 *v1, const Vec2 *v2);
float Vec2Len(const Vec2 *v);
float Vec2LenSq(const Vec2 *v);
Vec2 Vec2Unit(const Vec2 *v);
Vec2 Vec2Sat(const Vec2 *v);
Vec2 Vec2Had(const Vec2 *v1, const Vec2 *v2);

#endif