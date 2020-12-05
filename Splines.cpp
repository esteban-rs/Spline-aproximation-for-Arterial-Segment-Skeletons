#include <bits/stdc++.h>
#include "IMG_Processing.hpp"
#include "Splines.hpp"

Spline::Spline(PGM &myimage) {
    n = myimage.rows;
    m = myimage.cols;
    max_scale = myimage.max_scale;
}

void Spline::LinealSpline(IMG_Processing &myprocessing) {
    SplinePoints.clear();

    vector <vector <int>> tmp;
    for (int i = 0; i < myprocessing.Simplification.size();  i++){
        lines(myprocessing.Simplification[i], tmp); 
        SplinePoints.push_back(tmp);
        tmp.clear();        
    }
}

void Spline::lines(vector<vector<int>> &spline, vector<vector<int>> &tmp) {
    // Guarda posición (x_1,y_1) y (x_2, y_2)
    int x_1 = 0;
    int y_1 = 0;
    int x_2 = 1;
    int y_2 = 1;
    
    float m   = 0;
    int pivot = 0;

    vector <int> local = {0,0};

    // Dibujo segmentos p1-p2, p2-p3, ..., pn-p1
    for (int i = 0; i < spline.size()-1; i++){
        //tomo puntos p_i - p_{i+1}
        x_1 = spline[i][0];
        y_1 = spline[i][1];
        x_2 = spline[i+1][0];
        y_2 = spline[i+1][1];
		// |x_2 - x_1 | > |y_2 - y_1 |
		if(abs(x_2 - x_1) > abs(y_2 - y_1)){
            // Pendiente indefinida
    	    if (x_1 == x_2){
                local[0] = x_1;
                if(y_1 < y_2){
                    for (int y = y_1; y <= y_2; y++){
                        local[1] = y;
                        tmp.push_back(local);
                    }
                }
                // y_1 > y_2
		        else {
                    for (int y = y_2; y <= y_1; y++){
                        local[1] = y;
                        tmp.push_back(local);
                    }
                }
		    }		
            else if (x_1 < x_2){
                m     = (float) (y_2 - y_1)/(x_2 - x_1);

                for (int x = x_1; x <= x_2; x++){
                    pivot = (int) (m*(x-x_1) + y_1);
                    local[0] = x;
                    local[1] = pivot;
                    
                    tmp.push_back(local);
                    //myimage.Image[x][pivot] = myimage.max_scale/2;
                }
            }
            else {
                m     = (float) (y_2 - y_1)/(x_2 - x_1);

                for (int x = x_2; x <= x_1; x++){
                    pivot = (int) (m*(x-x_1) + y_1);
                    local[0] = x;
                    local[1] = pivot;
                    
                    tmp.push_back(local);
                    //myimage.Image[x][pivot] = myimage.max_scale/2;
                }
            }	
		}
		// |x_2 - x_1 | <= |y_2 - y_1 |
        else {
            // Pendiente indefinida
            if (y_1 == y_2){
                local[1] = y_1;
                if(x_1 < x_2){
                    for (int x = x_1; x <= x_2; x++){
                        local[0] = x;
                        tmp.push_back(local);

                        //myimage.Image[x][y_1] = myimage.max_scale/2;
                    }
                }
                // x_1 > x_2
                else {
                    for (int x = x_2; x <= x_1; x++){
                        local[0] = x;
                        tmp.push_back(local);
                        // myimage.Image[x][y_1] = myimage.max_scale/2;
                    }
                }
            }		
            else if (y_1 < y_2){
                m     = (float) (x_2 - x_1)/(y_2 - y_1);

                for (int y = y_1; y < y_2; y++){
                    pivot = (int) (m*(y-y_1)+x_1);
                    local[0] = pivot;
                    local[1] = y;
                    tmp.push_back(local);

                    //myimage.Image[pivot][y]  = myimage.max_scale/2;
                }
            }
            // y_1 > y_2
            else {
                m     = (float) (x_2 - x_1)/(y_2 - y_1);

                for (int y = y_2; y < y_1; y++){
                    pivot = (int) (m*(y-y_1)+x_1);

                    local[0] = pivot;
                    local[1] = y;
                    tmp.push_back(local);
                    //myimage.Image[pivot][y]=myimage.max_scale/2;
                }
            }		            
        }
    }
}

void Spline::WriteSpline(string filename) {
    vector < vector <int >>  Image;

    vector <int> tmp ={0,0};
    Image.assign(n, vector<int> (m,0));
    for (int i = 0; i < SplinePoints.size(); i++) {
        for (int j = 0; j < SplinePoints[i].size(); j++) {
            tmp = SplinePoints[i][j];
            Image[tmp[0]][tmp[1]] = max_scale;
        }
    }
    
    ofstream file(filename);

    // Comienza escritura
    file << "P2" <<endl;
    file << "# SPLINE CREATED BY ESTEBANRS" << endl;
    file << m << " " << n << endl;
    file << max_scale << endl;

    // Escribe valores
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            file << Image[i][j] << endl;
        }
    }
    file.close();

    cout << "¡Hurra! Se escribió Spline en " << filename << endl;
}

double Spline::Haussdorf_Distance_Single(vector <vector <int>> &myspline, vector <vector<int>> &segment)  {
    // temporal points
    point p1(0,0);
    point p2(0,0);
    double inf = 10000;

    // Get d1
    double d1 = 0;

    for (int i = 0; i < segment.size(); i++) {
        // Fix a point
        p1.x = segment[i][0];
        p1.y = segment[i][1];

        // Set a big distance
        inf = 100000;
        for (int j = 0; j < myspline.size(); j++) {
            p2.x = myspline[j][0];
            p2.y = myspline[j][1];

            Line myline(p1,p2);
            d1 = myline.longitud();
            if (d1 < inf) {
                inf = d1;
            }
        }
        if (inf > d1) {
            d1 = inf;
        }
    }

    // get d2
    inf = 10000;
    double d2 = 0;
    
    for (int i = 0; i < myspline.size(); i++) {
        // Fix a point
        p1.x = myspline[i][0];
        p1.y = myspline[i][1];

        // Set a big distance
        inf = 100000;
        for (int j = 0; j < segment.size(); j++) {
            p2.x = segment[j][0];
            p2.y = segment[j][1];

            Line myline(p1,p2);
            d2 = myline.longitud();
            if (d2 < inf) {
                inf = d2;
            }
        }
        if (inf > d1) {
            d2 = inf;
        }
    }

    double h = max(d1,d2);

    return h;
    

}

void Spline::Hausdorff_Distance(IMG_Processing &myprocessing) {
    cout << "** Hausdorff_Distances **" << endl;
    if (myprocessing.Segments.size() < 1) {
        cout << "Obtenga puntos spline primero." << endl;
        return;
    }

    double h = 0;
    for (int i = 0; i < myprocessing.Segments.size(); i++) {
        h = Haussdorf_Distance_Single(SplinePoints[i], myprocessing.Segments[i]);
        cout << h << endl;
    }
}
