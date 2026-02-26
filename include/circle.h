#ifndef CIRCLE_H
#define CIRCLE_H

typedef struct
{
    double x, y, r;
} CIRCLE;

void CIRCLE_RenderFill(void *renderer, CIRCLE *c);
unsigned char CIRCLE_IsPointInside(CIRCLE *c, double x, double y);

#endif