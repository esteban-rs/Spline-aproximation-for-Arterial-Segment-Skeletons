#include <bits/stdc++.h>
#include <time.h>
#include "PGM_Images.hpp"
#include "IMG_Processing.hpp"

int main(int argc, char const *argv[]){
    if (argc == 1 || argc > 2){
        cout << "Ingrese el nombre de un solo fichero. " << endl;
        exit(-1);
    }
    
    string name  = argv[1];
    string name1 = name.substr(7,8);

    PGM Image(name);
    
    Image.Skeletonization();

    IMG_Processing processing(Image);


    Image.PrintFigures();

    Image.WritePGM("Out/" + name1);

    return 0;
}