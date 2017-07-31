#include <algorithm>
#include "conversion.h"
#include "pixel.h"
#include "PIDMath.h"
#include "PIDMath.h"

/*

https://stackoverflow.com/questions/17892346/how-to-convert-rgb-yuv-rgb-both-ways

http://www.equasys.de/colorconversion.html

https://stackoverflow.com/questions/14018666/converting-rgb-image-to-yuv-using-c-programming

http://www.fourcc.org/fccyvrgb.php

https://msdn.microsoft.com/en-us/library/aa904813(VS.80).aspx#yuvformats_2

*/

double YUV_to_RGB_matrix[3][3] = {
    { 0.299,  0.587,  0.114},
    {-0.147, -0.289,  0.436},
    { 0.615, -0.515, -0.100}
};

double RGB_to_YUV_matrix[3][3] = {
    { 1.,  0.000,  1.140},
    { 1., -0.395, -0.581},
    { 1.,  2.032,  0.000}
};

/*void Conversion::toRGB (Pixel* mapa, int lines, int columns){
    MatrixOperation op;
    op.multiply(YUV_to_RGB_matrix, mapa, columns);
}*/

void Conversion::toRGB (Pixel* mapa, int lines, int columns){

}

void Conversion::toYUV (Pixel* mapa, int lines, int columns){
    MatrixOperation op;
    //A * B, sendo A e B matrizes. ColsA=LinhasB AND LinsA=LinsB, repetindo a op "ColsB" vezes
    // Transpose B, multiply, transpose B
    unsigned int multLines = 3, multColumns = 3;

    Pixel* toTranspose = (Pixel*)_mm_malloc(sizeof(Pixel)*multLines*columns, 64);
    std::copy(mapa, mapa+(multLines*columns), toTranspose);
    op.transposeMatrix(mapa, toTranspose, multColumns, columns);

    ///Multiplication

    unsigned char Y,U,V,
                  R,G,B;
    Pixel temp_pixel;
    #pragma omp parallel
    for (unsigned int i=0; i<multLines; i++){
        for (unsigned int j=0; j<columns; j++){ // O(n^2) intensifies
            //temp_pixel = mapa[i*columns+j]; //Normal
            temp_pixel = toTranspose[j*multLines+i]; //Transposta
            R = temp_pixel.get1st(); G = temp_pixel.get2nd(); B = temp_pixel.get3rd();
            Y = ( (  66 * R + 129 * G +  25 * B + 128) >> 8) +  16;
            U = ( ( -38 * R -  74 * G + 112 * B + 128) >> 8) + 128;
            V = ( ( 112 * R -  94 * G -  18 * B + 128) >> 8) + 128;

            toTranspose[j*multLines+i] = Pixel(Y, U, V);
        }
    }

    ///End Multiplication

    op.transposeMatrix(toTranspose, mapa, columns, multColumns);
}
