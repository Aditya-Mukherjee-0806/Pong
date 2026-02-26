#include "vec2.h"
#include <math.h>

const VEC2 VEC2_LEFT = {-1, 0};
const VEC2 VEC2_RIGHT = {1, 0};
const VEC2 VEC2_UP = {0, -1};
const VEC2 VEC2_DOWN = {0, 1};
const VEC2 VEC2_ZERO = {0, 0};

double VEC2_Magnitude(VEC2 v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

VEC2 VEC2_Normalised(VEC2 v)
{
    double mag = VEC2_Magnitude(v);
    if (mag == 0)
        return VEC2_ZERO;
    return (VEC2){
        .x = v.x / mag,
        .y = v.y / mag,
    };
}

VEC2 VEC2_Sum(VEC2 v1, VEC2 v2)
{
    return (VEC2){
        .x = v1.x + v2.x,
        .y = v1.y + v2.y,
    };
}

VEC2 VEC2_Difference(VEC2 v1, VEC2 v2)
{
    return (VEC2){
        .x = v1.x - v2.x,
        .y = v1.y - v2.y,
    };
}

VEC2 VEC2_Scale(VEC2 v, double k)
{
    return (VEC2){
        .x = v.x * k,
        .y = v.y * k,
    };
}

double VEC2_Dot(VEC2 v1, VEC2 v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}
