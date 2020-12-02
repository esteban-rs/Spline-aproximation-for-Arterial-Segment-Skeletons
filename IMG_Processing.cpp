#include <bits/stdc++.h>
#include "IMG_Processing.hpp"

point::point() {
    x = 0;
    y = 0;
}

point::point(int _x, int _y) {
    x     = _x;
    y     = _y;
    label = 0;
}

point::point(int _x, int _y, int _label) {
    x     = _x;
    y     = _y;
    label = _label;
}

Line::Line(point _p1, point _p2) {
    p1 = _p1;
    p2 = _p2;
}

double Line::longitud() {
    return sqrt((p1.x - p2.x)*(p1.x - p2.x) - (p1.y - p2.y)*(p1.y - p2.y));
}

void IMG_Processing::get_bifurcations(PGM &myimage) {
        int neiborns = 0;
    int end_point = 0;
    int bifurcations = 0;
    
    // Find Bifurcations
    for (int i = 0; i < myimage.rows; i++) {
        for(int j = 0; j < myimage.cols; j++) {
            if (ImageCopy[i][j] == max_gray_scale) {
                neiborns = myimage.CheckNeibors(i,j);
                if (neiborns == 1) {
                    end_point++;
                }
                if (myimage.A_transitions(i,j) > 2){
                    ImageCopy[i][j] = 50;
                    // Fix if gray pixel has a gray neiborn
                    if (fixgrayneiborns(i,j,myimage) == 1){
                        ImageCopy[i][j] = 0;
                        continue;
                    }
                    bifurcations++;
                    PointsList.push_back(point(i,j,1));
                }


            }
        }
    } 
    
    cout << "Puntos Límite: " << end_point << endl;
    cout << "Bifurcaciones: " << bifurcations << endl;
}

IMG_Processing::IMG_Processing(PGM &myimage) {
    // Copy Image
    max_gray_scale = myimage.max_scale;

    ImageCopy.assign(myimage.rows, vector<int> (myimage.cols,0));

    for (int i = 0; i < myimage.rows; i++) {
        for (int j = 0; j < myimage.cols; j++){
            ImageCopy[i][j] = myimage.Image[i][j];
        }
    }

    get_bifurcations(myimage);

    // Fix neiborns for bifurcations

    fixBifurcations(myimage);
    
    // Update matrix
    myimage.Image = ImageCopy;

    // Get convex figures
    myimage.GetConvexSet();
    
    // Add Bifurcations to segments
    fixsegmentation(myimage);

}

int IMG_Processing::fixgrayneiborns(int i, int j, PGM &myimage) {
    // search if a gray pixel has a gray neiborn
    if (i-1 >= 0) {
        if (ImageCopy[i-1][j] == 50) {
            return 1;
        }
    }
    if (j+1 <= myimage.cols) {
        if (ImageCopy[i][j+1] == 50) {
            return 1;
        }
    }
    if (i+1 <= myimage.rows) {
        if (ImageCopy[i+1][j] == 50) {
            return 1;
        }
    }
    if (j-1 >= 0) {
        if (ImageCopy[i][j-1] == 50) {
            return 1;
        }
    }
    return 0;
    
}

void IMG_Processing::fixBifurcations(PGM &myimage) {
        for (int i = 0; i < PointsList.size(); i++) {
        // Reviso todos los casos
        if (PointsList[i].y-1 >= 0){
            ImageCopy[PointsList[i].x][PointsList[i].y-1] = 0;
        }
        if (PointsList[i].x -1 >= 0){
            ImageCopy[PointsList[i].x-1][PointsList[i].y] = 0;
        }
        if (PointsList[i].x - 1 >= 0 && PointsList[i].y - 1 >= 0){
            ImageCopy[PointsList[i].x -1][PointsList[i].y-1] = 0;
        }
        if (PointsList[i].x-1 >= 0 && PointsList[i].y+1 <= myimage.cols){
            ImageCopy[PointsList[i].x-1][PointsList[i].y+1] = 0;
        }
        if (PointsList[i].x  + 1 <= myimage.rows && PointsList[i].y-1 >= 0){
            ImageCopy[PointsList[i].x +1][PointsList[i].y -1] = 0;
        }
        if (PointsList[i].y + 1 <= myimage.cols){
            ImageCopy[PointsList[i].x][PointsList[i].y+1] = 0;
        }
        if (PointsList[i].x +1 <= myimage.rows){
            ImageCopy[PointsList[i].x +1][PointsList[i].y] = 0;
        }
        if (PointsList[i].x + 1 <= myimage.rows && PointsList[i].y + 1 <= myimage.cols){
            ImageCopy[PointsList[i].x+1][PointsList[i].y+1] = 0;
        }
    }
}

void IMG_Processing::fixsegmentation(PGM &myimage) {
    point p(0,0,-1);

    vector <int> tmp ={0,0};

    for (int k = 0; k < myimage.FiguresID.size(); k++) {
        for (int i = 0; i < myimage.rows; i++) {
            for (int j = 0; j < myimage.cols; j++) {
                if (myimage.Figures[i][j] == myimage.FiguresID[k][0]){
                    // Add closed bifurcation to a segment
                    CheckExtendedNeiborns(i,j,k,myimage);
                }
            }
        }
    }
}

void IMG_Processing::CheckExtendedNeiborns(int i, int j, int id, PGM &myimage) {
    /* Add bifurcations to segments bacause
       in the segmentation they was ignored */
    point p(0,0,-1);
    vector <int> tmp = {0,0};
    for (int x = -2; x <= 2; x++) {
        for (int y = -2; y <= 2; y++) {
            for (int k = 0; k < PointsList.size(); k++){
                if (i+x == PointsList[k].x && y+j == PointsList[k].y) {
                    tmp[0] = i+x;
                    tmp[1] = j+y;
                    myimage.Segmentation[id].push_back(tmp);
                }
            }
        }
    }

}

void IMG_Processing::DouglasPeucker(double epsilon, PGM &myimage) {
     
}