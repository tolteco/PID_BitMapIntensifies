#ifndef PIXEL_H
#define PIXEL_H

#include <iostream>

class Pixel{
    public:
        Pixel() {};
        Pixel(unsigned char first8,
              unsigned char second8,
              unsigned char third8):
            first(first8), second(second8), third(third8){}

        Pixel(const Pixel &old);
        virtual ~Pixel() {};

        void set1st (unsigned char new1st);
        void set2nd (unsigned char new2nd);
        void set3rd (unsigned char new3rd);
        unsigned char get1st();
        unsigned char get2nd();
        unsigned char get3rd();

        /// Operators
        Pixel& operator++(); //++Pixel
        Pixel  operator++(int); //Pixel++
        Pixel& operator--(); //--Pixel
        Pixel  operator--(int); //Pixel--
        Pixel  operator+(const Pixel& b); //this + b
        Pixel  operator-(const Pixel& b); //this - b
        //bool   operator<(const Pixel& lhs, const Pixel& rhs);

    friend std::ostream& operator<<(std::ostream &os, Pixel const &m);

    private:
        unsigned char first;
        unsigned char second;
        unsigned char third;
};

class RGBColor : public Pixel{

};

class YUVColor : public Pixel{

};

#endif
