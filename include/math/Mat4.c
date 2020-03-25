#include "Mat4.h"

#include "../core/Library.h"
#include <math.h>

#include <assert.h>

#pragma GCC diagnostic ignored "-Wmissing-braces"
#pragma GCC diagnostic ignored "-Wpedantic"

Mat4 Mat4Add(const Mat4 m1, const Mat4 m2)
{
    Mat4 ret = m1;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            ret.elements[i][j] += m2.elements[i][j];
        }
    }
    return ret;
}
Mat4 Mat4Sub(const Mat4 m1, const Mat4 m2)
{
    Mat4 ret = m1;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            ret.elements[i][j] -= m2.elements[i][j];
        }
    }
    return ret;
}
Mat4 Mat4Mul(const Mat4 m1, const Mat4 m2)
{
    Mat4 ret;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            float t_sum = 0.0f;
            for (int k = 0; k < 4; k++)
            {
                t_sum += m1.elements[i][k] * m2.elements[k][j];
            }
            ret.elements[i][j] = t_sum;
        }
    }
    return ret;
}
Mat4 Mat4MulArr(const Mat4 all[], int n)
{
    assert(n > 1);
    Mat4 ret = all[0];
    for (int i = 1; i < n; i++)
    {
        ret = Mat4Mul(ret, all[i]);
    }
    return ret;
}
SDL_bool Mat4Equ(const Mat4 m1, const Mat4 m2)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (m1.elements[i][j] != m2.elements[i][j])
                return SDL_FALSE;
        }
    }
    return SDL_TRUE;
}

Mat4 Mat4MulL(const Mat4 m, const float rhs)
{
    Mat4 ret = m;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            ret.elements[i][j] *= rhs;
        }
    }
    return ret;
}

Vec3 Vec3MulMat4(const Vec3 v, const Mat4 m)
{
    return (Vec3){
        v.x * m.elements[0][0] + v.y * m.elements[1][0] + v.z * m.elements[2][0],
        v.x * m.elements[0][1] + v.y * m.elements[1][1] + v.z * m.elements[2][1],
        v.x * m.elements[0][2] + v.y * m.elements[1][2] + v.z * m.elements[2][2]};
}
Vec4 Vec4MulMat4(const Vec4 v, const Mat4 m)
{
    return (Vec4){
        v.x * m.elements[0][0] + v.y * m.elements[1][0] + v.z * m.elements[2][0] + v.w * m.elements[3][0],
        v.x * m.elements[0][1] + v.y * m.elements[1][1] + v.z * m.elements[2][1] + v.w * m.elements[3][1],
        v.x * m.elements[0][2] + v.y * m.elements[1][2] + v.z * m.elements[2][2] + v.w * m.elements[3][2],
        v.x * m.elements[0][3] + v.y * m.elements[1][3] + v.z * m.elements[2][3] + v.w * m.elements[3][3]};
}

Mat4 Mat4Identity()
{
    return (Mat4){{1.0f,
                   0.0f,
                   0.0f,
                   0.0f,
                   0.0f,
                   1.0f,
                   0.0f,
                   0.0f,
                   0.0f,
                   0.0f,
                   1.0f,
                   0.0f,
                   0.0f,
                   0.0f,
                   0.0f,
                   1.0f}};
}
Mat4 Mat4Scaling(float factor)
{
    return (Mat4){{factor,
                   0.0f,
                   0.0f,
                   0.0f,
                   0.0f,
                   factor,
                   0.0f,
                   0.0f,
                   0.0f,
                   0.0f,
                   factor,
                   0.0f,
                   0.0f,
                   0.0f,
                   0.0f,
                   1.0f}};
}
Mat4 Mat4Rotation(Mat4RotationAxis axis, float angle)
{
    const float sinAngle = sin(angle);
    const float cosAngle = cos(angle);
    switch (axis)
    {
    case AXIS_X:
        return (Mat4){{1.0f,
                       0.0f,
                       0.0f,
                       0.0f,
                       0.0f,
                       cosAngle,
                       sinAngle,
                       0.0f,
                       0.0f,
                       -sinAngle,
                       cosAngle,
                       0.0f,
                       0.0f,
                       0.0f,
                       0.0f,
                       1.0f}};
    case AXIS_Y:
        return (Mat4){{cosAngle,
                       0.0f,
                       -sinAngle,
                       0.0f,
                       0.0f,
                       1.0f,
                       0.0f,
                       0.0f,
                       sinAngle,
                       0.0f,
                       cosAngle,
                       0.0f,
                       0.0f,
                       0.0f,
                       0.0f,
                       1.0f}};
    case AXIS_Z:
        return (Mat4){{cosAngle,
                       sinAngle,
                       0.0f,
                       0.0f,
                       -sinAngle,
                       cosAngle,
                       0.0f,
                       0.0f,
                       0.0f,
                       0.0f,
                       1.0f,
                       0.0f,
                       0.0f,
                       0.0f,
                       0.0f,
                       1.0f}};
    default:
        return Mat4Identity();
    }
}
Mat4 Mat4Translation(const Vec3 translation)
{
    return (Mat4){{1.0f,
                   0.0f,
                   0.0f,
                   0.0f,
                   0.0f,
                   1.0f,
                   0.0f,
                   0.0f,
                   0.0f,
                   0.0f,
                   1.0f,
                   0.0f,
                   translation.x,
                   translation.y,
                   translation.z,
                   1.0f}};
}
Mat4 Mat4Projection(float w, float h, float n, float f)
{
    return (Mat4){{(2.0f * n) / w,
                   0.0f,
                   0.0f,
                   0.0f,
                   0.0f,
                   (2.0 * n) / h,
                   0.0f,
                   0.0f,
                   0.0f,
                   0.0f,
                   f / (f - n),
                   1.0f,
                   0.0f,
                   0.0f,
                   (-n * f) / (f - n),
                   0.0f}};
}
Mat4 Mat4ProjectionHFOV(float fov, float ar, float n, float f)
{
    const float fov_rad = fov * PI / 180.0f;
    const float w = 1.0f / tan(fov_rad / 2.0f);
    const float h = w * ar;
    return (Mat4){{w,
                   0.0f,
                   0.0f,
                   0.0f,
                   0.0f,
                   h,
                   0.0f,
                   0.0f,
                   0.0f,
                   0.0f,
                   f / (f - n),
                   1.0f,
                   0.0f,
                   0.0f,
                   -n * f / (f - n),
                   0.0f}};
}