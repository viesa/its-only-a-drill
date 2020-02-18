#ifndef VEC3_H
#define VEC3_H

#include "SDL2/SDL_bits.h"

typedef struct Vec3
{
    float x;
    float y;
    float z;
} Vec3;

Vec3 Vec3Create(float x, float y, float z);

Vec3 Vec3Add(const Vec3 *v1, const Vec3 *v2);
Vec3 Vec3Sub(const Vec3 *v1, const Vec3 *v2);
SDL_bool Vec3Equ(const Vec3 *v1, const Vec3 *v2);

Vec3 Vec3AddL(const Vec3 *v, const float rhs);
Vec3 Vec3SubL(const Vec3 *v, const float rhs);
Vec3 Vec3MulL(const Vec3 *v, const float rhs);
Vec3 Vec3DivL(const Vec3 *v, const float rhs);

float Vec3Dot(const Vec3 *v1, const Vec3 *v2);
Vec3 Vec3Cross(const Vec3 *v1, const Vec3 *v2);
float Vec3Len(const Vec3 *v);
float Vec3LenSq(const Vec3 *v);
Vec3 Vec3Unit(const Vec3 *v);
Vec3 Vec3Sat(const Vec3 *v);
Vec3 Vec3Had(const Vec3 *v1, const Vec3 *v2);

#endif