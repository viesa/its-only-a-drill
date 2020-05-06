#ifndef VEC2_H
#define VEC2_H

#include <SDL2/SDL_bits.h>

typedef struct Vec2
{
    float x;
    float y;
} Vec2;

Vec2 Vec2Create(float x, float y);

// add v1 and v2 returns sum
Vec2 Vec2Add(Vec2 v1, Vec2 v2);
// subtract v2 form v1 returns sum
Vec2 Vec2Sub(Vec2 v1, Vec2 v2);
// is the two vektors equal returns 1 or 0
SDL_bool Vec2Equ(Vec2 v1, Vec2 v2);

// add a float to the vektor
Vec2 Vec2AddL(Vec2 v, const float rhs);
// subtract a float form the vektor
Vec2 Vec2SubL(Vec2 v, const float rhs);
// multipli the vektor with a float
Vec2 Vec2MulL(Vec2 v, const float rhs);
// divida the vektor with a float
Vec2 Vec2DivL(Vec2 v, const float rhs);
// float = v1.x * v2.x + v1.y * v2.y
float Vec2Dot(Vec2 v1, Vec2 v2);
// float = Vectors length
float Vec2Len(Vec2 v);
// gets the spuared length
float Vec2LenSq(Vec2 v);
// får enhets vektorn
Vec2 Vec2Unit(Vec2 v);
// inventera vektorn
Vec2 Vec2Inv(Vec2 v);

// Returns the shortest angle between the two vectors in radians
double Vec2Ang(Vec2 v1, Vec2 v2);

#endif