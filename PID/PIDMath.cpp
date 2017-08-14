#include <omp.h>
#include <math.h>
#include <iostream>
#include <algorithm>
//#include <xmmintrin.h>
#include "PIDMath.h"
#include "pixel.h"

///MISC MATH
unsigned char MiscMath::lookUpPalette(std::vector<Pixel> palette, Pixel color){
    for(unsigned short i=0; i<palette.size(); i++){
        if(palette.at(i) == color) return (unsigned char) i;
    }
    std::cout<<"Zero!\n";
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

static inline int pixel_distance(Pixel p1, Pixel p2) {
    return abs(p1.get1st() - p2.get1st()) +
           abs(p1.get2nd() - p2.get2nd()) +
           abs(p1.get3rd() - p2.get3rd());
}

static inline int px_mapper(Pixel pixel, const std::vector<Pixel> &pal) {
    int idx = 0;
    int current_distance = INT_MAX;
    for (unsigned int i=0; i<pal.size(); ++i) {
        int dist = pixel_distance(pixel, pal.at(i));
        if (dist < current_distance) {
            current_distance = dist;
            idx = i;
        }
    }
    return idx;
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

    //http://www.cplusplus.com/forum/general/833/
    std::vector<std::vector<Pixel>> p_lists;
    p_lists.push_back(pixels);
    while(p_lists.size() < maxColors){ //Main loop
        unsigned int lists_size = p_lists.size();
        for (unsigned int i=0; i<lists_size; lists_size--){
            char b_range = chnl_with_greatest_range(p_lists.at(i)); //Canal com maior range
            quick_sort(&p_lists.at(i), 0, p_lists.at(i).size()-1, b_range); //Sort de acordo com o canal

            std::vector<Pixel> temp = p_lists.at(i);
            std::vector<Pixel> f_2(temp.begin(), temp.begin() + temp.size()/2),
                               f_3(temp.begin() + temp.size()/2, temp.end());
            p_lists.erase(p_lists.begin()+i); p_lists.push_back(f_2); p_lists.push_back(f_3); //Adiciona novas listas no fim
        }
        #ifdef LOG_PRINT
            std::cout << "Iteracao da quantizacao completa. Cores:" << p_lists.size() << ".\n";
        #endif // LOG_PRINT
    }

    std::vector<Pixel> palette;
    palette.reserve(p_lists.size());
    //Percorre todas as listas pegando o elemento mediano ( pos(size/2) )

    std::vector<Pixel> sub_lista;
    for (unsigned int i=0; i<p_lists.size(); i++){
        sub_lista = p_lists.at(i);
        palette.push_back( sub_lista.at(sub_lista.size()/2) ); //adiciona a cor mediana de cada sub lista
    }

    Pixel p;
    #pragma omp parallel for private(p)
    for(unsigned int i=0; i<lines; i++){
        for (unsigned int j=0; j<cols; j++){
            p = img->getPixel(i,j);
            img->setPixel( palette[px_mapper(p, palette)], i, j);
        }
    }

    #ifdef LOG_PRINT
        std::cout << "Quantização completa.\n";
    #endif // LOG_PRINT

    //img->setBitsPerColor(8);
    //Image ret = Image(img);
    return palette;
}
