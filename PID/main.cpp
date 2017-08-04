#include <omp.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <array>
#include "PIDMath.h"
#include "pixel.h"
#include "image.h"

#define OMP_THREADS 4

void test7(){
    std::string name = "Tucano.bmp";
    BMP imag = BMP();
    imag.readFromFile(name);
    MBT newImag = MBT(imag);
    newImag.writeToFile("out.mbt");
    newImag.readFromFile("out.mbt");
    newImag.changeColorSpace(MBT::ColorSpace::YUV);
    Quantization q;
    std::vector<Pixel> palette = q.medianCut2ndAnd3rdChannels(&newImag, 256);
    newImag.setPalette(palette);
    imag = newImag.constructBMP();
    imag.writeToFile("out.bmp");

    return;
}

int main(int argc, char *argv[]) {
    if (omp_get_max_threads() > OMP_THREADS)
        omp_set_num_threads(OMP_THREADS);

    if(argc >= 2){
        std::string file(argv[1]);

        BMP bitmap = BMP();
        bitmap.readFromFile(file);
        MBT mapaBit = MBT(bitmap);
        mapaBit.writeToFile("PID_OUT.mbt");
        mapaBit.readFromFile("PID_OUT.mbt");
        mapaBit.changeColorSpace(MBT::ColorSpace::YUV);
        Quantization q;
        std::vector<Pixel> palette = q.medianCut2ndAnd3rdChannels(&mapaBit, 256);
        mapaBit.setPalette(palette);
        bitmap = mapaBit.constructBMP();
        bitmap.writeToFile("PID_OUT.bmp");
    } else {
        std::cout<<"Passe uma string com o nome do arquivo BMP em TrueColor, sem compressao\n";
    }

    //Ou faz o teste manualmente
    //test7();

    return 0;
}

