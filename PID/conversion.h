#ifndef CONVERSION_H
#define CONVERSION_H

#include <vector>
#include "pixel.h"

class Conversion{
    public:
        void toRGB (std::vector<Pixel> mapa, unsigned int lines, unsigned int columns);
        void toRGB (std::vector<Pixel> pal,  unsigned int elements);
        void toYUV (std::vector<Pixel> mapa, unsigned int lines, unsigned int columns);
        void toYUV (std::vector<Pixel> pal,  unsigned int elements);
    private:
};

#endif
