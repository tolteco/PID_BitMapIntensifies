#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include "pixel.h"

#define MAX_BM_PRINT 10

///CLASSE PRINCIPAL PARA CALCULOS
class Image{
    public:
        Image() {};
        Image(unsigned int lines, unsigned int columns,
             unsigned char red_bits,
             unsigned char green_bits,
             unsigned char blue_bits,
             Pixel* map_of_pixels);

        //Image(const Image &old); //If define this, follow RULE OF THREE
        //https://stackoverflow.com/questions/4172722/what-is-the-rule-of-three

        //virtual ~Image() = 0;
        //virtual void writeToFile(char* filename) = 0;
        //virtual Image readFromFile(char* filename) = 0;

        ///GETTERS
        Pixel getPixel(unsigned int i, unsigned int j) const;
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

        /// Operators
        Image operator+(const Image& b); //this + b
        Image operator-(const Image& b); //this - b
        Image operator*(const Image& b); //this * b
        Image operator+(const Pixel& constant); //this + constant (Every pixel + constant)
        Image operator-(const Pixel& constant); //this - constant (Every pixel - constant)
        Image operator*(const Pixel& constant); //this * constant (Every pixel * constant)

    friend std::ostream& operator<<(std::ostream &os, Image const &m);

    private:
        unsigned int no_lines;
        unsigned int no_columns;

        unsigned char red_bits;
        unsigned char green_bits;
        unsigned char blue_bits;

        unsigned int pixels_meter_horizontal;
        unsigned int pixels_meter_vertical;

        Pixel* pixelMap;
        ///DEFINE VIRTUAL OPERATOR+ FOR IMAGE SUM IN CHILD CLASSES
};

#endif
