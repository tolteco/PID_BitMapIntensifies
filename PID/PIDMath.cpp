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
        transpose_block(src, dst, lines, columns, lda, ldb, BLOCK_SIZE); //Block tem que ser múltiplo, 3*5=15
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
void quick_sort(std::vector<unsigned char>* f_arr, std::vector<unsigned char>* s_arr, std::vector<unsigned char>* t_arr, int beg, int end, char chnl){
        unsigned char pivot;
        if      (chnl == 0) pivot = f_arr->at(beg + (end-beg)/2);
        else if (chnl == 1) pivot = s_arr->at(beg + (end-beg)/2);
        else                pivot = f_arr->at(beg + (end-beg)/2);

        int begC = beg,
            endC = end;

        while(begC <= endC){
            unsigned char b_p, e_p;
            if      (chnl == 0){ b_p = f_arr->at(begC); e_p = f_arr->at(endC);}
            else if (chnl == 1){ b_p = s_arr->at(begC); e_p = s_arr->at(endC);}
            else{                b_p = t_arr->at(begC); e_p = t_arr->at(endC);}

            while(b_p < pivot) ++begC;
            while(e_p > pivot) --endC;

            if (begC <= endC){
                iter_swap(f_arr->begin() + begC, f_arr->begin() + endC);
                iter_swap(s_arr->begin() + begC, s_arr->begin() + endC);
                iter_swap(t_arr->begin() + begC, t_arr->begin() + endC);
                ++begC;
                --endC;
            }
        }

        if (beg < endC) quick_sort(f_arr, s_arr, t_arr, beg, endC, chnl);
        if (begC < end) quick_sort(f_arr, s_arr, t_arr, begC, end, chnl);
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

char chnl_list_with_greatest_range(std::vector<std::vector<unsigned char>> arr1, std::vector<std::vector<unsigned char>> arr2){
    int range1 = chnl_range(arr1.at(0)),
        range2 = chnl_range(arr2.at(0));

    int rangeTemp1, rangeTemp2;

    return 5;
    /*for (int i=1; i<arr1.size(); i++){

    }*/
}

Image Quantization::medianCut2ndAnd3rdChannels(Image img, unsigned short maxColors){
    unsigned int lines = img.getLines();
    unsigned int cols = img.getColumns();

    std::vector<unsigned char> chnl_1, chnl_2, chnl_3;
    chnl_1.reserve(lines*cols); chnl_2.reserve(lines*cols); chnl_3.reserve(lines*cols);
    //temp second Channel
    for(unsigned int i=0; i<lines; i++){
        for (unsigned int j=0; j<cols; j++){
            Pixel p = img.getPixel(i,j);
            chnl_1.push_back(p.get1st());
            chnl_2.push_back(p.get2nd());
            chnl_3.push_back(p.get3rd());
        }
    }
    ///http://blog.isamert.net/median-cut-algorithm-in-qt-c/

    //http://www.cplusplus.com/forum/general/833/
    std::vector<std::vector<unsigned char>> f_lists, s_lists, t_lists;
    f_lists.push_back(chnl_1);
    s_lists.push_back(chnl_2);
    t_lists.push_back(chnl_3);

    while(f_lists.size() < maxColors){ //Main loop
        //char biggest_range = chnl_with_greatest_range()
        unsigned int lists_size = f_lists.size(); //Para não reavaliar e dividir listas igualitariamente
        for (unsigned int i=0; i<lists_size; i++){ //Para todas as listas, verifica qual tem o maior range e ordena
            char b_range = chnl_with_greatest_range(s_lists.at(i), t_lists.at(i)); //Canal com maior range
            quick_sort(&f_lists.at(i), &s_lists.at(i), &t_lists.at(i), 0, f_lists.size()-1, b_range); //Sort de acordo com o canal

            std::vector<unsigned char> temp = f_lists.at(i);
            std::vector<unsigned char> f_2(temp.begin(), temp.begin() + temp.size()/2),
                                       f_3(temp.begin() + temp.size()/2, temp.end());
            f_lists.erase(f_lists.begin()+i); f_lists.push_back(f_2); f_lists.push_back(f_3);

            std::vector<unsigned char> temp2 = s_lists.at(i);
            std::vector<unsigned char> t_2(temp2.begin(), temp2.begin() + temp2.size()/2),
                                       t_3(temp2.begin() + temp2.size()/2, temp2.end());
            s_lists.erase(s_lists.begin()+i); s_lists.push_back(s_2); f_lists.push_back(s_3);

            std::vector<unsigned char> temp3 = t_lists.at(i);
            std::vector<unsigned char> t_2(temp3.begin(), temp3.begin() + temp3.size()/2),
                                       t_3(temp3.begin() + temp3.size()/2, temp3.end());
            t_lists.erase(t_lists.begin()+i); t_lists.push_back(t_2); t_lists.push_back(t_3);
        }
    }

    std::vector<Pixel> palette;
    palette.reserve(maxColors);
    //Percorre todas as listas pegando o elemento mediano ( pos(size/2) )

}
