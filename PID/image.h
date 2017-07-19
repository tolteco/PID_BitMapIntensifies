#ifndef IMAGE_H
#define IMAGE_H

///CLASSE PRINCIPAL PARA CALCULOS
class Image{
    public:
        Image();

        virtual ~Image();


    private:
        unsigned int no_lines;
        unsigned int no_columns;
        unsigned char bits_per_color;
        unsigned char red_bits;
        unsigned char green_bits;
        unsigned char blue_bits;
};

#endif
