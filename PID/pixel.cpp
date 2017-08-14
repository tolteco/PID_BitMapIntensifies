#include "pixel.h"

///Constructors
Pixel::Pixel(const Pixel &old){ //Copy
    first = old.first;
    second = old.second;
    third = old.third;
}

///Methods

unsigned char Pixel::get1st() const{
    return first;
}

unsigned char Pixel::get2nd() const{
    return second;
}

unsigned char Pixel::get3rd() const{
    return third;
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
