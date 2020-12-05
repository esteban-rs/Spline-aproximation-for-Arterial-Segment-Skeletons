#ifndef GEOMETRY_PROCESSING_HPP
#define GEOMETRY_PROCESSING_HPP

using namespace std;

class point {
    friend class Line;
    friend class triangle;
    friend class IMG_Processing;
    friend class Spline;

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


class Object_2D {
    private:
        int n_points;

        double area();
    public:
        vector <point> points;
        Object_2D();
        Object_2D(vector <point> &points);
        int get_num_point();

};

class Line: public Object_2D {
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
};


class triangle : public Object_2D {
    private:
        point p1;
        point p2;
        point p3;

        double myarea;
    public:  
        triangle (point _p1, point _p2, point _p3);
        virtual double area ();
};


double perpendiculardistance(vector <int> &p, vector<int> &star, vector<int> &end) ;
#endif