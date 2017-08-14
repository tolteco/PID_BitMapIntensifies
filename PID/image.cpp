#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include "conversion.h"
#include "image.h"
#include "PIDMath.h"

//destructor
Image::~Image() {
    std::vector<Pixel>().swap(pixelMap); //https://stackoverflow.com/questions/993590/should-i-delete-vectorstring
}

Image::Image(unsigned int lines, unsigned int columns,
     unsigned char red_bits,
     unsigned char green_bits,
     unsigned char blue_bits,
     std::vector<Pixel> map_of_pixels)
     : no_lines(lines), no_columns(columns),
       red_bits(red_bits), green_bits(green_bits),
       blue_bits(blue_bits),
       pixelMap(map_of_pixels){ }

Image::Image(const Image &old)
     : no_lines(old.no_lines), no_columns(old.no_lines),
       red_bits(old.red_bits), green_bits(old.green_bits),
       blue_bits(old.blue_bits),
       pixelMap(old.pixelMap){ }

///GETTERS
Pixel         Image::getPixel(unsigned int i, unsigned int j) const{ return pixelMap[i*this->no_columns+j]; } //return Pixel(pixelMap[i*this->no_columns+j]); }
unsigned int  Image::getLines() const{ return no_lines; }
unsigned int  Image::getColumns() const{ return no_columns; }
unsigned char Image::getRedBits() const{ return red_bits; }
unsigned char Image::getGreenBits() const{ return green_bits; }
unsigned char Image::getBlueBits() const{ return blue_bits; }
std::vector<Pixel> Image::getMap() const{ return pixelMap; }

///SETTERS
void Image::setPixel(Pixel p, unsigned int i, unsigned int j){ pixelMap[i*this->no_columns+j] = p; }

RGBQUAD Image::quad_builder(const Pixel& p){
    RGBQUAD quad;
    quad.rgbRed = p.get1st();
    quad.rgbGreen = p.get2nd();
    quad.rgbBlue = p.get3rd();
    quad.rgbReserved = 0;
    return quad;
}

RGBTRI Image::tri_builder (const Pixel& p){
    RGBTRI tri;
    tri.rgbBlue  = p.get1st();
    tri.rgbGreen = p.get2nd();
    tri.rgbRed   = p.get3rd();
    return tri;
}
