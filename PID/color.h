#ifndef COLOR_H
#define COLOR_H

class Color{
    public:
        Color(unsigned char red8Bits,
              unsigned char green8Bits,
              unsigned char blue8Bits):
            red(red8Bits), green(green8Bits), blue(blue8Bits){}

        ~Color();

        void setRed  (unsigned char newRed);
        void setGreen(unsigned char newGreen);
        void setBlue (unsigned char newBlue);
        unsigned char getRed();
        unsigned char getGreen();
        unsigned char getBlue();

    private:
        SpatialDistance*  spatialClass;
        TemporalDistance* temporalClass;
        unsigned char red;
        unsigned char green;
        unsigned char blue;
};

#endif
