#ifndef VEC4_H
#define VEC4_H

#include "SDL2/SDL_bits.h"

typedef struct Vec4
{
    float x;
    float y;
    float z;
    float w;
} Vec4;

Vec4 Vec4Create(float x, float y, float z, float w);

Vec4 Vec4Add(const Vec4 v1, const Vec4 v2);
Vec4 Vec4Sub(const Vec4 v1, const Vec4 v2);
SDL_bool Vec4Equ(const Vec4 v1, const Vec4 v2);

Vec4 Vec4AddL(const Vec4 v, const float rhs);
Vec4 Vec4SubL(const Vec4 v, const float rhs);
Vec4 Vec4MulL(const Vec4 v, const float rhs);
Vec4 Vec4DivL(const Vec4 v, const float rhs);

#endif