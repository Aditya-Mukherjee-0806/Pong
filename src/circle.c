#include <SDL2/SDL.h>
#include "circle.h"
#include "vec2.h"

void CIRCLE_RenderFill(void *renderer, CIRCLE *c)
{
    for (int i = c->x - c->r; i <= c->x + c->r; i++)
    {
        for (int j = c->y - c->r; j <= c->y + c->r; j++)
        {
            if (CIRCLE_IsPointInside(c, i, j))
                SDL_RenderDrawPoint((SDL_Renderer *)renderer, i, j);
        }
    }
}

unsigned char CIRCLE_IsPointInside(CIRCLE *c, double x, double y)
{
    return VEC2_Magnitude(VEC2_Difference((VEC2){x, y}, (VEC2){c->x, c->y})) <= c->r;
}