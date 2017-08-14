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

        unsigned char get1st() const;
        unsigned char get2nd() const;
        unsigned char get3rd() const;

        bool  operator==(const Pixel& b); //this == b

        //NAO deve ser feito sobrecarda dos operadores "<", ">", etc;
        //por meio de operadores, nao se sabe que canal comparar.
        //Use as funções "moreThan", "lessThan", ou similar para tal
        //bool   operator<(const Pixel& lhs, const Pixel& rhs);

        void channelSwap(unsigned char chnl1, unsigned char chnl2);
        bool moreThan(Pixel b, char channel);
        bool lessThan(Pixel b, char channel);
    private:
        unsigned char first;
        unsigned char second;
        unsigned char third;
};

#endif
