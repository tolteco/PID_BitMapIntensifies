#ifndef PIDMATH_H
#define PIDMATH_H

#include <vector>
#include "pixel.h"
#include "image.h"

#define BLOCK_SIZE 16
#define ROUND_UP(x, s) (((x)+((s)-1)) & -(s))

class MatrixOperation{
    public:
        ~MatrixOperation() {};

        void transposeMatrix (Pixel* src, Pixel* dst, const unsigned int lines, const unsigned int columns);
        void multiply(Pixel* mat, const unsigned int lines,     const unsigned int columns,
                    double* mult, const unsigned int multLines, const unsigned int multColumns);
    private:

};

class MiscMath{
public:
    unsigned int roundUpToNearestMultiple(unsigned int num, unsigned int multiple);
};

class Quantization{
public:
    std::vector<Pixel> medianCut2ndAnd3rdChannels(Image* img, unsigned short maxColors);
};

#endif
