#include <omp.h>
#include <math.h>
#include <iostream>
//#include <xmmintrin.h>
#include "PIDMath.h"
#include "pixel.h"

//https://stackoverflow.com/questions/16737298/what-is-the-fastest-way-to-transpose-a-matrix-in-c
inline void transpose_scalar_block(Pixel *A, Pixel *B, const int lda, const int ldb, const int block_size) {
    #pragma omp parallel for
    for(int i=0; i<block_size; i++) {
        for(int j=0; j<block_size; j++) {
            B[j*ldb + i] = A[i*lda +j];
        }
    }
}

inline void transpose_block(Pixel *A, Pixel *B, const int n, const int m, const int lda, const int ldb, const int block_size) {
    #pragma omp parallel for
    for(int i=0; i<n; i+=block_size) {

        for(int j=0; j<m; j+=block_size) {
            transpose_scalar_block(&A[i*lda +j], &B[j*ldb + i], lda, ldb, block_size);
        }
    }
}

void transpose(Pixel *src, Pixel *dst, const int N, const int M) {
    #pragma omp parallel for
    for(int n = 0; n<N*M; n++) {
        int i = n/N;
        int j = n%N;
        dst[n] = src[M*j + i];
    }
}

void MatrixOperation::transposeMatrix (Pixel* src, Pixel* dst, const unsigned int lines, const unsigned int columns){
    /*if(lines%BLOCK_SIZE==0 && columns%BLOCK_SIZE==0){
        //This method seems to be slower...
        int lda = ROUND_UP(columns, BLOCK_SIZE);
        int ldb = ROUND_UP(lines, BLOCK_SIZE);
        transpose_block(src, dst, lines, columns, lda, ldb, BLOCK_SIZE); //Block tem que ser m�ltiplo, 3*5=15
    } else {*/{
        transpose(src, dst, lines, columns);
    }
}

void MatrixOperation::multiply(Pixel* mat, const unsigned int lines,     const unsigned int columns,
            double* mult, const unsigned int multLines, const unsigned int multColumns){


}

unsigned int MiscMath::roundUpToNearestMultiple(unsigned int num, unsigned int multiple){
    if (multiple == 0)
        return num;

    int remainder = num % multiple;
    if (remainder == 0)
        return num;

    return num + multiple - remainder;
}

#include <vector>
#include <algorithm>

void swapCHAR(unsigned char *a, unsigned char *b){
    unsigned char temp;
    temp = *a;
    *a = *b;
    *b = temp;
}


//http://www.vogella.com/tutorials/JavaAlgorithmsQuicksort/article.html
void quick_sort(std::vector<Pixel>* arr, int beg, int end, char chnl){
        Pixel pivot = arr->at(beg + (end-beg)/2);

        int begC = beg,
            endC = end;

        while(begC <= endC){
            while(arr->at(begC).moreThan(pivot, chnl)) ++begC;
            while(arr->at(endC).lessThan(pivot, chnl)) --endC;

            if (begC <= endC){
                iter_swap(arr->begin() + begC, arr->begin() + endC);
                ++begC;
                --endC;
            }
        }

        if (beg < endC) quick_sort(arr, beg, endC, chnl);
        if (begC < end) quick_sort(arr, begC, end, chnl);
}

int chnl_range(std::vector<unsigned char> arr1){
    auto minmax1 = std::minmax_element(arr1.begin(), arr1.end());
    return arr1.at(minmax1.second - arr1.begin()) - arr1.at(minmax1.first - arr1.begin());
}

char chnl_with_greatest_range(std::vector<unsigned char> arr1, std::vector<unsigned char> arr2){
    auto minmax1 = std::minmax_element(arr1.begin(), arr1.end());
    auto minmax2 = std::minmax_element(arr2.begin(), arr2.end());

    int range1 = arr1.at(minmax1.second - arr1.begin()) - arr1.at(minmax1.first - arr1.begin());
    int range2 = arr2.at(minmax2.second - arr2.begin()) - arr2.at(minmax2.first - arr2.begin());

    if (range1 > range2) return 1;
    else return 2;
}

char chnl_with_greatest_range(std::vector<Pixel> arr){
    std::vector<unsigned char> chnl_1, chnl_2;
    chnl_1.reserve(arr.size()); chnl_2.reserve(arr.size());
    for (unsigned int i=0; i<arr.size(); i++){
        Pixel p = arr.at(i);
        chnl_1.push_back(p.get2nd());
        chnl_2.push_back(p.get3rd());
    }

    int range1 = chnl_range(chnl_1),
        range2 = chnl_range(chnl_2);

    if (range1 > range2) return 1;
    else return 2;
}

Image Quantization::medianCut2ndAnd3rdChannels(Image img, unsigned short maxColors){
    unsigned int lines = img.getLines();
    unsigned int cols = img.getColumns();

    std::vector<Pixel> pixels;
    pixels.reserve(lines*cols);
    for(unsigned int i=0; i<lines; i++){
        for (unsigned int j=0; j<cols; j++){
            pixels.push_back(img.getPixel(i,j));
        }
    }
std::cout << "Pixels postos no primeiro vector\n";
    ///http://blog.isamert.net/median-cut-algorithm-in-qt-c/
    //http://www.cplusplus.com/forum/general/833/
    std::vector<std::vector<Pixel>> p_lists;
    p_lists.push_back(pixels);
std::cout << "Pixels postos no vector de vectors\n";
    while(p_lists.size() < maxColors){ //Main loop

        unsigned int lists_size = p_lists.size();
std::cout << "Size: " << lists_size << "\n";
        for (unsigned int i=0; i<lists_size; lists_size--){
            char b_range = chnl_with_greatest_range(p_lists.at(i)); //Canal com maior range
            quick_sort(&p_lists.at(i), 0, p_lists.at(i).size()-1, b_range); //Sort de acordo com o canal

            std::vector<Pixel> temp = p_lists.at(i);
            std::vector<Pixel> f_2(temp.begin(), temp.begin() + temp.size()/2),
                               f_3(temp.begin() + temp.size()/2, temp.end());
            p_lists.erase(p_lists.begin()+i); p_lists.push_back(f_2); p_lists.push_back(f_3); //Adiciona novas listas no fim
        }
std::cout << "Finalizou iteracao. Subs: " << p_lists.size() << "\n";
    }

    std::cout << "Divis�o completa. B: " << p_lists.size() << "\n";

    std::vector<Pixel> palette;
    palette.reserve(p_lists.size());
    //Percorre todas as listas pegando o elemento mediano ( pos(size/2) )

    std::vector<Pixel> sub_lista;
    for (unsigned int i=0; i<p_lists.size(); i++){
        sub_lista = p_lists.at(i);
        palette.push_back( sub_lista.at(sub_lista.size()/2) ); //adiciona a cor mediana de cada sub lista
    }

    std::cout << "Criou paleta\n";
}
