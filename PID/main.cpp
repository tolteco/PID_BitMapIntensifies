#include <omp.h>
#include <iostream>
#include <algorithm>
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
    int i=imag.readFromFile(name);
    cout << "AHHHHHHHHHHHHHHHH: " << i << endl;
    cout << imag;
}

#include <vector>
#include <iterator>
int test3(vector<int>arr1, vector<int>arr2){
    //vector<int>arr1{1,2,3,-1,-2,-3};
    //vector<int>arr2{1,2,3,-1,-2,-4};
    auto minmax1 = std::minmax_element(arr1.begin(), arr1.end());
    auto minmax2 = std::minmax_element(arr2.begin(), arr2.end());

    int range1 = arr1.at(minmax1.second - arr1.begin()) - arr1.at(minmax1.first - arr1.begin());
    int range2 = arr2.at(minmax2.second - arr2.begin()) - arr2.at(minmax2.first - arr2.begin());

    cout<<endl;
    cout<<range1<<endl;
    cout<<range2<<endl;

    if (range1 > range2) return 1;
    else return 2;
}

void test4(vector<int>*arr1){
    iter_swap(arr1->begin() + 0, arr1->begin() + 3);
}

void test5(){
    char* name = "Tucano.bmp";
    BMP imag = BMP();
    int i=imag.readFromFile(name);
    cout << "Leu imagem: " << imag << endl;

    cout << "Chamando quantizacao...\n";

    vector<Pixel> palette;
    Quantization q;
    palette = q.medianCut2ndAnd3rdChannels(&imag, 256);

    cout << "Fim da quantizacao.\n";
}

void test6(){
    char* name = "Tucano.bmp";
    BMP imag = BMP();
    int i=imag.readFromFile(name);
    cout << "Leu imagem: " << imag << endl;
    Pixel t1 = imag.getPixel(0,0);
    t1 = t1+Pixel(48,48,48);
    cout << t1;
    MBT newImag = MBT(imag);
    Pixel t2 = newImag.getPixel(0,0);
    cout << "\nTransformou.\n";

    newImag.changeColorSpace(MBT::ColorSpace::YUV);
    Pixel t3 = newImag.getPixel(0,0);
    cout << "Converteu para YUV\n";

    Quantization q;
    vector<Pixel> palette = q.medianCut2ndAnd3rdChannels(&newImag, 256);

    cout << "Quantizou\n";
    return;
}

int main() {
    if (omp_get_max_threads() > OMP_THREADS)
        omp_set_num_threads(OMP_THREADS);

    test6();

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
