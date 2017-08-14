#include "bitmap.h"
#include "mapadebits.h"
#include "conversion.h"
#include "image.h"
#include "PIDMath.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>

BMP::BMP(BITMAPFILEHEADER fil, BITMAPINFOHEADER inf, std::vector<Pixel> palette, MBT* i) //https://stackoverflow.com/questions/120876/what-are-the-rules-for-calling-the-superclass-constructor
   : Image(i->getLines(), i->getColumns(), i->getRedBits(), i->getGreenBits(), i->getBlueBits(), i->getMap()),
     file_header(fil), info_header(inf), palette(palette) {

    file_header.Reserved2 = 0;
    file_header.Reserved = 0;
    file_header.Type = 0x4D42;
    if(palette.size() > 0){
        file_header.OffsetBits = 54 + (palette.size()*4);
        file_header.Size = BMP::BMP_file_size(palette.size(), no_lines, no_columns, 8);
        info_header.biSize = 40;
        info_header.biBitCount = 8;
        info_header.biSizeImage= 0;
    } else {
        file_header.OffsetBits = 54;
        info_header.biSize = BMP::BMP_file_size(palette.size(), no_lines, no_columns, 24);
        info_header.biSizeImage = info_header.biSize-54;
    }

    #ifdef LOG_PRINT
        std::cout << "BMP construido com MBT\n";
    #endif // LOG_PRINT
}

BMP::BMP(const BMP& old)
   : Image(old.getLines(), old.getColumns(), old.getRedBits(), old.getGreenBits(), old.getBlueBits(), old.getMap()),
     file_header(old.file_header), info_header(old.info_header), palette(old.palette), isInBGRformat(old.isBGR()){ }

BITMAPFILEHEADER BMP::getFileHeader() const{ return file_header; }
BITMAPINFOHEADER BMP::getInfoHeader() const{ return info_header; }
std::vector<Pixel> BMP::getPalette() const{  return palette; }
bool BMP::isBGR() const{ return isInBGRformat; }

void BMP::readWithPalette(std::ifstream& InputStream){

}

void BMP::readWithoutPalette(std::ifstream& InputStream){

}

int BMP::readFromFile(std::string FileName){
    std::ifstream InputStream(FileName, std::ios::in | std::ios::binary);
    if(!InputStream) return 0;

    BITMAPFILEHEADER BMPHdr;
    BITMAPINFOHEADER BMPInfHdr;
    InputStream.read(reinterpret_cast<char *>(&BMPHdr), sizeof(BITMAPFILEHEADER));
    InputStream.read(reinterpret_cast<char *>(&BMPInfHdr), sizeof(BITMAPINFOHEADER));

    // Verifica se formato da imagem é aceitável (deve ser ‘BM’)
    if(BMPHdr.Type != (('M' << 8) | 'B')) return -1;

    unsigned int lines = BMPInfHdr.biHeight;
    unsigned int cols = BMPInfHdr.biWidth;
    unsigned int lines_and_cols = lines*cols;
    Image::pixelMap.clear();
    Image::pixelMap.reserve(lines_and_cols);

    unsigned int i,j;
    for (i=0; i<lines_and_cols; i++){ //Create "n" "empty" positions
        Image::pixelMap.push_back(Pixel());
    }

    MiscMath mscm; ///Generalizar para diferente de True color
    unsigned int linePixels = cols;
    unsigned int lineBytes = mscm.roundUpToNearestMultiple(linePixels*3, 4);

    RGBTRI lineBuffer[linePixels];
    unsigned int no_spare_bytes = lineBytes - (linePixels*3);
    char spareBytes[no_spare_bytes];

    for(i=0; i<BMPInfHdr.biHeight; i++){ //Read lines
        InputStream.read((char*)&lineBuffer, sizeof(lineBuffer));

        if (no_spare_bytes != 0){ //Read spare bytes
           InputStream.read((char*)&spareBytes, sizeof(spareBytes));
        }
        for (j=0; j<cols; j++){ //Freaking std::reverse(std::begin(lineBuffer), std::end(lineBuffer));
            RGBTRI bgrP = lineBuffer[linePixels-j-1];
            Image::pixelMap[i*cols+j] = Pixel(bgrP.rgbRed, bgrP.rgbGreen, bgrP.rgbBlue);
        }
    }

    file_header = BMPHdr;
    info_header = BMPInfHdr;

    Image::no_lines = info_header.biHeight;
    Image::no_columns = info_header.biWidth;

    Image::red_bits   = 8;
    Image::green_bits = 8;
    Image::blue_bits  = 8;

    /*if (BMPHdr.OffsetBits != 54)
        for (i=0; i<256; i++)
            palette[i] = Pixel(palette_rs[i].rgbRed, palette_rs[i].rgbGreen, palette_rs[i].rgbBlue);*/

    //fromBGRtoRGB();
    InputStream.close();

    #ifdef LOG_PRINT
        std::cout << "BMP True Color lido de arquivo.\n";
    #endif // LOG_PRINT
    return 0;
}

