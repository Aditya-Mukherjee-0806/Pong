#ifndef VEC2_H
#define VEC2_H

typedef struct
{
    double x, y;
} VEC2;

double VEC2_Magnitude(VEC2);
VEC2 VEC2_Normalised(VEC2);
VEC2 VEC2_Sum(VEC2, VEC2);
VEC2 VEC2_Difference(VEC2, VEC2);
VEC2 VEC2_Scale(VEC2, double);
double VEC2_Dot(VEC2, VEC2);

extern const VEC2 VEC2_LEFT;
extern const VEC2 VEC2_RIGHT;
extern const VEC2 VEC2_UP;
extern const VEC2 VEC2_DOWN;
extern const VEC2 VEC2_ZERO;

#endif