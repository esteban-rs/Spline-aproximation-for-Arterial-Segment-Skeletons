#include <bits/stdc++.h>
#include <time.h>
#include "PGM_Images.hpp"

int main(int argc, char const *argv[]){
    if (argc == 1 || argc > 2){
        cout << "Ingrese el nombre de un solo fichero. " << endl;
        exit(-1);
    }
    
    string name  = argv[1];
    string name1 = name.substr(8,8);

    PGM Image(name);

    Image.GetConvexSet();

    Image.WritePGM_MM("Out/trial.pgm");

    exit(0);

    


    return 0;
}