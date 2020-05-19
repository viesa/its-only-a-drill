#ifndef CollisionDetection_H
#define CollisionDetection_H
//includes
#include <stdbool.h>
#include <math.h>
#include "Vec2.h"
// #include <SDL2/SDL_rect.h>
#include "Dependencies.h"

//structs/veriables
typedef struct Circle
{
    Vec2 Position;
    float Radius;
} Circle;

//functions
// Note all functions are for 2D
SDL_bool CollisionPointWithBox_AABB(Vec2 *point, SDL_Rect *Box);
SDL_bool CollisionBoxWithBox_AABB(SDL_Rect *BoxA, SDL_Rect *BoxB);
SDL_bool CollisionCircelWithPoint(Vec2 *point, Circle *circle);
SDL_bool CollisionCircelWithCircel(Circle *circleA, Circle *circleB);

// char is 1 byte preftect to store 4 sides in
char CollisionBoxWithBoxReturnsSide_AABB(SDL_Rect *BoxA, SDL_Rect *BoxB);
#endif