#include <iostream>
#include <algorithm>
#include <array>
#include "PIDMath.h"
#include "pixel.h"
#include "image.h"
using namespace std;

///Testes aleatorios com partes do codigo e etc

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
    cout << "Out: " << i << endl;
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
    MBT newImag = MBT(imag);
    newImag.writeToFile("out.mbt");
    newImag.readFromFile("out.mbt");
    cout << "\nTransformou.\n";
    newImag.changeColorSpace(MBT::ColorSpace::YUV);
    cout << "Converteu para YUV\n";

    Quantization q;
    vector<Pixel> palette = q.medianCut2ndAnd3rdChannels(&newImag, 256);
    newImag.setPalette(palette);
    imag = newImag.constructBMP();

    cout << "Quantizou.\nPaleta size: " << imag.getPalette().size() << endl;
    imag = newImag.constructBMP();
    cout << endl << imag << endl;

    cout << "\nBefore write...\n";

    imag.writeToFile("out.bmp");

    return;
}
