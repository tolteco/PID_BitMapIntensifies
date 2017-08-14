#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>

///Explosion warning:
/*  Usar os typedefs do header
"windows.h" faz com que as estruturas
do BMP (Com o mesmo nome que as abaixo)
também sejam incluídas. Possíveis
problemas de desalinhamento.
*/

#define LOG_PRINT 1

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned long QWORD;

#pragma pack(push, 1)
typedef struct tagBITMAPFILEHEADER {
    WORD  Type;
    DWORD Size;
    WORD  Reserved;
    WORD  Reserved2;
    DWORD OffsetBits;
}BITMAPFILEHEADER;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct tagRGBQUAD {
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
}RGBQUAD;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct tagRGBTRI {
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
}RGBTRI;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct tagBITMAPINFOHEADER {
    DWORD biSize;
    DWORD biWidth;
    DWORD biHeight;
    WORD  biPlanes;
    WORD  biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    DWORD biXPelsPerMeter;
    DWORD biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
}BITMAPINFOHEADER;
#pragma pack(pop)

#include <iostream>
#include "pixel.h"

///CLASSE PRINCIPAL PARA CALCULOS
class Image{
    public:
        Image() {};
        Image(unsigned int lines, unsigned int columns,
             unsigned char red_bits,
             unsigned char green_bits,
             unsigned char blue_bits,
             std::vector<Pixel> map_of_pixels);

        Image(const Image &old);

        virtual int writeToFile (std::string FileName) = 0;
        virtual int readFromFile(std::string FileName) = 0;
        //Image(const Image &old); //If define this, follow RULE OF THREE
        //https://stackoverflow.com/questions/4172722/what-is-the-rule-of-three

        ~Image();

        ///GETTERS
        Pixel getPixel(unsigned int i, unsigned int j) const;
        unsigned int  getLines()     const;
        unsigned int  getColumns()   const;
        unsigned char getRedBits()   const;
        unsigned char getGreenBits() const;
        unsigned char getBlueBits()  const;
        std::vector<Pixel> getMap()  const; ///I know. Ugly access and stuff.

        ///SETTERS
        void setPixel(Pixel p, unsigned int i, unsigned int j);

        static RGBQUAD quad_builder(const Pixel& p);
        static RGBTRI  tri_builder (const Pixel& p);
    protected:
        unsigned int no_lines;
        unsigned int no_columns;
        unsigned char red_bits;
        unsigned char green_bits;
        unsigned char blue_bits;
        std::vector<Pixel> pixelMap;
};


/*#ifdef HORIZONTAL_FLIP
    //int
#elifdef VERTICAL_FLIP
    //int
#elifdef BOTH_FLIP
    //int
#else
    //int
#endif // HORIZONTAL_FLIP*/


#endif
