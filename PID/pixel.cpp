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

bool Pixel::operator==(const Pixel& b){
    if(
        this->first  == b.first &&
        this->second == b.second &&
        this->third  == b.third
      )    { return true; }
      else { return false;}
}

bool Pixel::moreThan(Pixel b, char channel){
    unsigned char a_value, b_value;
    if (channel == 0){
        a_value = this->get1st();
        b_value = b.get1st();
    } else if (channel == 1){
        a_value = this->get2nd();
        b_value = b.get2nd();
    } else {
        a_value = this->get3rd();
        b_value = b.get3rd();
    }

    if (a_value > b_value) return true;
    else return false;
}

bool Pixel::lessThan(Pixel b, char channel){
    unsigned char a_value, b_value;
    if (channel == 0){
        a_value = this->get1st();
        b_value = b.get1st();
    } else if (channel == 1){
        a_value = this->get2nd();
        b_value = b.get2nd();
    } else {
        a_value = this->get3rd();
        b_value = b.get3rd();
    }

    if (a_value < b_value) return true;
    else return false;
}

bool Pixel::moreThan(Pixel a, Pixel b, char channel){
    unsigned char a_value, b_value;
    if (channel == 0){
        a_value = a.get1st();
        b_value = b.get1st();
    } else if (channel == 1){
        a_value = a.get2nd();
        b_value = b.get2nd();
    } else {
        a_value = a.get3rd();
        b_value = b.get3rd();
    }

    if (a_value > b_value) return true;
    else return false;
}

bool Pixel::lessThan(Pixel a, Pixel b, char channel){
    unsigned char a_value, b_value;
    if (channel == 0){
        a_value = a.get1st();
        b_value = b.get1st();
    } else if (channel == 1){
        a_value = a.get2nd();
        b_value = b.get2nd();
    } else {
        a_value = a.get3rd();
        b_value = b.get3rd();
    }

    if (a_value < b_value) return true;
    else return false;
}

/*void swapUCHAR(unsigned char *a, unsigned char *b){
    unsigned char temp;
    temp = *a;
    *a = *b;
    *b = temp;
} */

void Pixel::channelSwap(unsigned char chnl1, unsigned char chnl2){
    if(chnl1 == chnl2) return;

    unsigned char chnl1_bits, chnl2_bits;

    if      (chnl1 == 0) chnl1_bits = this->first;
    else if (chnl1 == 1) chnl1_bits = this->second;
    else                 chnl1_bits = this->third;

    if      (chnl2 == 0) chnl2_bits = this->first;
    else if (chnl2 == 1) chnl2_bits = this->second;
    else                 chnl2_bits = this->third;

    std::swap(chnl1_bits, chnl2_bits);

    if      (chnl1 == 0) this->first  = chnl1_bits;
    else if (chnl1 == 1) this->second = chnl1_bits;
    else                 this->third  = chnl1_bits;

    if      (chnl2 == 0) this->first  = chnl2_bits;
    else if (chnl2 == 1) this->second = chnl2_bits;
    else                 this->third  = chnl2_bits;
}
