#include "image.h"

Image::Image(Pixel** map_of_pixels,
             unsigned int lines, unsigned int columns,
             unsigned char red_bits,
             unsigned char green_bits,
             unsigned char blue_bits)
             : pixelMap(map_of_pixels),
               no_lines(lines), no_columns(columns),
               red_bits(red_bits), green_bits(green_bits),
               blue_bits(blue_bits) {}

//destructor
Image::~Image() {}

///GETTERS

Pixel Image::getPixel(unsigned int i, unsigned int j){
    return pixelMap[i][j];
}

unsigned int Image::getLines(){
    return no_lines;
}

unsigned int Image::getColumns(){
    return no_columns;
}

unsigned char Image::getBitsPerColor(){
    return red_bits + green_bits + blue_bits;
}

unsigned char Image::getRedBits(){
    return red_bits;
}

unsigned char Image::getGreenBits(){
    return green_bits;
}

unsigned char Image::getBlueBits(){
    return blue_bits;
}

unsigned int Image::getHorizontalResolution(){
    return pixels_meter_horizontal;
}

unsigned int Image::getVerticalResolution(){
    return pixels_meter_vertical;
}

///SETTERS
void Image::setHorizontalResolution(unsigned int pixels_per_meter){
    pixels_meter_horizontal = pixels_per_meter;
}

void Image::setVerticalResolution(unsigned int pixels_per_meter){
    pixels_meter_vertical = pixels_per_meter;
}
