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
        void multiply(double mult[3][3], Pixel* mat, const unsigned int columns);
        void multiplySum(double mult[3][3], double sum[3], Pixel* mat, const unsigned int columns);
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
