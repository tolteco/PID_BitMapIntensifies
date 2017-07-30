#ifndef IMAGE_H
#define IMAGE_H

///Explosion warning:
/*  Usar os typedefs do header
"windows.h" faz com que as estruturas
do BMP (Com o mesmo nome que as abaixo)
também sejam incluídas. Possíveis
problemas de desalinhamento.
*/

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

typedef struct tagBITMAPINFO {
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD bmiColors[1];
}__attribute__((packed)) BITMAPINFO;

#include <iostream>
#include "pixel.h"

#define MAX_BM_PRINT 10 //Se linhas ou colunas for maior que isso, o operador << nao retorna stream com print

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

    protected:
        unsigned int no_lines;
        unsigned int no_columns;

        unsigned char red_bits;
        unsigned char green_bits;
        unsigned char blue_bits;

        unsigned int pixels_meter_horizontal;
        unsigned int pixels_meter_vertical;

        Pixel* pixelMap;
};

class PersistableIMG : public Image {
    public:
        virtual int writeToFile (char* FileName) = 0;
        virtual int readFromFile(char* FileName) = 0;
};

//REF. NOTAS DE AULA DE PID :D
class BMP : public PersistableIMG {
    public:
        enum BiCompress_E {
            BI_RGB  = 0, //Sem compressao
            BI_RLE8 = 1, //RLE 8 bits
            BI_RLE4 = 2 //RLE 4 bits
        };

        int readFromFile(char* FileName);
        int writeToFile (char* FileName);

    friend std::ostream& operator<<(std::ostream &os, BMP const &m);

    private:
        BITMAPFILEHEADER file_header;
        BITMAPINFO info;
        BITMAPINFOHEADER info_header;
        Pixel palette[256];
        bool isBGR = true;

        void fromBGRtoRGB();  //BMP armazenados em BGR
        void fromRGBtoBGR();
        //void reorderBMPcolumns(); //Armazena-se primeiro a ultima coluna da primeira linha
};

//Formato próprio - Mapa de BiTs - MBT
class MBT : public PersistableIMG {
    public:
        //https://stackoverflow.com/questions/8771881/setting-multiple-attributes-for-enum-structure
        enum Reserved_FH {
            YUV = 0, //Armazenado em YUV
            RGB = 1  //Armazenado em RGB
        };

        ///Campo reservado2 do BMP pode ser usado para
        ///quando tempos num de bits diferentes entre
        ///canais, onde cada byte (dos 3 mais significativos)
        ///indica o tanto de bits por cor. Ex.: 5-6-5
        ///Reservado2 0101 0110 0101 0000
        ///             5    6    5        (RGB - 1st, 2nd, 3rd)

    private:

};



#endif
