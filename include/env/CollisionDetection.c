#include "CollisionDetection.h"

SDL_bool CollisionPointWithBox_AABB(Vec2 *point, SDL_Rect *Box)
{
    return (point->x >= Box->x && point->x <= (Box->x + Box->w)) &&
           (point->y >= Box->y && point->y <= (Box->y + Box->h));
}

SDL_bool CollisionBoxWithBox_AABB(SDL_Rect *BoxA, SDL_Rect *BoxB)
{
    // logic based on https://en->wikipedia->org/wiki/Bounding_volume
    return (BoxA->x <= (BoxB->x + BoxB->w) && (BoxA->x + BoxA->w) >= BoxB->x) &&
           (BoxA->y <= (BoxB->y + BoxB->h) && (BoxA->y + BoxA->h) >= BoxB->y);
}

SDL_bool CollisionCircleWithPoint(Vec2 *point, Circle *circle)
{
    // first part is distance formula, second is the test
    return sqrt(pow((point->x - circle->Position.x), 2.0f) + pow((point->y - circle->Position.y), 2.0f)) < circle->Radius;
}

SDL_bool CollisionCircleWithCircle(Circle *circleA, Circle *circleB)
{
    return sqrt(pow((circleA->Position.y - circleB->Position.x), 2.0f) + pow((circleA->Position.y - circleB->Position.y), 2.0f)) < (circleA->Radius + circleB->Radius);
}