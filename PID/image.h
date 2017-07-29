#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include "pixel.h"

#define MAX_BM_PRINT 10

///CLASSE PRINCIPAL PARA CALCULOS
class Image{
    public:
        Image() {};
        Image(unsigned int lines, unsigned int columns,
             unsigned char red_bits,
             unsigned char green_bits,
             unsigned char blue_bits,
             Pixel* map_of_pixels);

        //Image(const Image &old); //If define this, follow RULE OF THREE
        //https://stackoverflow.com/questions/4172722/what-is-the-rule-of-three

        //virtual ~Image() = 0;
        //virtual void writeToFile(char* filename) = 0;
        //virtual Image readFromFile(char* filename) = 0;

        ///GETTERS
        Pixel getPixel(unsigned int i, unsigned int j) const;
        unsigned int getLines();
        unsigned int getColumns();
        unsigned char getBitsPerColor();
        unsigned char getRedBits();
        unsigned char getGreenBits();
        unsigned char getBlueBits();
        unsigned int getHorizontalResolution();
        unsigned int getVerticalResolution();

        ///SETTERS
        void setHorizontalResolution(unsigned int);
        void setVerticalResolution(unsigned int);

        /// Operators
        Image operator+(const Image& b); //this + b
        Image operator-(const Image& b); //this - b
        Image operator*(const Image& b); //this * b
        Image operator+(const Pixel& constant); //this + constant (Every pixel + constant)
        Image operator-(const Pixel& constant); //this - constant (Every pixel - constant)
        Image operator*(const Pixel& constant); //this * constant (Every pixel * constant)

    friend std::ostream& operator<<(std::ostream &os, Image const &m);

    private:
        unsigned int no_lines;
        unsigned int no_columns;

        unsigned char red_bits;
        unsigned char green_bits;
        unsigned char blue_bits;

        unsigned int pixels_meter_horizontal;
        unsigned int pixels_meter_vertical;

        Pixel* pixelMap;
        ///DEFINE VIRTUAL OPERATOR+ FOR IMAGE SUM IN CHILD CLASSES
};

//REF. NOTAS DE AULA DE PID :D
class BMP : Image {
    public:
        enum BiCompress_E {
            BI_RGB  = 0; //Sem compressao
            BI_RLE8 = 1; //RLE 8 bits
            BI_RLE4 = 2; //RLE 4 bits
        }

    private:
        BITMAPFILEHEADER file_header;
        BITMAPINFO info;
        BITMAPINFOHEADER info_header;
        BYTE* reserved_map;


};

//Formato próprio - Mapa de BiTs - MBT
class MBT : Image {
    public:
        //https://stackoverflow.com/questions/8771881/setting-multiple-attributes-for-enum-structure
        enum Reserved_FH {
            YUV = 0; //Armazenado em YUV
            RGB = 1; //Armazenado em RGB
        }

        ///Campo reservado2 do BMP pode ser usado para
        ///quando tempos num de bits diferentes entre
        ///canais, onde cada byte (dos 3 mais significativos)
        ///indica o tanto de bits por cor. Ex.: 5-6-5
        ///Reservado2 0101 0110 0101 0000
        ///             5    6    5        (RGB - 1st, 2nd, 3rd)

    private:

};

typedef struct tagBITMAPFILEHEADER {
    WORD Type;
    DWORD Size;
    WORD Reserved;
    WORD Reserved2;
    DWORD OffsetBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFO {
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD bmiColors[1];
} BITMAPINFO;

typedef struct tagBITMAPINFOHEADER {
    DWORD biSize;
    DWORD biWidth;
    DWORD biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    DWORD biXPelsPerMeter;
    DWORD biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagRGBQUAD {
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
} RGBQUAD;

#endif
