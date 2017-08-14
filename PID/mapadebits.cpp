#include "bitmap.h"
#include "mapadebits.h"
#include "conversion.h"
#include "image.h"
#include "PIDMath.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>

MBT::MBT(const BMP& old)
   : Image(old.getLines(), old.getColumns(), old.getRedBits(), old.getGreenBits(), old.getBlueBits(), old.getMap()),
     file_header(old.getFileHeader()), info_header(old.getInfoHeader()), palette(old.getPalette()){

    #ifdef LOG_PRINT
        std::cout << "MBT construido de BMP.\n";
    #endif // LOG_PRINT

    color = MBT::ColorSpace::RGB;
}

BMP MBT::constructBMP(){
    if (color == MBT::ColorSpace::YUV){
        MBT::changeColorSpace(MBT::ColorSpace::RGB);
    }

    return BMP(this->file_header, this->info_header, this->palette, this);
}

void MBT::changeColorSpace(MBT::ColorSpace color_space){
    if (color_space == color) return; //Nao precisa converter para o mesmo

    Conversion c;
    //So converte se foi definido
    if (color_space == YUV){
        c.toYUV(pixelMap, no_lines, no_columns);
        c.toYUV(palette, palette.size());
        color = MBT::ColorSpace::YUV;
        #ifdef LOG_PRINT
            std::cout << "ColorSpace de MBT alterado para YUV.\n";
        #endif // LOG_PRINT
    } else if (color_space == RGB){
        c.toRGB(pixelMap, no_lines, no_columns);
        c.toRGB(palette, palette.size());
        color = MBT::ColorSpace::RGB;
        #ifdef LOG_PRINT
            std::cout << "ColorSpace de MBT alterado para RGB.\n";
        #endif // LOG_PRINT
    }
}

void MBT::readWithPalette(std::ifstream& InputStream){
    std::cout<<"Read pal\n\n\n\n";
}

void MBT::readWithoutPalette(std::ifstream& InputStream){
    unsigned int lines = info_header.biHeight;
    unsigned int cols =  info_header.biWidth;
    unsigned int lines_and_cols = lines*cols;
    Image::pixelMap.clear();
    Image::pixelMap.reserve(lines_and_cols);

    unsigned int i,j;
    for (i=0; i<lines_and_cols; i++){ //Create "n" "empty" positions
        Image::pixelMap.push_back(Pixel());
    }
    RGBTRI lineBuffer[cols];

    for(i=0; i<lines; i++){ //Read lines
        InputStream.read((char*)&lineBuffer, sizeof(lineBuffer));
        for (j=0; j<cols; j++){
            //RGBTRI bgrP = lineBuffer[cols-j-1]; //Invertido para passar para BMP
            RGBTRI bgrP = lineBuffer[j];
            Image::pixelMap[i*cols+j] = Pixel(bgrP.rgbBlue, bgrP.rgbGreen, bgrP.rgbRed);
        }
    }
}

int MBT::readFromFile(std::string FileName){
    std::ifstream InputStream(FileName, std::ios::in | std::ios::binary);
    if(!InputStream) return 0;

    BITMAPFILEHEADER BMPHdr;
    BITMAPINFOHEADER BMPInfHdr;
    InputStream.read(reinterpret_cast<char *>(&BMPHdr), sizeof(BITMAPFILEHEADER));
    InputStream.read(reinterpret_cast<char *>(&BMPInfHdr), sizeof(BITMAPINFOHEADER));

    file_header = BMPHdr;
    info_header = BMPInfHdr;

    Image::no_lines = info_header.biHeight;
    Image::no_columns = info_header.biWidth;

    // Verifica se formato da imagem é aceitável (deve ser ‘MB’)
    if(BMPHdr.Type != (('B' << 8) | 'M')) return -1;

    if(info_header.biSize > 0){ readWithPalette(InputStream); }
    else { readWithoutPalette(InputStream); }

    Image::red_bits   = 8;
    Image::green_bits = 8;
    Image::blue_bits  = 8;

    InputStream.close();

    #ifdef LOG_PRINT
        std::cout << "MBT lido de arquivo.\n";
    #endif // LOG_PRINT
    return 0;
}

void MBT::writeWithPalette(std::ofstream& Output){
    ///Nao generico, so para paleta de ate 256 elementos
    unsigned char color_coding[no_lines][no_columns];
    MiscMath m;

    #pragma omp parallel for
    for (unsigned int i=0; i<no_lines; i++){
        for (unsigned int j=0; j<no_columns; j++){
            color_coding[i][j] = m.lookUpPalette(palette, this->getPixel(i,j));
        }
    }

    RGBTRI palette_rs[palette.size()];
    #pragma omp parallel for
    for (unsigned int i=0; i<palette.size(); i++){
        palette_rs [i] = Image::tri_builder(palette.at(i));
    }

    Output.write(reinterpret_cast<char *>(&palette_rs), sizeof(palette_rs));

    Output.flush();
    unsigned int lines = info_header.biHeight;

    unsigned int i;
    for(i=0; i<lines; i++){
        Output.write((char*)&color_coding[i], sizeof(color_coding[i]));
    }
}

void MBT::writeWithoutPalette(std::ofstream& Output){
    ///Nao e generico, utiliza apenas formato 24 bits e 3 canais
    unsigned int cols = info_header.biWidth;
    unsigned int lines = info_header.biHeight;
    unsigned int i,j;
    RGBTRI buffer[lines][cols];
    Pixel p;

    for(i=0; i<lines; i++){
        for (j=0; j<cols; j++){
            p = Image::getPixel(i,j);
            buffer[i][j] = Image::tri_builder(p);
        }
    }
    for(i=0; i<lines; i++){
        Output.write((char*)&buffer[i], sizeof(buffer[i]));
    }
}

int MBT::writeToFile(std::string FileName){
    std::ofstream Output(FileName,std::ofstream::binary); //Não verifica se ha arquivo já existente
    file_header.Type = 0x424D;
    info_header.biSize = 0;
    Output.write(reinterpret_cast<char *>(&file_header), sizeof(BITMAPFILEHEADER));
    Output.write(reinterpret_cast<char *>(&info_header), sizeof(BITMAPINFOHEADER));

    Output.flush();

    if (palette.size() == 0){ writeWithoutPalette(Output); }
    else { writeWithPalette(Output);}

    Output.close();

    #ifdef LOG_PRINT
        std::cout << "MBT escrito em arquivo.\n";
    #endif // LOG_PRINT
    return 0;
}

BITMAPFILEHEADER MBT::getFileHeader() const{ return file_header; }
BITMAPINFOHEADER MBT::getInfoHeader() const{ return info_header; }
std::vector<Pixel> MBT::getPalette() const{  return palette; }
void MBT::setPalette(std::vector<Pixel> pal){ palette = pal;}

unsigned short MBT::getBitsPerColor () const{ return MBT::get1stChannelBits()+MBT::get2ndChannelBits()+MBT::get3rdChannelBits()+MBT::get4thChannelBits();}
unsigned char MBT::get1stChannelBits() const{ return (unsigned char)((file_header.Reserved>>8) & 0xFF); }
unsigned char MBT::get2ndChannelBits() const{ return (unsigned char)(file_header.Reserved & 0xFF);}
unsigned char MBT::get3rdChannelBits() const{ return (unsigned char)((file_header.Reserved2>>8) & 0xFF);}
unsigned char MBT::get4thChannelBits() const{ return (unsigned char)(file_header.Reserved2 & 0xFF);}
MBT::ColorSpace MBT::getColorSpace() const{ return color;}//return static_cast<ColorSpace>(file_header.Size);}
