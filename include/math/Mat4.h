#ifndef MAT4_H
#define MAT4_H

#include "Vec3.h"
#include "Vec4.h"

typedef struct Mat4
{
    float elements[4][4];
} Mat4;

typedef enum Mat4RotationAxis
{
    AXIS_X,
    AXIS_Y,
    AXIS_Z,
} Mat4RotationAxis;

Mat4 Mat4Add(const Mat4 m1, const Mat4 m2);
Mat4 Mat4Sub(const Mat4 m1, const Mat4 m2);
Mat4 Mat4Mul(const Mat4 m1, const Mat4 m2);
Mat4 Mat4MulArr(const Mat4 all[], int n);
SDL_bool Mat4Equ(const Mat4 m1, const Mat4 m2);

Mat4 Mat4MulL(const Mat4 m, const float rhs);
Vec3 Vec3MulMat4(const Vec3 v, const Mat4 m);
Vec4 Vec4MulMat4(const Vec4 v, const Mat4 m);

Mat4 Mat4Identity();
Mat4 Mat4Scaling(float factor);
Mat4 Mat4Rotation(Mat4RotationAxis axis, float angle);
Mat4 Mat4Translation(const Vec3 translation);
Mat4 Mat4Projection(float w, float h, float n, float f);
Mat4 Mat4ProjectionHFOV(float fov, float ar, float n, float f);

#endif