#include "Vec4.h"
#include "Library.h"

Vec4 Vec4Create(float x, float y, float z, float w)
{
    return (Vec4){x, y, z, w};
}

Vec4 Vec4Add(const Vec4 v1, const Vec4 v2)
{
    return (Vec4){v1.x + v2.x,
                  v1.y + v2.y,
                  v1.z + v2.z,
                  v1.w + v2.w};
}
Vec4 Vec4Sub(const Vec4 v1, const Vec4 v2)
{
    return (Vec4){v1.x - v2.x,
                  v1.y - v2.y,
                  v1.z - v2.z,
                  v1.w - v2.w};
}
SDL_bool Vec4Equ(const Vec4 v1, const Vec4 v2)
{
    return (SDL_bool)(int){v1.x == v2.x &&
                           v1.y == v2.y &&
                           v1.z == v2.z &&
                           v1.w == v2.w};
}

Vec4 Vec4AddL(const Vec4 v, const float rhs)
{
    return (Vec4){v.x + rhs,
                  v.y + rhs,
                  v.z + rhs,
                  v.w + rhs};
}
Vec4 Vec4SubL(const Vec4 v, const float rhs)
{
    return (Vec4){v.x - rhs,
                  v.y - rhs,
                  v.z - rhs,
                  v.w - rhs};
}
Vec4 Vec4MulL(const Vec4 v, const float rhs)
{
    return (Vec4){v.x * rhs,
                  v.y * rhs,
                  v.z * rhs,
                  v.w * rhs};
}
Vec4 Vec4DivL(const Vec4 v, const float rhs)
{
    return (Vec4){v.x / rhs,
                  v.y / rhs,
                  v.z / rhs,
                  v.w / rhs};
}