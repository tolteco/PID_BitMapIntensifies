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

void Conversion::toRGB (Pixel* mapa, int lines, int columns){
    MatrixOperation op;
    op.multiply(YUV_to_RGB_matrix, mapa, columns);
}

void Conversion::toYUV (Pixel* mapa, int lines, int columns){

}
