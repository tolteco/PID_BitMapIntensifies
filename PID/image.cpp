#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include "conversion.h"
#include "image.h"
#include "PIDMath.h"

//destructor
Image::~Image() {
    std::vector<Pixel>().swap(pixelMap); //https://stackoverflow.com/questions/993590/should-i-delete-vectorstring
}

Image::Image(unsigned int lines, unsigned int columns,
     unsigned char red_bits,
     unsigned char green_bits,
     unsigned char blue_bits,
     std::vector<Pixel> map_of_pixels)
     : no_lines(lines), no_columns(columns),
       red_bits(red_bits), green_bits(green_bits),
       blue_bits(blue_bits),
       pixelMap(map_of_pixels){ }

Image::Image(const Image &old)
     : no_lines(old.no_lines), no_columns(old.no_lines),
       red_bits(old.red_bits), green_bits(old.green_bits),
       blue_bits(old.blue_bits),
       pixelMap(old.pixelMap){ }

///GETTERS
Pixel         Image::getPixel(unsigned int i, unsigned int j) const{ return pixelMap[i*this->no_columns+j]; } //return Pixel(pixelMap[i*this->no_columns+j]); }
unsigned int  Image::getLines() const{ return no_lines; }
unsigned int  Image::getColumns() const{ return no_columns; }
unsigned char Image::getBitsPerColor() const{ return red_bits + green_bits + blue_bits; }
unsigned char Image::getRedBits() const{ return red_bits; }
unsigned char Image::getGreenBits() const{ return green_bits; }
unsigned char Image::getBlueBits() const{ return blue_bits; }
unsigned int  Image::getHorizontalResolution() const{ return pixels_meter_horizontal; }
unsigned int  Image::getVerticalResolution() const{ return pixels_meter_vertical; }
std::vector<Pixel> Image::getMap() const{ return pixelMap; }

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

///Persistable IMG
PersistableIMG::PersistableIMG(unsigned int lines, unsigned int columns,
                unsigned char red_bits,
                unsigned char green_bits,
                unsigned char blue_bits,
                std::vector<Pixel> map_of_pixels)
             : Image(lines, columns, red_bits, green_bits, blue_bits, map_of_pixels){}

///BMP

BMP::BMP(BITMAPFILEHEADER fil, BITMAPINFOHEADER inf, std::vector<Pixel> palette, MBT* i) //https://stackoverflow.com/questions/120876/what-are-the-rules-for-calling-the-superclass-constructor
   : PersistableIMG(i->getLines(), i->getColumns(), i->getRedBits(), i->getGreenBits(), i->getBlueBits(), i->getMap()),
     file_header(fil), info_header(inf), palette(palette) {

    file_header.Reserved2 = 0;
    file_header.Reserved = 0;

    if(palette.size() > 0){
        file_header.OffsetBits = 54 + (palette.size()*4);
        file_header.Size = BMP::BMP_file_size(palette.size(), no_lines, no_columns, 8);
        info_header.biBitCount = 8;
        std::cout<<"Build here. Size: " << info_header.biSize << " Bit: " << info_header.biBitCount << "\n";
        info_header.biSizeImage=0;
    } else {
        file_header.OffsetBits = 54;
        info_header.biSize = BMP::BMP_file_size(palette.size(), no_lines, no_columns, 24);
        info_header.biSizeImage = info_header.biSize-54;
    }
}

BMP::BMP(const BMP& old)
   : PersistableIMG(old.getLines(), old.getColumns(), old.getRedBits(), old.getGreenBits(), old.getBlueBits(), old.getMap()),
     file_header(old.file_header), info_header(old.info_header), palette(old.palette), isInBGRformat(old.isBGR()){ }

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

    unsigned int lines = BMPInfHdr.biHeight;
    unsigned int cols = BMPInfHdr.biWidth;
    unsigned int lines_and_cols = lines*cols;
    Image::pixelMap.clear();
    Image::pixelMap.reserve(lines_and_cols);

    unsigned int i,j;
    for (i=0; i<lines_and_cols; i++){ //Create "n" "empty" positions
        Image::pixelMap.push_back(Pixel());
    }

    /*RGBQUAD palette_rs[256];
    int offset = BMPHdr.OffsetBits;
    if (offset != 54){ //Tem paleta
        offset -= 54;
        offset /= 4;

        InputStream.read((char*)&palette_rs, sizeof(RGBQUAD)*offset);
    }*/

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
    return 0;
}

RGBQUAD quadBuilder(Pixel p){
    RGBQUAD quad;
    quad.rgbRed = p.get1st();
    quad.rgbGreen = p.get2nd();
    quad.rgbBlue = p.get3rd();
    quad.rgbReserved = 0;
    return quad;
}

int BMP::writeToFile(char* FileName){
    std::cout<<"Escrita\n";
    if (isBGR()){ swap1stAnd3rdChannels(); }
    //Write to file
    std::ofstream Output(FileName,std::ofstream::binary); //Não verifica se ha arquivo já existente
std::cout<<"Outstream\n";
    Output.write(reinterpret_cast<char *>(&file_header), sizeof(BITMAPFILEHEADER));
    Output.write(reinterpret_cast<char *>(&info_header), sizeof(BITMAPINFOHEADER));
std::cout<<"Write header\n";
    unsigned char color_coding[no_lines][no_columns];
    Output.flush();
    MiscMath m;
    //#pragma omp parallel for
    for (unsigned int i=0; i<no_lines; i++){
        for (unsigned int j=0; j<no_columns; j++){
            color_coding[i][j] = m.lookUpPalette(palette, this->getPixel(i,j));
        }
    }
    std::cout<<"Over, anakin\n";
    RGBQUAD palette_rs[palette.size()];
    //#pragma omp parallel for
    for (unsigned int i=0; i<palette.size(); i++){
        palette_rs [i] = quadBuilder(palette.at(i));
    }
    Output.write(reinterpret_cast<char *>(&palette_rs), sizeof(palette_rs));
std::cout<<"Write palette.\n";
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
            lineBuffer[linePixels-j-1] = color_coding[i][j];
        }
        Output.write((char*)&lineBuffer, sizeof(lineBuffer));
        if (no_spare_bytes != 0){
           Output.write((char*)&spareBytes, sizeof(spareBytes));
        }

    }
    Output.close();

    return 0;
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

///MBT

MBT::MBT(const BMP& old)
   : PersistableIMG(old.getLines(), old.getColumns(), old.getRedBits(), old.getGreenBits(), old.getBlueBits(), old.getMap()),
     file_header(old.getFileHeader()), info_header(old.getInfoHeader()), palette(old.getPalette()){

    /*if (old.isBGR()){
        old.changeToBGR();
    }*/

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

BITMAPFILEHEADER MBT::getFileHeader() const{ return file_header; }
BITMAPINFOHEADER MBT::getInfoHeader() const{ return info_header; }
std::vector<Pixel> MBT::getPalette() const{  return palette; }
void MBT::setPalette(std::vector<Pixel> pal){ palette = pal;}
