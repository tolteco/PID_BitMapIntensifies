#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include "conversion.h"
#include "image.h"
#include "PIDMath.h"

//destructor
Image::~Image() {
    _mm_free(pixelMap);
}

Image::Image(unsigned int lines, unsigned int columns,
     unsigned char red_bits,
     unsigned char green_bits,
     unsigned char blue_bits,
     Pixel* map_of_pixels)
     : no_lines(lines), no_columns(columns),
       red_bits(red_bits), green_bits(green_bits),
       blue_bits(blue_bits){

    pixelMap  = (Pixel*)_mm_malloc(sizeof(Pixel)*lines*columns, 64);
    std::copy(map_of_pixels, map_of_pixels+(lines*columns), pixelMap);
}

Image::Image(const Image &old){
    no_lines = old.getLines();
    no_columns = old.getColumns();
    pixels_meter_horizontal = old.getHorizontalResolution();
    pixels_meter_vertical = old.getVerticalResolution();

    red_bits   = old.getRedBits();
    green_bits = old.getGreenBits();
    blue_bits  = old.getBlueBits();

    pixelMap  = (Pixel*)_mm_malloc(sizeof(Pixel)*no_lines*no_columns, 64);
    std::copy(old.pixelMap, old.pixelMap+(no_lines*no_columns), pixelMap);
}

///GETTERS
Pixel Image::getPixel(unsigned int i, unsigned int j) const{ return Pixel(pixelMap[i*this->no_columns+j]); }
unsigned int Image::getLines() const{ return no_lines; }
unsigned int Image::getColumns() const{ return no_columns; }
unsigned char Image::getBitsPerColor() const{ return red_bits + green_bits + blue_bits; }
unsigned char Image::getRedBits() const{ return red_bits; }
unsigned char Image::getGreenBits() const{ return green_bits; }
unsigned char Image::getBlueBits() const{ return blue_bits; }
unsigned int Image::getHorizontalResolution() const{ return pixels_meter_horizontal; }
unsigned int Image::getVerticalResolution() const{ return pixels_meter_vertical; }

///SETTERS
void Image::setPixel(Pixel p, unsigned int i, unsigned int j){ pixelMap[i*this->no_columns+j] = p; }
void Image::setHorizontalResolution(unsigned int pixels_per_meter){ pixels_meter_horizontal = pixels_per_meter; }
void Image::setVerticalResolution(unsigned int pixels_per_meter){ pixels_meter_vertical = pixels_per_meter; }

std::ostream &operator<<(std::ostream &os, Image const &m) {
    unsigned int cols = m.no_columns;
    if(cols<=MAX_BM_PRINT && m.no_lines<=MAX_BM_PRINT){
        for (unsigned int i=0; i<m.no_lines; i++){
            for (unsigned int j=0; j<cols; j++)
                os << m.getPixel(i,j) << ',';
            os << std::endl;
        }
    }
    return os;
}

//Maybe we could use this as a wrapper to
//a generic function with a functor that operates
//between pixels? Nah. (Juliano).
Image Image::operator+(const Image& b) {
    /* E se as imagens forem de tamanhos
    diferentes? A imagem LHS nunca terá
    suas dimensões alteradas. Ex.:
    Img A 255 255  ImgB 240 230 100
           10  10        20  10   5

    Img Resultado 15 25
                   0  0

    O operador sub não dá "wrap" nos
    valores negativos por conta do operador
    + definido para os pixels.
    */

    Image px = Image(*this);
    unsigned int cols = this->no_columns;
    for (unsigned int i=0; i<this->no_lines; i++){
        for (unsigned int j=0; j<cols; j++){
            if( (i<b.no_lines) && (j<b.no_columns) ){
                px.pixelMap[i*cols+j] = this->getPixel(i,j) + b.getPixel(i,j);
            }
        }
    }

    return px;
}

