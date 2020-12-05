#ifndef IMG_PROCESSING_HPP
#define IMG_PROCESSING_HPP

#include "PGM_Images.hpp"
#include "Geometry.hpp"
#include <queue>

using namespace std;


class IMG_Processing: public point {
    friend class PGM;
    friend class Spline;
    private:
        int max_gray_scale = 0;

        vector <vector<int>>          ImageCopy;
        vector<point>                 Bifurcations;
        vector<vector< vector <int>>> Segments;
        vector<vector< vector <int>>> Simplification;

        // Segmentation
        void get_bifurcations(PGM &myimage);
        void fixBifurcations(PGM &myimage);
        void fixsegmentation(PGM &myimage);
        int fixgrayneiborns(int i, int j, PGM &myimage);
        void CheckExtendedNeiborns(int i, int j, int id, PGM &myimage);
        void SaveSegmentation(PGM &myimage);

        // sort
        void SortSegementation(PGM &myimage);
        void SpecialNeiborns(int i, int j, int id, queue<point> &myqueue, PGM &myimage);
        void SpecialExtendedNeiborns (int i, int j, int id, queue<point> &myqueoue, PGM &myimage);
        point find_extreme( int i, PGM &image);
        void endpoints(PGM &myimage);

        // Douglas 
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
};

#endif