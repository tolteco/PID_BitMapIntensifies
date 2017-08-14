#ifndef BITMAP_H
#define BITMAP_H

#include "image.h"
#include "mapadebits.h"

class MBT;

class BMP : public Image {
    public:
        BMP() {};
        BMP(const BMP &old);
        BMP(BITMAPFILEHEADER fil, BITMAPINFOHEADER inf, std::vector<Pixel> palette, MBT* i);

        BITMAPFILEHEADER getFileHeader() const;
        BITMAPINFOHEADER getInfoHeader() const;
        std::vector<Pixel> getPalette() const;
        bool isBGR() const;

        int readFromFile(std::string FileName) override;
        int writeToFile (std::string FileName) override;

    static unsigned int BMP_file_size(unsigned int paletteElements, unsigned int lines, unsigned int cols, unsigned char bitsPerColor);

    private:
        BITMAPFILEHEADER file_header;
        BITMAPINFOHEADER info_header;
        std::vector<Pixel> palette;
        bool isInBGRformat = false;

        void changeToBGR();
        void swap1stAnd3rdChannels();  //BMP armazenados em BGR
        void writeWithPalette(std::ofstream& Output);
        void writeWithoutPalette(std::ofstream& Output);
        void readWithPalette(std::ifstream& InputStream);
        void readWithoutPalette(std::ifstream& InputStream);
};

#endif
