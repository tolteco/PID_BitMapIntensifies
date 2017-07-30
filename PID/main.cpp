#include <omp.h>
#include <iostream>
#include <array>
#include "PIDMath.h"
#include "pixel.h"
#include "image.h"

#define OMP_THREADS 4
using namespace std;

void fillMap(Pixel* mat, int lines, int cols){
    for (int i=0; i<lines; i++){
        for (int j=0; j<cols; j++){
              mat[i*cols+j] = Pixel(i+48,j+48,48);
            //mat[i]l[j]
        }
    }
}

void print(Pixel* mat, int lines, int cols){
    for (int i=0; i<lines; i++){
        for (int j=0; j<cols; j++){
              cout << mat[i*cols+j] << ',';
            //mat[i]l[j]
        }
        cout << endl;
    }
}

void test1(){
    const int lines = 1920;
    const int columns = 1088;
    #define ROUND_UP(x, s) (((x)+((s)-1)) & -(s))
    //int lda = ROUND_UP(columns, 16);
    //int ldb = ROUND_UP(lines, 16);
    int lda = columns;
    int ldb = lines;
    cout << lda << "," << ldb << endl;
    Pixel *mat  = (Pixel*)_mm_malloc(sizeof(Pixel)*lda*ldb, 64);
    Pixel *matB = (Pixel*)_mm_malloc(sizeof(Pixel)*lda*ldb, 64);
    fillMap(mat, lines, columns);
    //print(mat, lines, columns);
    MatrixOperation m = MatrixOperation();
    m.transposeMatrix(mat, matB, lines, columns);
    cout << "Transpose\n";
    //print(matB, lda, ldb);
}

void test2(){
    char* name = "Tucano.bmp";
    BMP imag = BMP();
    imag.readFromFile(name);
    cout << imag;
}

int main() {
    if (omp_get_max_threads() > OMP_THREADS)
        omp_set_num_threads(OMP_THREADS);

    test2();
    return 0;
}



/* C++ Links
https://stackoverflow.com/questions/11747954/c-inheritance-in-separate-files-using-include-and-inclusion-guards

https://stackoverflow.com/questions/12854778/abstract-class-vs-interface-in-c

https://stackoverflow.com/questions/318064/how-do-you-declare-an-interface-in-c

https://stackoverflow.com/questions/4421706/what-are-the-basic-rules-and-idioms-for-operator-overloading

https://stackoverflow.com/questions/32810252/return-this-or-this-in-operator-overloading

*/

/* MAYBE USEFUL IN THE NEAR FUTURE LINKS

https://stackoverflow.com/questions/874298/c-templates-that-accept-only-certain-types

https://stackoverflow.com/questions/500493/c-equivalent-of-instanceof

*/

/* IMAGE RELATED LINKS

https://en.wikipedia.org/wiki/YUV

http://www.equasys.de/colorconversion.html



*/
