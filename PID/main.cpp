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

void test8(){
    MBT newImag = MBT();
    newImag.readFromFile("out.mbt");
    //newImag.changeColorSpace(MBT::ColorSpace::RGB);
    Quantization q;
    std::vector<Pixel> palette = q.medianCut2ndAnd3rdChannels(&newImag, 256);
    newImag.setPalette(palette);
    BMP imag = BMP();
    newImag.changeColorSpace(MBT::ColorSpace::RGB);
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
    test7();

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
