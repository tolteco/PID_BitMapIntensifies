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

//Notas de aula de PID :D
/*int BMP::readFromFile(char* FileName){
    std::ifstream InputStream(FileName, std::ios::in | std::ios::binary);
    if(!InputStream) return 0;

    // Lê o header do arquivo imagem
    InputStream.read((unsigned char*)&BMPHdr, sizeof(BITMAPFILEHEADER));

    // Verifica se formato da imagem é aceitável (deve ser ‘BM’)
    if(BMPHdr.bfType != (('M' << 8) | 'B')) return -1;

    // Determina o tamanho do DIB para leitura através do campo bfSize
    // menos o tamanho do BITMAPFILEHEADER
    long bmpsize = BMPHdr.bfSize - sizeof(BITMAPFILEHEADER);

    // Aloca espaço para o bitmap
    PointerToDIB = GlobalAllocPtr(GHND, bmpsize);

    // Se a memória falha retorna NULL
    if(PointerToDIB == 0) return -1;

    // Aloca espaço para a imagem
    unsigned char *rbuf = new unsigned char[MaxBlock];
    if(rbuf == NULL) return -1;

    unsigned char huge
    *DIBData = (unsigned char huge*) PointerToDIB;
    unsigned int chunksize;
    unsigned int i;
    // Faz leitura por pedaços até acabar o arquivo
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
    // Computa o número de bytes por linha, arredondando para múltiplo de 4
    LPBITMAPINFOHEADER pBMPInfo = (LPBITMAPINFOHEADER)PointerToDIB;
    BytePerLine = ((long)pBMPInfo->biWidth * (long)pBMPInfo->biBitCount +
                    31L) / 32 * 4;
    return TRUE;
}*/

/*int BMP::readFromFile(char* FileName){
    FILE *filePtr; //our file pointer
    BITMAPFILEHEADER bitmapFileHeader; //our bitmap file header
    unsigned char *bitmapImage;  //store image data
    int imageIdx=0;  //image index counter
    unsigned char tempRGB;  //our swap variable

    //open filename in read binary mode
    filePtr = fopen(filename,"rb");
    if (filePtr == NULL)
        return NULL;

    //read the bitmap file header
    fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER),1,filePtr);

    //verify that this is a bmp file by check bitmap id
    if (bitmapFileHeader.bfType !=0x4D42)
    {
        fclose(filePtr);
        return NULL;
    }

    //read the bitmap info header
    fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER),1,filePtr); // small edit. forgot to add the closing bracket at sizeof

    //move file point to the begging of bitmap data
    fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

    //allocate enough memory for the bitmap image data
    bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

    //verify memory allocation
    if (!bitmapImage)
    {
        free(bitmapImage);
        fclose(filePtr);
        return NULL;
    }

    //read in the bitmap image data
    fread(bitmapImage,bitmapInfoHeader->biSizeImage,filePtr);

    //make sure bitmap image data was read
    if (bitmapImage == NULL)
    {
        fclose(filePtr);
        return NULL;
    }

    //swap the r and b values to get RGB (bitmap is BGR)
    for (imageIdx = 0;imageIdx < bitmapInfoHeader->biSizeImage;imageIdx+=3) // fixed semicolon
    {
        tempRGB = bitmapImage[imageIdx];
        bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
        bitmapImage[imageIdx + 2] = tempRGB;
    }

    //close file and return bitmap iamge data
    fclose(filePtr);
    return bitmapImage;
}*/

int BMP::readFromFile(char* FileName){
    std::ifstream InputStream(FileName, std::ios::in | std::ios::binary);
    if(!InputStream) return 0;

    BITMAPFILEHEADER BMPHdr;
    BITMAPINFOHEADER BMPInfHdr;
    InputStream.read((char*)&BMPHdr, sizeof(BITMAPFILEHEADER));
    InputStream.read((char*)&BMPInfHdr, sizeof(BITMAPINFOHEADER));

    // Verifica se formato da imagem é aceitável (deve ser ‘BM’)
    if(BMPHdr.Type != (('M' << 8) | 'B')) return -1;

    Pixel *mat  = (Pixel*)_mm_malloc(sizeof(Pixel)*BMPInfHdr.biHeight*BMPInfHdr.biWidth, 64);
    if(mat == nullptr) return -1;

    RGBQUAD palette_rs[256];
    int offset = BMPHdr.OffsetBits;
    if (offset != 54){ //Tem paleta
        offset -= 54;
        offset /= 4;

        InputStream.read((char*)&palette_rs, sizeof(RGBQUAD)*offset);
    }

    MiscMath mscm; ///Generalizar para diferente de True color
    unsigned int linePixels = BMPInfHdr.biWidth;
    unsigned int lineBytes = mscm.roundUpToNearestMultiple(linePixels*3, 4);
    Pixel* lineBuffer = new Pixel[linePixels];
    char* spareBytes;
    unsigned int no_spare_bytes = lineBytes - (linePixels*3);
    if (no_spare_bytes != 0){
        spareBytes = new char[no_spare_bytes];
    }

    ///std::copy(map_of_pixels, map_of_pixels+(lines*columns), pixelMap);
    unsigned int i;
    unsigned int pixelCount=0;
    for(i=0; i<BMPInfHdr.biHeight; i++){ //Read lines
        InputStream.read((char*)&lineBuffer, sizeof(Pixel)*linePixels);
        if (no_spare_bytes != 0){ //Read spare bytes
           InputStream.read(spareBytes, sizeof(char)*no_spare_bytes);
        }
        std::reverse(lineBuffer, lineBuffer+linePixels);
        //std::reverse(std::begin(lineBuffer), std::end(lineBuffer));
        std::copy(lineBuffer, lineBuffer+linePixels, mat+pixelCount);
        //std::copy(std::begin(lineBuffer), std::end(lineBuffer), mat+pixelCount);
        pixelCount += linePixels;
    }

    delete[] lineBuffer;
    if (no_spare_bytes != 0) delete[] spareBytes;


    //CONSTRUIR NOVA INSTÂNCIA BMP

    return 0;
}

int BMP::writeToFile(char* FileName){
    return -1;
}
