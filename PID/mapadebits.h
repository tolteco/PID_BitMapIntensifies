#ifndef MAPADEBITS_H
#define MAPADEBITS_H

#include "image.h"
#include "bitmap.h"

class BMP;
//Formato próprio - Mapa de BiTs - MBT
class MBT : public Image {
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

#endif
