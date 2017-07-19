#include "color.h"

//destructor
myPoly::~myPoly() {}

//accessors
float myPoly::getWidth() {return width;}
float myPoly::getHeight() {return height;}

void myPoly::setHeight(const float & w) {width = w;}
void myPoly::setWidth(const float & h) {height = h;}

void Color::setRed  (unsigned char newRed){
    red = newRed;
}

void Color::setGreen(unsigned char newGreen){
    green = newGreen;
}

void Color::setBlue (unsigned char newBlue){
    blue = newBlue;
}

unsigned char Color::getRed(){
    return red;
}

unsigned char Color::getGreen(){
    return green;
}

unsigned char Color::getBlue(){
    return blue;
}
