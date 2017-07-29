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
    if(lines%BLOCK_SIZE==0 && columns%BLOCK_SIZE==0){
        int lda = ROUND_UP(columns, BLOCK_SIZE);
        int ldb = ROUND_UP(lines, BLOCK_SIZE);
        transpose_block(src, dst, lines, columns, lda, ldb, BLOCK_SIZE); //Block tem que ser múltiplo, 3*5=15
    } else {
        transpose(src, dst, lines, columns);
    }
}

void multiply(Pixel* mat, const unsigned int lines,     const unsigned int columns,
            double* mult, const unsigned int multLines, const unsigned int multColumns){


}

