#ifndef CONVERSION_H
#define CONVERSION_H

#include "pixel.h"

class Conversion{
    public:
        ~Conversion() {};

        RGBColor** toRGB (YUVColor** mapa, int lines, int columns);
        YUVColor** toYUV (RGBColor** mapa, int lines, int columns);
    private:

};

#endif
