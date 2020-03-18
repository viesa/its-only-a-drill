#include "Vec3.h"
#include "../core/Library.h"

Vec3 Vec3Create(float x, float y, float z)
{
    return (Vec3){x, y, z};
}

Vec3 Vec3Add(const Vec3 *v1, const Vec3 *v2)
{
    return (Vec3){v1->x + v2->x,
                  v1->y + v2->y,
                  v1->z + v2->z};
}
Vec3 Vec3Sub(const Vec3 *v1, const Vec3 *v2)
{
    return (Vec3){v1->x - v2->x,
                  v1->y - v2->y,
                  v1->z - v2->z};
}
SDL_bool Vec3Equ(const Vec3 *v1, const Vec3 *v2)
{
    return (SDL_bool)(int){v1->x == v2->x &&
                           v1->y == v2->y &&
                           v1->z == v2->z};
}

Vec3 Vec3AddL(const Vec3 *v, const float rhs)
{
    return (Vec3){v->x + rhs,
                  v->y + rhs,
                  v->z + rhs};
}
Vec3 Vec3SubL(const Vec3 *v, const float rhs)
{
    return (Vec3){v->x - rhs,
                  v->y - rhs,
                  v->z - rhs};
}
Vec3 Vec3MulL(const Vec3 *v, const float rhs)
{
    return (Vec3){v->x * rhs,
                  v->y * rhs,
                  v->z * rhs};
}
Vec3 Vec3DivL(const Vec3 *v, const float rhs)
{
    return (Vec3){v->x / rhs,
                  v->y / rhs,
                  v->z / rhs};
}

float Vec3Dot(const Vec3 *v1, const Vec3 *v2)
{
    return v1->x * v2->x +
           v1->y * v2->y +
           v1->z * v2->z;
}
Vec3 Vec3Cross(const Vec3 *v1, const Vec3 *v2)
{
    Vec3 v_ret;
    v_ret.x = v1->y * v2->z - v1->z * v2->y;
    v_ret.y = v1->z * v2->x - v1->x * v2->z;
    v_ret.z = v1->x * v2->y - v1->y * v2->x;
    return v_ret;
}
float Vec3Len(const Vec3 *v)
{
    return sqrt(Vec3LenSq(v));
}
float Vec3LenSq(const Vec3 *v)
{
    return sq(v->x) + sq(v->y) + sq(v->z);
}
Vec3 Vec3Unit(const Vec3 *v)
{
    return Vec3DivL(v, Vec3Len(v));
}

Vec3 Vec3Sat(const Vec3 *v)
{
    Vec3 v_ret = *v;
    v_ret.x = min(1.0f, max(0.0f, v_ret.x));
    v_ret.y = min(1.0f, max(0.0f, v_ret.y));
    v_ret.z = min(1.0f, max(0.0f, v_ret.z));
    return v_ret;
}
Vec3 Vec3Had(const Vec3 *v1, const Vec3 *v2)
{
    return (Vec3){v1->x * v2->x, v1->y * v2->y, v1->z * v2->z};
}