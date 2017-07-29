#include "pixel.h"
/*
//destructor
myPoly::~myPoly() {}

//accessors
float myPoly::getWidth() {return width;}
float myPoly::getHeight() {return height;}

void myPoly::setHeight(const float & w) {width = w;}
void myPoly::setWidth(const float & h) {height = h;}*/

///Constructors
Pixel::Pixel(const Pixel &old){ //Copy
    first = old.first;
    second = old.second;
    third = old.third;
}

///Methods
void Pixel::set1st  (unsigned char newFirst){
    first = newFirst;
}

void Pixel::set2nd(unsigned char newSecond){
    second = newSecond;
}

void Pixel::set3rd (unsigned char newThird){
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

///Operators
std::ostream &operator<<(std::ostream &os, Pixel const &m) {
    return os << '(' <<
    m.first   << ',' <<
    m.second  << ',' <<
    m.third   << ')' ;
}

// ++Pixel
Pixel& Pixel::operator++(){
    if(this->first  < 255) ++this->first;
    if(this->second < 255) ++this->second;
    if(this->third  < 255) ++this->third;
    return *this;
}

// Pixel++
Pixel Pixel::operator++(int){
    Pixel newP = Pixel(*this);
    operator++();
    return newP;
}

// --Pixel
Pixel& Pixel::operator--(){
    if(this->first  > 0) --this->first;
    if(this->second > 0) --this->second;
    if(this->third  > 0) --this->third;
    return *this;
}

// Pixel--
Pixel Pixel::operator--(int){
    Pixel newP = Pixel(*this);
    operator--();
    return newP;
}

//this + b
Pixel Pixel::operator+(const Pixel& b) {
     Pixel px;
     unsigned short f = this->first + b.first;
     if (f < 255) px.first = (unsigned char) f;
     else         px.first = 255;

     f = this->second + b.second;
     if (f < 255) px.second = (unsigned char) f;
     else         px.second = 255;

     f = this->third + b.third;
     if (f < 255) px.third = (unsigned char) f;
     else         px.third = 255;

     return px;
}

//this - b
Pixel Pixel::operator-(const Pixel& b) {
     Pixel px;
     short f = this->first - b.first;
     if (f > 0) px.first = (unsigned char) f;
     else       px.first = 0;

     f = this->second - b.second;
     if (f > 0) px.second = (unsigned char) f;
     else       px.second = 0;

     f = this->third - b.third;
     if (f > 0) px.third = (unsigned char) f;
     else       px.third = 0;

     return px;
}

Pixel Pixel::operator*(const Pixel& b) {
     Pixel px;
     unsigned short f = this->first * b.first;
     if (f < 255) px.first = (unsigned char) f;
     else         px.first = 255;

     f = this->second * b.second;
     if (f < 255) px.second = (unsigned char) f;
     else         px.second = 255;

     f = this->third * b.third;
     if (f < 255) px.third = (unsigned char) f;
     else         px.third = 255;

     return px;
}
