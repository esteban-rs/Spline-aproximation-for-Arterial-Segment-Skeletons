#include <bits/stdc++.h>
#include <time.h>
#include "PGM_Images.hpp"
#include "IMG_Processing.hpp"
#include "Splines.hpp"

int main(int argc, char const *argv[]){
    if (argc == 1 || argc > 2){
        cout << "Ingrese el nombre de un solo archivo. " << endl;
        exit(-1);
    }
    
    string name  = argv[1];
    stringstream ss(name);  // Obtengo dimensiones
    string name1;

    getline(ss,name1,'/');
    getline(ss,name1,'/');

    PGM Image(name);
    Image.Skeletonization();

    IMG_Processing processing(Image);
    processing.Segmentation(Image);

    processing.RamerDouglasPeucker(Image, 10);
    //processing.Visvaligram(Image, 50);

    Spline myspline(Image);
    myspline.LinealSpline(processing);
    myspline.WriteSpline("Out/Spline_"+ name1);
    myspline.Hausdorff_Distance(processing);
    // Image.PrintFigures();

    Image.WritePGM("Out/" + name1);


    return 0;
}