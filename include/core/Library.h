#ifndef LIBRARY_H
#define LIBRARY_H

#include "Dependencies.h"
#include "Vec2.h"

#define INF ((size_t)18446744073709551615)
#define PI_D (3.141592653589793238462643383279)
#define PI (3.14159265358979f)

#ifndef _INC_STDLIB
#define max(a, b) \
  ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a, b) \
  ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })
#endif

#define sq(x) \
  ({ __typeof__ (x) _x = (x); \
     _x * _x; })

#define MakePositive(x) \
  ({ __typeof__ (x) _x = (x); \
     _x * -1; })

Vec2 RectMid(SDL_Rect rect);

double toDegrees(double radians);

float Clamp(float val, float low, float high);

#endif