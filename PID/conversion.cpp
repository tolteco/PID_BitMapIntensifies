#include <algorithm>
#include <vector>
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

void Conversion::toRGB (std::vector<Pixel> mapa, unsigned int lines, unsigned int columns){
    unsigned char Y,U,V,
                  R,G,B,
                  C,D,E;
    Pixel temp_pixel;
    #pragma omp parallel
    for (unsigned int i=0; i<lines; i++){
        for (unsigned int j=0; j<columns; j++){ // O(n^2) intensifies
            temp_pixel = mapa[i*columns+j];
            Y = temp_pixel.get1st(); U = temp_pixel.get2nd(); V = temp_pixel.get3rd();
            C = Y-16; D = U-128; E = V-128;
            R = ( 298 * C           + 409 * E + 128) >> 8;
            G = ( 298 * C - 100 * D - 208 * E + 128) >> 8;
            B = ( 298 * C + 516 * D           + 128) >> 8;

            mapa[i*columns+j] = Pixel(R, G, B);
        }
    }
}

void Conversion::toRGB (std::vector<Pixel> pal, unsigned int lines){
    unsigned char Y,U,V,
                  R,G,B,
                  C,D,E;
    Pixel temp_pixel;
    #pragma omp parallel
    for (unsigned int i=0; i<lines; i++){
        temp_pixel = pal[i];
        Y = temp_pixel.get1st(); U = temp_pixel.get2nd(); V = temp_pixel.get3rd();
        C = Y-16; D = U-128; E = V-128;
        R = ( 298 * C           + 409 * E + 128) >> 8;
        G = ( 298 * C - 100 * D - 208 * E + 128) >> 8;
        B = ( 298 * C + 516 * D           + 128) >> 8;

        pal[i] = Pixel(R, G, B);
    }
}

void Conversion::toYUV (std::vector<Pixel> mapa, unsigned int lines, unsigned int columns){
    unsigned char Y,U,V,
                  R,G,B;
    Pixel temp_pixel;
    #pragma omp parallel
    for (unsigned int i=0; i<lines; i++){
        for (unsigned int j=0; j<columns; j++){ // O(n^2) intensifies

            temp_pixel = mapa[i*columns+j];
            R = temp_pixel.get1st(); G = temp_pixel.get2nd(); B = temp_pixel.get3rd();
            Y = ( (  66 * R + 129 * G +  25 * B + 128) >> 8) +  16;
            U = ( ( -38 * R -  74 * G + 112 * B + 128) >> 8) + 128;
            V = ( ( 112 * R -  94 * G -  18 * B + 128) >> 8) + 128;

            mapa[i*columns+j] = Pixel(Y, U, V);
        }
    }
}

void Conversion::toYUV (std::vector<Pixel> pal, unsigned int lines){
    unsigned char Y,U,V,
                  R,G,B;
    Pixel temp_pixel;
    #pragma omp parallel
    for (unsigned int i=0; i<lines; i++){
        temp_pixel = pal[i];
        R = temp_pixel.get1st(); G = temp_pixel.get2nd(); B = temp_pixel.get3rd();
        Y = ( (  66 * R + 129 * G +  25 * B + 128) >> 8) +  16;
        U = ( ( -38 * R -  74 * G + 112 * B + 128) >> 8) + 128;
        V = ( ( 112 * R -  94 * G -  18 * B + 128) >> 8) + 128;

        pal[i] = Pixel(Y, U, V);
    }
}
