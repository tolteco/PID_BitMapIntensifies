#ifndef CONVERSION_H
#define CONVERSION_H

#include "pixel.h"

extern double YUV_to_RGB_matrix[3][3];
extern double RGB_to_YUV_matrix[3][3];

class Conversion{
    public:
        void toRGB (Pixel* mapa, int lines, int columns);
        void toYUV (Pixel* mapa, int lines, int columns);
    private:
};

#endif