void BMP::writeWithPalette(std::ofstream& Output){

}

void BMP::writeWithoutPalette(std::ofstream& Output){

}

int BMP::writeToFile(std::string FileName){
    if (isBGR()){ swap1stAnd3rdChannels(); }

    std::ofstream Output(FileName,std::ofstream::binary); //Não verifica se ha arquivo já existente
    Output.write(reinterpret_cast<char *>(&file_header), sizeof(BITMAPFILEHEADER));
    Output.write(reinterpret_cast<char *>(&info_header), sizeof(BITMAPINFOHEADER));
    Output.flush();

    unsigned char color_coding[no_lines][no_columns];
    MiscMath m;
    #pragma omp parallel for
    for (unsigned int i=0; i<no_lines; i++){
        for (unsigned int j=0; j<no_columns; j++){
            color_coding[i][j] = m.lookUpPalette(palette, this->getPixel(i,j));
        }
    }

    RGBQUAD palette_rs[palette.size()];
    #pragma omp parallel for
    for (unsigned int i=0; i<palette.size(); i++){
        palette_rs [i] = Image::quad_builder(palette.at(i));
    }
    Output.write(reinterpret_cast<char *>(&palette_rs), sizeof(palette_rs));
    Output.flush();

    MiscMath mscm; ///Generalizar
    unsigned int linePixels = info_header.biWidth;
    unsigned int lineBytes = mscm.roundUpToNearestMultiple(linePixels, 4);
    unsigned char lineBuffer[linePixels];
    unsigned int no_spare_bytes = lineBytes - (linePixels);
    char spareBytes[no_spare_bytes];

    unsigned int i,j;
    //unsigned int cols = info_header.biWidth;
    for(i=0; i<info_header.biHeight; i++){
        for(j=0; j<linePixels; j++){
            lineBuffer[linePixels-j-1] = color_coding[i][j]; //Normal
            //lineBuffer[j] = color_coding[i][j]; //Horizontal flip
            //lineBuffer[linePixels-j-1] = color_coding[no_lines-i-1][j]; // Vertical Flip
            //lineBuffer[j] = color_coding[no_lines-i-1][j]; // Both flip
        }
        Output.write((char*)&lineBuffer, sizeof(lineBuffer));
        if (no_spare_bytes != 0){
           Output.write((char*)&spareBytes, sizeof(spareBytes));
        }

    }
    Output.close();
    #ifdef LOG_PRINT
        std::cout << "BMP 8 bits escrito em arquivo.\n";
    #endif // LOG_PRINT
    return 0;
}

void BMP::swap1stAnd3rdChannels(){
    //stuff
    unsigned int lines = this->Image::getLines();
    unsigned int cols = this->Image::getColumns();

    for (unsigned int i=0; i<lines; i++){
        for (unsigned int j=0; j<cols; j++){
            Image::getPixel(i,j).channelSwap(1,3);
        }
    }

    isInBGRformat = ( isInBGRformat ? false : true); //swap bool value
}

void BMP::changeToBGR(){
    if (isBGR()) return;
    swap1stAnd3rdChannels();
}

unsigned int BMP::BMP_file_size(unsigned int paletteElements, unsigned int lines, unsigned int cols, unsigned char bitsPerColor){
    #define FILE_AND_INFO_HEADERS_SIZE 54
    unsigned int size_in_bytes = FILE_AND_INFO_HEADERS_SIZE;
    if (paletteElements == 0){
        unsigned char bytes_per_color = bitsPerColor/8; //Explosions not byte round
        unsigned int line_bytes = cols*bytes_per_color;
        MiscMath m;
        unsigned int round_lines = m.roundUpToNearestMultiple(line_bytes, 4);
        size_in_bytes += (round_lines) * lines;
    } else {
        size_in_bytes += (paletteElements*4); //RGB QUAD
        unsigned int line_bytes = cols; //Cada cor 1 byte para a unica quantizacao implementada
        MiscMath m;
        unsigned int round_lines = m.roundUpToNearestMultiple(line_bytes, 4);
        size_in_bytes += (round_lines) * lines;
    }
    return size_in_bytes;
}
