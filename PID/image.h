#ifndef IMAGE_H
#define IMAGE_H

#include "pixel.h"

///CLASSE PRINCIPAL PARA CALCULOS
class Image{
    public:
        Image(Pixel** map_of_pixels,
              unsigned int lines, unsigned int columns,
              unsigned char red_bits,
              unsigned char green_bits,
              unsigned char blue_bits);

        virtual ~Image();
        virtual void writeToFile(char* filename);
        virtual Image readFromFile(char* filename);

        ///GETTERS
        Pixel getPixel(unsigned int i, unsigned int j);

        unsigned int getLines();
        unsigned int getColumns();

        unsigned char getBitsPerColor();
        unsigned char getRedBits();
        unsigned char getGreenBits();
        unsigned char getBlueBits();

        unsigned int getHorizontalResolution();
        unsigned int getVerticalResolution();

        ///SETTERS

        void setHorizontalResolution(unsigned int);
        void setVerticalResolution(unsigned int);

    private:
        unsigned int no_lines;
        unsigned int no_columns;

        unsigned char red_bits;
        unsigned char green_bits;
        unsigned char blue_bits;

        unsigned int pixels_meter_horizontal;
        unsigned int pixels_meter_vertical;

        Pixel** pixelMap;

        ///DEFINE VIRTUAL OPERATOR+ FOR IMAGE SUM IN CHILD CLASSES
};

#endif