Image Image::operator-(const Image& b) {
    Image px = Image(*this);
    unsigned int cols = this->no_columns;
    for (unsigned int i=0; i<this->no_lines; i++){
        for (unsigned int j=0; j<cols; j++){
            if( (i<b.no_lines) && (j<b.no_columns) ){
                px.pixelMap[i*cols+j] = this->getPixel(i,j) - b.getPixel(i,j);
            }
        }
    }

    return px;
}

Image Image::operator*(const Image& b) {
    Image px = Image(*this);
    unsigned int cols = this->no_columns;
    for (unsigned int i=0; i<this->no_lines; i++){
        for (unsigned int j=0; j<cols; j++){
            if( (i<b.no_lines) && (j<b.no_columns) ){
                px.pixelMap[i*cols+j] = this->getPixel(i,j) * b.getPixel(i,j);
            }
        }
    }

    return px;
}

Image Image::operator+(const Pixel& constant) {
    Image px = Image(*this);
    unsigned int cols = this->no_columns;
    for (unsigned int i=0; i<this->no_lines; i++){
        for (unsigned int j=0; j<cols; j++){
            px.pixelMap[i*cols+j] = this->getPixel(i,j) + constant;
        }
    }

    return px;
}

Image Image::operator-(const Pixel& constant) {
    Image px = Image(*this);
    unsigned int cols = this->no_columns;
    for (unsigned int i=0; i<this->no_lines; i++){
        for (unsigned int j=0; j<cols; j++){
            px.pixelMap[i*cols+j] = this->getPixel(i,j) - constant;
        }
    }

    return px;
}

Image Image::operator*(const Pixel& constant) {
    Image px = Image(*this);
    unsigned int cols = this->no_columns;
    for (unsigned int i=0; i<this->no_lines; i++){
        for (unsigned int j=0; j<cols; j++){
            px.pixelMap[i*cols+j] = this->getPixel(i,j) * constant;
        }
    }

    return px;
}

///BMP

BMP::BMP(const BMP& old){
    no_lines = old.getLines();
    no_columns = old.getColumns();
    pixels_meter_horizontal = old.getHorizontalResolution();
    pixels_meter_vertical = old.getVerticalResolution();
    red_bits   = old.getRedBits();
    green_bits = old.getGreenBits();
    blue_bits  = old.getBlueBits();
    pixelMap  = (Pixel*)_mm_malloc(sizeof(Pixel)*no_lines*no_columns, 64);
    std::copy(old.pixelMap, old.pixelMap+(no_lines*no_columns), pixelMap);

    file_header = old.getFileHeader();
    info_header = old.getInfoHeader();
    if (old.palette.size() > 0){
        palette.reserve(old.getPalette().size());
        std::copy(old.getPalette().begin(), old.getPalette().end(), palette.begin());
        isInBGRformat = old.isBGR();
    }
}

BITMAPFILEHEADER BMP::getFileHeader() const{ return file_header; }
BITMAPINFOHEADER BMP::getInfoHeader() const{ return info_header; }
std::vector<Pixel> BMP::getPalette() const{  return palette; }
bool BMP::isBGR() const{ return isInBGRformat; }

