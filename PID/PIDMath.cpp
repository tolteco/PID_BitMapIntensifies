#include <omp.h>
#include <math.h>
#include <iostream>
#include <algorithm>
//#include <xmmintrin.h>
#include "PIDMath.h"
#include "pixel.h"

///MATRIX OP
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

void MatrixOperation::multiply(double mult[3][3], Pixel* mat, const unsigned int columns){
    //A * B, sendo A e B matrizes. ColsA=LinhasB AND LinsA=LinsB, repetindo a op "ColsB" vezes
    // Transpose B, multiply, transpose B
    unsigned int multLines = 3, multColumns = 3;

    Pixel* toTranspose = (Pixel*)_mm_malloc(sizeof(Pixel)*multLines*columns, 64);
    std::copy(mat, mat+(multLines*columns), toTranspose);
    transposeMatrix(mat, toTranspose, multColumns, columns);

    ///Multiplication

    //aaaaaaaaaaaaaaa

    ///End Multiplication

    transposeMatrix(mat, toTranspose, columns, multColumns);

}

void MatrixOperation::multiplySum(double mult[3][3], double sum[3], Pixel* mat, const unsigned int columns){
    //A * B, sendo A e B matrizes. ColsA=LinhasB AND LinsA=LinsB, repetindo a op "ColsB" vezes
    // Transpose B, multiply, transpose B
    unsigned int multLines = 3, multColumns = 3;

    Pixel* toTranspose = (Pixel*)_mm_malloc(sizeof(Pixel)*multLines*columns, 64);;
    std::copy(mat, mat+(multLines*columns), toTranspose);
    transposeMatrix(mat, toTranspose, multColumns, columns);

    ///Multiplication

    unsigned char Y,U,V,
                  R,G,B;
    Pixel temp_pixel;
    #pragma omp parallel
    for (unsigned int i=0; i<multLines; i++){
        for (unsigned int j=0; j<columns; j++){ // O(n^2) intensifies
            //temp_pixel = mat[i*columns+j]; //Normal
            temp_pixel = mat[j*multLines+i]; //Transposta
            R = temp_pixel.get1st(); G = temp_pixel.get2nd(); B = temp_pixel.get3rd();
            Y = ( (  66 * R + 129 * G +  25 * B + 128) >> 8) +  16;
            U = ( ( -38 * R -  74 * G + 112 * B + 128) >> 8) + 128;
            V = ( ( 112 * R -  94 * G -  18 * B + 128) >> 8) + 128;

            mat[j*multLines+i] = Pixel(Y, U, V);
        }
    }

    ///End Multiplication

    transposeMatrix(mat, toTranspose, columns, multColumns);
}

///MISC MATH
unsigned char MiscMath::lookUpPalette(std::vector<Pixel> palette, Pixel color){
    for(unsigned char i=0; i<palette.size(); i++){
        if(palette.at(i) == color) return i;
    }
    return 0;
}

unsigned int MiscMath::roundUpToNearestMultiple(unsigned int num, unsigned int multiple){
    if (multiple == 0)
        return num;

    int remainder = num % multiple;
    if (remainder == 0)
        return num;

    return num + multiple - remainder;
}

///QUANTIZATION
#include <vector>
#include <algorithm>

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

std::vector<Pixel> Quantization::medianCut2ndAnd3rdChannels(Image* img, unsigned short maxColors){
    unsigned int lines = img->getLines();
    unsigned int cols = img->getColumns();

    std::vector<Pixel> pixels;
    pixels.reserve(lines*cols);
    for(unsigned int i=0; i<lines; i++){
        for (unsigned int j=0; j<cols; j++){
            pixels.push_back(img->getPixel(i,j));
        }
    }
std::cout << "Pixels postos no primeiro vector\n";

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

std::cout << "Divisão completa. B: " << p_lists.size() << "\n";

    std::vector<Pixel> palette;
    palette.reserve(p_lists.size());
    //Percorre todas as listas pegando o elemento mediano ( pos(size/2) )

    std::vector<Pixel> sub_lista;
    for (unsigned int i=0; i<p_lists.size(); i++){
        sub_lista = p_lists.at(i);
        palette.push_back( sub_lista.at(sub_lista.size()/2) ); //adiciona a cor mediana de cada sub lista
    }

    #pragma omp parallel for
    for(unsigned int i=0; i<lines; i++){
        for (unsigned int j=0; j<cols; j++){
            Pixel p = img->getPixel(i,j);
            //std::cout << "Pixel save. " << p << "\n";
            for (unsigned int k=0; k<p_lists.size(); k++){
                sub_lista = p_lists.at(k);
                //std::cout<< k << "\n";
                if(std::find(sub_lista.begin(), sub_lista.end(), p) != sub_lista.end()){
                    img->setPixel(palette.at(k), i, j);
                    //std::cout << "Encontrado na lista: " << k << "Mapeado para: " << palette.at(k) << "\n";
                    k=p_lists.size(); //Break this if and inner for
                }
            }
        }
    }

    std::cout << "Mapeamento de pixels completo.\n";

    //img->setBitsPerColor(8);
    //Image ret = Image(img);
    return palette;
}
