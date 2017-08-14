#ifndef PIDMATH_H
#define PIDMATH_H

#include <vector>
#include "pixel.h"
#include "image.h"

#define BLOCK_SIZE 16
#define ROUND_UP(x, s) (((x)+((s)-1)) & -(s))

#define LOG_PRINT 1

class MiscMath{
public:
    unsigned int roundUpToNearestMultiple(unsigned int num, unsigned int multiple);
    unsigned char lookUpPalette(std::vector<Pixel> palette, Pixel color);
};

class Quantization{
public:
    std::vector<Pixel> medianCut2ndAnd3rdChannels(Image* img, unsigned short maxColors);
};

#endif
