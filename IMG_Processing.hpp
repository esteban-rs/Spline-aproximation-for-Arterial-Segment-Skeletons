#ifndef IMG_PROCESSING_HPP
#define IMG_PROCESSING_HPP

#include "PGM_Images.hpp"
#include <queue>

using namespace std;

class point {
    friend class Line;
    friend class triangle;
    friend class IMG_Processing;

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
        double m;
        double a;
        double b;
        double c;
    public :
        Line(point _p1, point _p2);

        double longitud();
        double distance(point p);
};


class triangle {
    private:
        point p1;
        point p2;
        point p3;

        double myarea;
    public:  
        triangle (point _p1, point _p2, point _p3);
        double area ();
};

class IMG_Processing {
    friend class PGM;
    private:
        int max_gray_scale = 0;

        vector <vector<int>> ImageCopy;
        vector<point>  PointsList;

        // Segmentation
        void get_bifurcations(PGM &myimage);
        void fixBifurcations(PGM &myimage);
        void fixsegmentation(PGM &myimage);
        int fixgrayneiborns(int i, int j, PGM &myimage);
        void CheckExtendedNeiborns(int i, int j, int id, PGM &myimage);

        // sort
        void SortSegementation(PGM &myimage);
        void SpecialNeiborns(int i, int j, int id, queue<point> &myqueue, PGM &myimage);
        void SpecialExtendedNeiborns (int i, int j, int id, queue<point> &myqueoue, PGM &myimage);
        point find_extreme( int i, PGM &image);
        void endpoints(PGM &myimage);


        // Douglas 
        double perpendiculardistance(vector <int> &p, vector<int> &lineStar, vector<int> &lineEnd);
        void lines(vector<vector<int>> &spline, PGM &myimage);
        void DouglasPeucker(vector <vector <int>> &segment,  double epsilon,vector <vector<int>> &out);

        // Visvaligram
        int visvaligram_search (vector < vector <int>> &myvector, double epsilon);
        void Visvaligram_local(vector <vector<int>> &segment,vector <vector<int>> &local, double epsilon);

    public: 
        IMG_Processing(PGM &myimage);
        void Segmentation(PGM &myimage);
        void RamerDouglasPeucker(PGM &myimage, double epsilon);
        void Visvaligram(PGM &myimage, double epsilon);

        // Splines
};

#endif