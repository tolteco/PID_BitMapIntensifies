#include <omp.h>
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

#include <cstring>

//F: http://www.geeksforgeeks.org/counting-sort/
void countSort(char arr[]){
    #define RANGE 256

    // The output character array that will have sorted arr
    char output[strlen(arr)];

    // Create a count array to store count of inidividul
    // characters and initialize count array as 0
    int countt[RANGE + 1], i;
    memset(countt, 0, sizeof(countt));

    // Store count of each character
    for(i = 0; arr[i]; ++i)
        ++countt[arr[i]];

    // Change count[i] so that count[i] now contains actual
    // position of this character in output array
    for (i = 1; i <= RANGE; ++i)
        countt[i] += countt[i-1];

    // Build the output character array
    for (i = 0; arr[i]; ++i)
    {
        output[countt[arr[i]]-1] = arr[i];
        --countt[arr[i]];
    }

    // Copy the output array to arr, so that arr now
    // contains sorted characters
    for (i = 0; arr[i]; ++i)
        arr[i] = output[i];
}

#include <map>
Image Quantization::medianCut2ndAnd3rdChannels(Image img, unsigned short maxColors){
    unsigned int lines = img.getLines();
    unsigned int cols = img.getColumns();

    //first Channel
    char fChnl [lines*cols];
    char sChnl [lines*cols];
    char tChnl [lines*cols];
    for(unsigned int i=0; i<lines; i++){
        for (unsigned int j=0; j<cols; j++){
            fChnl[i*cols+j] = img.getPixel(i, j).get1st();
            sChnl[i*cols+j] = img.getPixel(i, j).get2nd();
            tChnl[i*cols+j] = img.getPixel(i, j).get3rd();
        }
    }
    countSort(fChnl); countSort(sChnl); countSort(tChnl);

    unsigned int range1 = fChnl[(lines*cols)-1] - fChnl[0];
    unsigned int range2 = sChnl[(lines*cols)-1] - sChnl[0];
    unsigned int range3 = tChnl[(lines*cols)-1] - tChnl[0];



}
