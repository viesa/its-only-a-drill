#ifndef LIBRARY_H
#define LIBRARY_H

#include <math.h>
#include <SDL2/SDL_rect.h>
#include "../math/Vec2.h"
#include "Log.h"

#define INF ((size_t)18446744073709551615)
#define PI_D (3.141592653589793238462643383279)
#define PI (3.14159265358979f)

float min(float f1, float f2);
float max(float f1, float f2);
float sq(float x);

Vec2 RectMid(SDL_Rect rect);

double toDegrees(double radians);

#endif