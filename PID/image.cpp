#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include "image.h"
#include "PIDMath.h"

//destructor
//Image::~Image() {}

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

///GETTERS

Pixel Image::getPixel(unsigned int i, unsigned int j) const{
    return Pixel(pixelMap[i*this->no_columns+j]);
}

unsigned int Image::getLines(){
    return no_lines;
}

unsigned int Image::getColumns(){
    return no_columns;
}

unsigned char Image::getBitsPerColor(){
    return red_bits + green_bits + blue_bits;
}

unsigned char Image::getRedBits(){
    return red_bits;
}

unsigned char Image::getGreenBits(){
    return green_bits;
}

unsigned char Image::getBlueBits(){
    return blue_bits;
}

unsigned int Image::getHorizontalResolution(){
    return pixels_meter_horizontal;
}

unsigned int Image::getVerticalResolution(){
    return pixels_meter_vertical;
}

///SETTERS
void Image::setHorizontalResolution(unsigned int pixels_per_meter){
    pixels_meter_horizontal = pixels_per_meter;
}

void Image::setVerticalResolution(unsigned int pixels_per_meter){
    pixels_meter_vertical = pixels_per_meter;
}

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
    diferentes? A imagem LHS nunca ter�
    suas dimens�es alteradas. Ex.:
    Img A 255 255  ImgB 240 230 100
           10  10        20  10   5

    Img Resultado 15 25
                   0  0

    O operador sub n�o d� "wrap" nos
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

//Notas de aula de PID :D
/*int BMP::readFromFile(char* FileName){
    std::ifstream InputStream(FileName, std::ios::in | std::ios::binary);
    if(!InputStream) return 0;

    // L� o header do arquivo imagem
    InputStream.read((unsigned char*)&BMPHdr, sizeof(BITMAPFILEHEADER));

    // Verifica se formato da imagem � aceit�vel (deve ser �BM�)
    if(BMPHdr.bfType != (('M' << 8) | 'B')) return -1;

    // Determina o tamanho do DIB para leitura atrav�s do campo bfSize
    // menos o tamanho do BITMAPFILEHEADER
    long bmpsize = BMPHdr.bfSize - sizeof(BITMAPFILEHEADER);

    // Aloca espa�o para o bitmap
    PointerToDIB = GlobalAllocPtr(GHND, bmpsize);

    // Se a mem�ria falha retorna NULL
    if(PointerToDIB == 0) return -1;

    // Aloca espa�o para a imagem
    unsigned char *rbuf = new unsigned char[MaxBlock];
    if(rbuf == NULL) return -1;

    unsigned char huge
    *DIBData = (unsigned char huge*) PointerToDIB;
    unsigned int chunksize;
    unsigned int i;
    // Faz leitura por peda�os at� acabar o arquivo
    while(bmpsize > 0) {
        if(bmpsize > MaxBlock)
            chunksize = MaxBlock;
        else
            chunksize = (WORD)bmpsize;
            InputStream.read(rbuf, chunksize);
        // Copia para o DIB
        for(i = 0; i < chunksize; i++)
            DIBData[i] = rbuf[i];
        bmpsize -= chunksize;
        DIBData += chunksize;
    }
    delete rbuf;
    // Computa o n�mero de bytes por linha, arredondando para m�ltiplo de 4
    LPBITMAPINFOHEADER pBMPInfo = (LPBITMAPINFOHEADER)PointerToDIB;
    BytePerLine = ((long)pBMPInfo->biWidth * (long)pBMPInfo->biBitCount +
                    31L) / 32 * 4;
    return TRUE;
}*/

int BMP::readFromFile(char* FileName){
    std::ifstream InputStream(FileName, std::ios::in | std::ios::binary);
    if(!InputStream) return 0;

    BITMAPFILEHEADER BMPHdr;
    BITMAPINFOHEADER BMPInfHdr;
    InputStream.read(reinterpret_cast<char *>(&BMPHdr), sizeof(BITMAPFILEHEADER));
    InputStream.read(reinterpret_cast<char *>(&BMPInfHdr), sizeof(BITMAPINFOHEADER));

    // Verifica se formato da imagem � aceit�vel (deve ser �BM�)
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

    isBGR = ( isBGR ? false : true); //swap bool value
}

std::ostream &operator<<(std::ostream &os, BMP const &m) {
    return os << m.info_header.biWidth << "x" << m.info_header.biHeight;
}
