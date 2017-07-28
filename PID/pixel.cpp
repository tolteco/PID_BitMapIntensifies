#include "pixel.h"
/*
//destructor
myPoly::~myPoly() {}

//accessors
float myPoly::getWidth() {return width;}
float myPoly::getHeight() {return height;}

void myPoly::setHeight(const float & w) {width = w;}
void myPoly::setWidth(const float & h) {height = h;}*/

void Pixel::set1st  (unsigned char newFirst){
    first = newFirst;
}

void Pixel::set2nd(unsigned char newSecond){
    second = newSecond;
}

void Pixel::set3rd() (unsigned char newThird){
    third = newThird;
}

unsigned char Pixel::get1st(){
    return first;
}

unsigned char Pixel::get2nd(){
    return second;
}

unsigned char Pixel::get3rd(){
    return third;
}
