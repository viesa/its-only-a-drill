#ifndef VEC2_H
#define VEC2_H

#include "SDL2/SDL_bits.h"

typedef struct Vec2
{
    float x;
    float y;
} Vec2;

Vec2 Vec2Create(float x, float y);

Vec2 Vec2Add(Vec2 v1, Vec2 v2);
Vec2 Vec2Sub(Vec2 v1, Vec2 v2);
SDL_bool Vec2Equ(Vec2 v1, Vec2 v2);

Vec2 Vec2AddL(Vec2 v, const float rhs);
Vec2 Vec2SubL(Vec2 v, const float rhs);
Vec2 Vec2MulL(Vec2 v, const float rhs);
Vec2 Vec2DivL(Vec2 v, const float rhs);

float Vec2Dot(Vec2 v1, Vec2 v2);
float Vec2Len(Vec2 v);
float Vec2LenSq(Vec2 v);
Vec2 Vec2Unit(Vec2 v);
Vec2 Vec2Inv(Vec2 v);

// Returns the shortest angle between the two vectors in radians
double Vec2Ang(Vec2 v1, Vec2 v2);

#endif