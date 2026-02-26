#ifndef COLORS_H
#define COLORS_H

typedef struct
{
    unsigned char r, g, b;
} RGB24;

extern const RGB24 RGB24_BLACK;
extern const RGB24 RGB24_RED;
extern const RGB24 RGB24_GREEN;
extern const RGB24 RGB24_BLUE;
extern const RGB24 RGB24_YELLOW;
extern const RGB24 RGB24_CYAN;
extern const RGB24 RGB24_MAGENTA;
extern const RGB24 RGB24_WHITE;

RGB24 RGB24_GenerateVividColor();
RGB24 RGB24_MixTwoColors(RGB24 color1, RGB24 color2);

#endif