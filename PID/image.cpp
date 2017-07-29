#include <iostream>
#include <string>
#include <algorithm>
#include "image.h"

//destructor
//Image::~Image() {}

Image::Image(unsigned int lines, unsigned int columns,
     unsigned char red_bits,
     unsigned char green_bits,
     unsigned char blue_bits,
     Pixel* map_of_pixels)
     : no_lines(lines), no_columns(columns),
       red_bits(red_bits), green_bits(green_bits),
       blue_bits(blue_bits){

    pixelMap  = (Pixel*)_mm_malloc(sizeof(Pixel)*lines*columns, 64);
    std::copy(map_of_pixels, map_of_pixels+(lines*columns), pixelMap);
}

///GETTERS

Pixel Image::getPixel(unsigned int i, unsigned int j) const{
    return Pixel(pixelMap[i*this->no_columns+j]);
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

std::ostream &operator<<(std::ostream &os, Image const &m) {
    unsigned int cols = m.no_columns;
    for (unsigned int i=0; i<m.no_lines; i++){
        for (unsigned int j=0; j<cols; j++){
            os << m.getPixel(i,j) << ',';
            //mat[i]l[j]
        }
        os << std::endl;
    }
    return os;
}

//Maybe we could use this as a wrapper to
//a generic function with a functor that operates
//between pixels? Nah. (Juliano).
Image Image::operator+(const Image& b) {
    /* E se as imagens forem de tamanhos
    diferentes? A imagem LHS nunca terá
    suas dimensões alteradas. Ex.:
    Img A 255 255  ImgB 240 230 100
           10  10        20  10   5

    Img Resultado 15 25
                   0  0

    O operador sub não dá "wrap" nos
    valores negativos por conta do operador
    + definido para os pixels.
    */

    Image px = Image(*this);
    unsigned int cols = this->no_columns;
    for (unsigned int i=0; i<this->no_lines; i++){
        for (unsigned int j=0; j<cols; j++){
            if( (i<b.no_lines) && (j<b.no_columns) ){
                px.pixelMap[i*cols+j] = this->getPixel(i,j) + b.getPixel(i,j);
            }
        }
    }

    return px;
}

Image Image::operator-(const Image& b) {
    Image px = Image(*this);
    unsigned int cols = this->no_columns;
    for (unsigned int i=0; i<this->no_lines; i++){
        for (unsigned int j=0; j<cols; j++){
            if( (i<b.no_lines) && (j<b.no_columns) ){
                px.pixelMap[i*cols+j] = this->getPixel(i,j) - b.getPixel(i,j);
            }
        }
    }

    return px;
}

Image Image::operator*(const Image& b) {
    Image px = Image(*this);
    unsigned int cols = this->no_columns;
    for (unsigned int i=0; i<this->no_lines; i++){
        for (unsigned int j=0; j<cols; j++){
            if( (i<b.no_lines) && (j<b.no_columns) ){
                px.pixelMap[i*cols+j] = this->getPixel(i,j) * b.getPixel(i,j);
            }
        }
    }

    return px;
}

Image Image::operator+(const Pixel& constant) {
    Image px = Image(*this);
    unsigned int cols = this->no_columns;
    for (unsigned int i=0; i<this->no_lines; i++){
        for (unsigned int j=0; j<cols; j++){
            px.pixelMap[i*cols+j] = this->getPixel(i,j) + constant;
        }
    }

    return px;
}

Image Image::operator-(const Pixel& constant) {
    Image px = Image(*this);
    unsigned int cols = this->no_columns;
    for (unsigned int i=0; i<this->no_lines; i++){
        for (unsigned int j=0; j<cols; j++){
            px.pixelMap[i*cols+j] = this->getPixel(i,j) - constant;
        }
    }

    return px;
}

Image Image::operator*(const Pixel& constant) {
    Image px = Image(*this);
    unsigned int cols = this->no_columns;
    for (unsigned int i=0; i<this->no_lines; i++){
        for (unsigned int j=0; j<cols; j++){
            px.pixelMap[i*cols+j] = this->getPixel(i,j) * constant;
        }
    }

    return px;
}
