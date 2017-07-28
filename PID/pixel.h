#ifndef PIXEL_H
#define PIXEL_H

class Pixel{
    public:
        Pixel(unsigned char first8,
              unsigned char second8,
              unsigned char third8Bits):
            first(red8Bits), second(green8Bits), third(blue8Bits){}

        virtual ~Pixel() {};

        ///SET CHANNELS
        void set1st (unsigned char new1st);
        void set2nd (unsigned char new2nd);
        void set3rd (unsigned char new3rd);
        unsigned char get1st();
        unsigned char get2nd();
        unsigned char get3rd();

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
