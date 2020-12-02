#ifndef IMG_PROCESSING_HPP
#define IMG_PROCESSING_HPP

#include "PGM_Images.hpp"
#include <queue>

using namespace std;

class point {
    friend class Line;
    friend class IMG_Processing;
    friend class PGM;
    
    private:
        int x = 0;
        int y = 0;
        int label = 0;
        // [0] endpoint
        // [1] bifurcation
    public :
        point(int _x, int _y);
        point (int _x, int _y, int _label);
        point ();
};

class Line {
    private:
        point p1;
        point p2;
    public :
        Line(point _p1, point _p2);
        double longitud();
};

class IMG_Processing {
    friend class PGM;
    private:
        int max_gray_scale = 0;
        vector <vector<int>> ImageCopy;
        vector<point>  PointsList;
        vector<point>  DHPointsList;
        vector<point>  VPointsList;

        double perpendicularDistance(point &mypoint, Line &myline);

        // Segmentation
        void get_bifurcations(PGM &myimage);
        void fixBifurcations(PGM &myimage);
        void fixsegmentation(PGM &myimage);
        int fixgrayneiborns(int i, int j, PGM &myimage);
        void CheckExtendedNeiborns(int i, int j, int id, PGM &myimage);

    public: 
        IMG_Processing(PGM &myimage);

        // Splines
        void DouglasPeucker(double epsilon, PGM &myimage);
};

#endif