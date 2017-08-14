#include <omp.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <array>
#include "PIDMath.h"
#include "pixel.h"
#include "image.h"
#include "mapadebits.h"
#include "bitmap.h"

#define OMP_THREADS 4

const std::string FULL_ARG = "-full";
const std::string WRITE_MBT_ARG = "-write";
const std::string READ_MBT_ARG = "-read";

void valid_input_text();
void full_op(std::string filename);
void write_op(std::string filename);
void read_op(std::string filename);

int main(int argc, char *argv[]) {
    if (omp_get_max_threads() > OMP_THREADS)
        omp_set_num_threads(OMP_THREADS);

    if(argc == 3){
        std::string option(argv[1]);
        std::string file(argv[2]);

        if (option == FULL_ARG){
            full_op(file);
        } else if (option == WRITE_MBT_ARG){
            write_op(file);
        } else if (option == READ_MBT_ARG){
            read_op(file);
        } else {
            valid_input_text();
        }
    } else if (argc == 2) {
        std::string file(argv[1]);
        full_op(file);
    } else {
        valid_input_text();
    }

    return 0;
}

void valid_input_text(){
    std::cout<<"Argumentos: [-ARG] + nome do arquivo de entrada. ARG=\n";
    std::cout<<"-full  : Escreve um arquivo MBT do BMP, le o arquivo, quantiza e escreve BMP de saida;\n";
    std::cout<<"-write : Escreve um arquivo MBT do BMP;\n";
    std::cout<<"-read  : Le um MBT, quantiza e escreve BMP de saida.\n";
    std::cout<<"\nSe for passado somente arquivo de entrada, o procedimento sera o mesmo de \"-full\".\n";
}

void full_op(std::string filename){
    BMP bitmap = BMP();
    bitmap.readFromFile(filename);
    MBT mapaBit = MBT(bitmap);
    mapaBit.writeToFile("PID_OUT.mbt");
    mapaBit.readFromFile("PID_OUT.mbt");
    mapaBit.changeColorSpace(MBT::ColorSpace::YUV);
    Quantization q;
    std::vector<Pixel> palette = q.medianCut2ndAnd3rdChannels(&mapaBit, 256);
    mapaBit.setPalette(palette);
    bitmap = mapaBit.constructBMP();
    bitmap.writeToFile("PID_OUT.bmp");
}

void write_op(std::string filename){
    BMP bitmap = BMP();
    bitmap.readFromFile(filename);
    MBT mapaBit = MBT(bitmap);
    mapaBit.writeToFile("PID_OUT.mbt");
}

void read_op(std::string filename){
    BMP bitmap = BMP();
    MBT mapaBit = MBT();
    mapaBit.readFromFile(filename);
    mapaBit.changeColorSpace(MBT::ColorSpace::YUV);
    Quantization q;
    std::vector<Pixel> palette = q.medianCut2ndAnd3rdChannels(&mapaBit, 256);
    mapaBit.setPalette(palette);
    bitmap = mapaBit.constructBMP();
    bitmap.writeToFile("PID_OUT.bmp");
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
