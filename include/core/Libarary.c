#include "Library.h"

float min(float f1, float f2)
{
    if (f2 < f1)
    {
        return f2;
    }
    else
    {
        return f1;
    }
}
float max(float f1, float f2)
{
    if (f2 > f1)
    {
        return f2;
    }
    else
    {
        return f1;
    }
}
float sq(float x)
{
    return x * x;
}