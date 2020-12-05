#include <bits/stdc++.h>
#include "Geometry.hpp"



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

Object_2D:: Object_2D() {
    n_points = 0;
}

Object_2D:: Object_2D(vector <point> &_points) {
    for (int i = 0; i < _points.size(); i++) {
        points.push_back(_points[i]);
        n_points++;
    }
}

int Object_2D:: get_num_point() {
    return n_points;
}


Line::Line(point _p1, point _p2) {
    p1 = _p1;
    p2 = _p2;

    // Check vertical line
    if (p1.x == p2.x) {
        m = 0;
        a = 0;
        b = 1;
        c = p1.x;
    }
    else {
        m = (p1.y - p2.y)/(p1.x - p2.x);
        a = m;
        b = -1;
        c = -m*p1.x + p1.y;
    }
}

double Line::longitud() {
    return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

triangle::triangle(point _p1, point _p2, point _p3) {
    p1 = _p1;
    p2 = _p2;
    p3 = _p3;

    /* area with Heron formula */
    Line a(p1,p2);
    Line b(p2,p3);
    Line c(p3,p1);

    double s = (a.longitud()+b.longitud()+c.longitud())/2;
    myarea = sqrt(s * (s - a.longitud()) * (s - b.longitud()) * (s - c.longitud()) );

}

double triangle::area() {
    return myarea;
}

double perpendiculardistance(vector <int> &p, vector<int> &star, vector<int> &end) { 
    double dx = end[0] - star[0];
    double dy = end[1] - star[1];

    // Get distance and normalize
    double d = sqrt(dx*dx + dy*dy);
    if (d > 0) {
        dx = dx/d;
        dy = dy/d;
    }

    // Get proyectation
    double pvdot = dx*(p[0] - star[0]) + dy *(p[1] - star[1]);

    double ax = p[0] - star[0] - pvdot * dx;
    double ay = p[1] - star[1] - pvdot * dy;

    return sqrt(ax*ax + ay*ay);
}
