#ifndef COLOR_H
#define COLOR_H

class Color{
    public:
        virtual ~Color() {};

        ///SET CHANNELS
        virtual void set1st (unsigned char new1st) = 0;
        virtual void set2nd (unsigned char new2nd) = 0;
        virtual void set3rd (unsigned char new3rd) = 0;
        virtual unsigned char get1st() = 0;
        virtual unsigned char get2nd() = 0;
        virtual unsigned char get3rd() = 0;

    private:
        unsigned char first;
        unsigned char second;
        unsigned char third;
};

class RGBColor : public Color{

};

class YUVColor : public Color{

};

#endif
