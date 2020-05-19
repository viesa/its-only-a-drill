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
// Note all functions are for 2D and all with extension AABB is axis-aligned bounding box
SDL_bool CollisionPointWithBox_AABB(Vec2 *point, SDL_Rect *Box);
SDL_bool CollisionBoxWithBox_AABB(SDL_Rect *BoxA, SDL_Rect *BoxB);
SDL_bool CollisionCircleWithPoint(Vec2 *point, Circle *circle);
SDL_bool CollisionCircleWithCircle(Circle *circleA, Circle *circleB);
#endif