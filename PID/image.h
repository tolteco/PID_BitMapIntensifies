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

#define MAX_BM_PRINT 10 //Se linhas ou colunas for maior que isso, o operador << nao retorna stream com print

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

        //Image(const Image &old); //If define this, follow RULE OF THREE
        //https://stackoverflow.com/questions/4172722/what-is-the-rule-of-three

        ~Image();

        ///GETTERS
        Pixel getPixel(unsigned int i, unsigned int j) const;
        unsigned int getLines() const;
        unsigned int getColumns() const;
        unsigned char getBitsPerColor() const;
        unsigned char getRedBits() const;
        unsigned char getGreenBits() const;
        unsigned char getBlueBits() const;
        unsigned int getHorizontalResolution() const;
        unsigned int getVerticalResolution() const;
        std::vector<Pixel> getMap() const; ///I know. Ugly access and stuff.

        ///SETTERS
        void setHorizontalResolution(unsigned int);
        void setVerticalResolution(unsigned int);
        void setPixel(Pixel p, unsigned int i, unsigned int j);

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

        std::vector<Pixel> pixelMap;
};

class PersistableIMG : public Image {
    public:
        virtual int writeToFile (std::string FileName) = 0;
        virtual int readFromFile(std::string FileName) = 0;
    protected:
        PersistableIMG() {};
        PersistableIMG(unsigned int lines, unsigned int columns,
             unsigned char red_bits,
             unsigned char green_bits,
             unsigned char blue_bits,
             std::vector<Pixel> map_of_pixels); //Só chamável por subclasses. Serve como modelo Bridge
};

class MBT;
//REF. NOTAS DE AULA DE PID :D
class BMP : public PersistableIMG {
    public:
        BMP() {};
        BMP(const BMP &old);
        BMP(BITMAPFILEHEADER fil, BITMAPINFOHEADER inf, std::vector<Pixel> palette, MBT* i);
        enum BiCompress_E {
            BI_RGB  = 0, //Sem compressao
            BI_RLE8 = 1, //RLE 8 bits
            BI_RLE4 = 2 //RLE 4 bits
        };

        BITMAPFILEHEADER getFileHeader() const;
        BITMAPINFOHEADER getInfoHeader() const;
        std::vector<Pixel> getPalette() const;
        bool isBGR() const;

        int readFromFile(std::string FileName) override;
        int writeToFile (std::string FileName) override;
        void changeToBGR();

    static unsigned int BMP_file_size(unsigned int paletteElements, unsigned int lines, unsigned int cols, unsigned char bitsPerColor);

    friend std::ostream& operator<<(std::ostream &os, BMP const &m);

    private:
        BITMAPFILEHEADER file_header;
        BITMAPINFOHEADER info_header;
        std::vector<Pixel> palette;
        bool isInBGRformat = false;

        void swap1stAnd3rdChannels();  //BMP armazenados em BGR
        void writeWithPalette(std::ofstream& Output);
        void writeWithoutPalette(std::ofstream& Output);
        void readWithPalette(std::ifstream& InputStream);
        void readWithoutPalette(std::ifstream& InputStream);
};

//Formato próprio - Mapa de BiTs - MBT
class MBT : public PersistableIMG {
    public:
        typedef enum tagCOLORSPACE {
            YUV = 0, //Armazenado em YUV
            RGB = 1, //Armazenado em RGB
            HSV = 2
        }ColorSpace;

        MBT() {};
        MBT(const BMP& old);

        BITMAPFILEHEADER getFileHeader() const;
        BITMAPINFOHEADER getInfoHeader() const;
        std::vector<Pixel> getPalette()  const;
        void setPalette(std::vector<Pixel> pal);
        unsigned short getBitsPerColor () const;
        unsigned char get1stChannelBits() const;
        unsigned char get2ndChannelBits() const;
        unsigned char get3rdChannelBits() const;
        unsigned char get4thChannelBits() const;
        ColorSpace getColorSpace() const;

        int readFromFile(std::string FileName) override;
        int writeToFile (std::string FileName) override;
        void changeColorSpace(ColorSpace color_space);
        BMP constructBMP();

    private:
        BITMAPFILEHEADER file_header;
        BITMAPINFOHEADER info_header;
        std::vector<Pixel> palette;
        ColorSpace color = RGB;

        void writeWithPalette(std::ofstream& Output);
        void writeWithoutPalette(std::ofstream& Output);
        void readWithPalette(std::ifstream& InputStream);
        void readWithoutPalette(std::ifstream& InputStream);

        /*
        Campos do FileHeader (BMP) com diferentes propositos:

        -- BfType = "MB"
        -- BfSize = ColorSpace (0, RGB; 1, YUV; 2, HSV)
        -- BfReser1 = Byte mais alto para bits do primeiro canal, depois do segundo canal;
        -- BfReser2 = Byte mais alto para bits do terceiro canal, depois do quarto (Se não houver, 0);
        -- BiSize = Tamanho da paleta; 0 se inexistente
        */
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
