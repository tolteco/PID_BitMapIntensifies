#ifndef CONVERSION_H
#define CONVERSION_H

#include <vector>
#include "pixel.h"

extern double YUV_to_RGB_matrix[3][3];
extern double RGB_to_YUV_matrix[3][3];

class Conversion{
    public:
        void toRGB (std::vector<Pixel> mapa, unsigned int lines, unsigned int columns);
        void toYUV (std::vector<Pixel> mapa, unsigned int lines, unsigned int columns);
    private:
};

#endif
