#ifndef Splines_HPP
#define Splines_HPP

using namespace std;

class Spline {
    private:
        int n = 0;
        int m = 0;
        int max_scale = 0;
        vector <vector <vector <int>>> SplinePoints;


        void lines(vector<vector<int>> &spline, vector<vector<int>> &tmp);
        double Haussdorf_Distance_Single(vector <vector <int>> &myspline,vector <vector <int>> &segment);


    public:
        Spline(PGM &myimage);
        void LinealSpline(IMG_Processing &myprocessing);

        void Hausdorff_Distance(IMG_Processing &myprocessing);
        void WriteSpline(string filename);

};


#endif