int BMP::readFromFile(char* FileName){
    std::ifstream InputStream(FileName, std::ios::in | std::ios::binary);
    if(!InputStream) return 0;

    BITMAPFILEHEADER BMPHdr;
    BITMAPINFOHEADER BMPInfHdr;
    InputStream.read(reinterpret_cast<char *>(&BMPHdr), sizeof(BITMAPFILEHEADER));
    InputStream.read(reinterpret_cast<char *>(&BMPInfHdr), sizeof(BITMAPINFOHEADER));

    // Verifica se formato da imagem é aceitável (deve ser ‘BM’)
    if(BMPHdr.Type != (('M' << 8) | 'B')) return -1;

    Pixel *mat  = (Pixel*)_mm_malloc(sizeof(Pixel)*BMPInfHdr.biHeight*BMPInfHdr.biWidth, 64);
    /*RGBQUAD palette_rs[256];
    int offset = BMPHdr.OffsetBits;
    if (offset != 54){ //Tem paleta
        offset -= 54;
        offset /= 4;

        InputStream.read((char*)&palette_rs, sizeof(RGBQUAD)*offset);
    }*/

    MiscMath mscm; ///Generalizar para diferente de True color
    unsigned int linePixels = BMPInfHdr.biWidth;
    unsigned int lineBytes = mscm.roundUpToNearestMultiple(linePixels*3, 4);

    RGBTRI lineBuffer[linePixels];
    unsigned int no_spare_bytes = lineBytes - (linePixels*3);
    char spareBytes[no_spare_bytes];

    unsigned int i,j;
    unsigned int cols = BMPInfHdr.biWidth;
    for(i=0; i<BMPInfHdr.biHeight; i++){ //Read lines
        InputStream.read((char*)&lineBuffer, sizeof(lineBuffer));

        if (no_spare_bytes != 0){ //Read spare bytes
           InputStream.read((char*)&spareBytes, sizeof(spareBytes));
        }
        for (j=0; j<cols; j++){ //Freaking std::reverse(std::begin(lineBuffer), std::end(lineBuffer));
            RGBTRI bgrP = lineBuffer[linePixels-j-1];
            mat[i*cols+j] = Pixel(bgrP.rgbRed, bgrP.rgbGreen, bgrP.rgbBlue);
        }
    }

    Image::pixelMap = mat;
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
    return 0;
}

int BMP::writeToFile(char* FileName){
    if (!isBGR()){
        swap1stAnd3rdChannels();
    }
    //Write to file

    return -1;
}

void swapUCHAR(unsigned char *a, unsigned char *b){
    unsigned char temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void BMP::swap1stAnd3rdChannels(){
    //stuff
    unsigned int lines = this->Image::getLines();
    unsigned int cols = this->Image::getColumns();

    unsigned char first, third;
    for (unsigned int i=0; i<lines; i++){
        for (unsigned int j=0; j<cols; j++){
            first = Image::pixelMap[i*cols+j].get1st();
            third = Image::pixelMap[i*cols+j].get3rd();
            swapUCHAR(&first, &third);
            Image::pixelMap[i*cols+j].set1st(first);
            Image::pixelMap[i*cols+j].set3rd(third);
        }
    }

    isInBGRformat = ( isInBGRformat ? false : true); //swap bool value
}

std::ostream &operator<<(std::ostream &os, BMP const &m) {
    return os << m.info_header.biWidth << "x" << m.info_header.biHeight;
}

void BMP::changeToBGR(){
    if (isBGR()) return;
    swap1stAnd3rdChannels();
}

///MBT

MBT::MBT(BMP old){
    if (old.isBGR()){
        old.changeToBGR();
    }

    file_header = old.getFileHeader();
    info_header = old.getInfoHeader();

    std::vector<Pixel> temPalette = old.getPalette();
    if (temPalette.size() > 0){
        palette.reserve(temPalette.size());
        std::copy(temPalette.begin(), temPalette.end(), palette.begin());
    }
    color = MBT::ColorSpace::RGB;
}

BMP MBT::constructBMP(){
    if (color == MBT::ColorSpace::YUV){
        MBT::changeColorSpace(MBT::ColorSpace::RGB);
    }

    ///STUFF
}

void MBT::changeColorSpace(MBT::ColorSpace color_space){
    if (color_space == color) return; //Nao precisa converter para o mesmo

    Conversion c;
    //So converte se foi definido na enum "ColrSchemes"
    if (color_space == YUV){
        std::cout<<"Entrou em toYUV\n";
        c.toYUV(pixelMap, no_lines, no_columns);
        color = MBT::ColorSpace::YUV;
    } else if (color_space == RGB){
        std::cout<<"Entrou em toRGB\n";
        c.toRGB(pixelMap, no_lines, no_columns);
        color = MBT::ColorSpace::RGB;
    }
    std::cout<<"Saiu de ChangeColorSpace\n";
}

int MBT::readFromFile(char* FileName){
    return -1;
}

int MBT::writeToFile(char* FileName){
    return -1;
}

