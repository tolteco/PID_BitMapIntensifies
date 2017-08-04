#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <string>
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
        info_header.biSizeImage=0;
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
   : PersistableIMG(old.getLines(), old.getColumns(), old.getRedBits(), old.getGreenBits(), old.getBlueBits(), old.getMap()),
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

RGBQUAD quadBuilder(Pixel p){
    RGBQUAD quad;
    quad.rgbRed = p.get1st();
    quad.rgbGreen = p.get2nd();
    quad.rgbBlue = p.get3rd();
    quad.rgbReserved = 0;
    return quad;
}

void writeWithPalette(std::ofstream& Output){

}

void writeWithoutPalette(std::ofstream& Output){

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
        palette_rs [i] = quadBuilder(palette.at(i));
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
            RGBTRI bgrP = lineBuffer[cols-j-1];
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

RGBTRI triBuilder(Pixel p){
    RGBTRI tri;
    tri.rgbBlue  = p.get1st();
    tri.rgbGreen = p.get2nd();
    tri.rgbRed   = p.get3rd();
    return tri;
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
        palette_rs [i] = triBuilder(palette.at(i));
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
    RGBTRI buffer[cols][lines];

    Pixel p;
    #pragma omp parallel for private(p)
    for(i=0; i<lines; i++){
        for (j=0; j<cols; j++){
            p = Image::getPixel(i,j);
            buffer[i][j] = triBuilder(p);
        }
    }

    for(i=0; i<lines; i++){
        Output.write((char*)&buffer[i], sizeof(buffer[i]));
    }
}

int MBT::writeToFile(std::string FileName){
    std::ofstream Output(FileName,std::ofstream::binary); //Não verifica se ha arquivo já existente

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

