#include <bits/stdc++.h>
#include "IMG_Processing.hpp"


void IMG_Processing::get_bifurcations(PGM &myimage) {
    int neiborns = 0;
    int end_point = 0;
    int bifurcations = 0;

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
                    Bifurcations.push_back(point(i,j,1));
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
}

void IMG_Processing::SaveSegmentation(PGM &myimage) {
    Segments.clear();
    // Add pixels to a point list
    vector <vector <int>> points;
    vector <int> mypoint = {0,0};

    for (int k = 0; k < myimage.FiguresID.size(); k++) {
        for (int i = 0; i < myimage.rows; i++) {
            for (int j = 0; j< myimage.cols; j++) {
                if (myimage.Figures[i][j] == myimage.FiguresID[k][0]) {
                    mypoint[0] = i;
                    mypoint[1] = j;
                    points.push_back(mypoint);
                }
            }
        }
        Segments.push_back(points);
        points.clear();
    } 
}

void IMG_Processing::Segmentation(PGM &myimage) {
    get_bifurcations(myimage);

    // Fix neiborns for bifurcations
    fixBifurcations(myimage);
    
    // Update matrix
    myimage.Image = ImageCopy;

    // Get convex figures
    myimage.GetConvexSet();

    // Save it in IMG_Processing class
    SaveSegmentation(myimage);
    
    // Add Bifurcations to segments
    fixsegmentation(myimage);
    SortSegementation(myimage);
    endpoints(myimage);


}

void IMG_Processing::endpoints(PGM &myimage) {
    /* Add final bifurcation at the end of segment list */
    vector <int> tmp = {0,0};
    for (int i = 0; i < Segments.size(); i++) {
       tmp = Segments[i][Segments[i].size() -1 ];
       CheckExtendedNeiborns(tmp[0], tmp[1], i, myimage);
    }
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
        for (int i = 0; i < Bifurcations.size(); i++) {
        // Reviso todos los casos
        if (Bifurcations[i].y-1 >= 0){
            ImageCopy[Bifurcations[i].x][Bifurcations[i].y-1] = 0;
        }
        if (Bifurcations[i].x -1 >= 0){
            ImageCopy[Bifurcations[i].x-1][Bifurcations[i].y] = 0;
        }
        if (Bifurcations[i].x - 1 >= 0 && Bifurcations[i].y - 1 >= 0){
            ImageCopy[Bifurcations[i].x -1][Bifurcations[i].y-1] = 0;
        }
        if (Bifurcations[i].x-1 >= 0 && Bifurcations[i].y+1 <= myimage.cols){
            ImageCopy[Bifurcations[i].x-1][Bifurcations[i].y+1] = 0;
        }
        if (Bifurcations[i].x  + 1 <= myimage.rows && Bifurcations[i].y-1 >= 0){
            ImageCopy[Bifurcations[i].x +1][Bifurcations[i].y -1] = 0;
        }
        if (Bifurcations[i].y + 1 <= myimage.cols){
            ImageCopy[Bifurcations[i].x][Bifurcations[i].y+1] = 0;
        }
        if (Bifurcations[i].x +1 <= myimage.rows){
            ImageCopy[Bifurcations[i].x +1][Bifurcations[i].y] = 0;
        }
        if (Bifurcations[i].x + 1 <= myimage.rows && Bifurcations[i].y + 1 <= myimage.cols){
            ImageCopy[Bifurcations[i].x+1][Bifurcations[i].y+1] = 0;
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
            for (int k = 0; k < Bifurcations.size(); k++){
                if (i+x == Bifurcations[k].x && y+j == Bifurcations[k].y) {
                    tmp[0] = i+x;
                    tmp[1] = j+y;
                    Segments[id].push_back(tmp);
                }
            }
        }
    }

}

void IMG_Processing::RamerDouglasPeucker(PGM &myimage, double epsilon) {

    Simplification.clear();
    vector <vector <int>> out;

    for (int i = 0; i < Segments.size(); i++) {
        vector <vector <int>> out;
        DouglasPeucker(Segments[i],epsilon, out);
        Simplification.push_back(out);
        out.clear();
    }
}

void IMG_Processing::DouglasPeucker(vector <vector <int>> &segment, double epsilon, vector <vector<int>> &out) {
    if(segment.size() < 2) {
        cout << "Not enough points." << endl;
        return;
    }
    // Get point with maximum distance from line [star,end]
	int index = 0;
	int end   = segment.size() - 1;

	double dmax = 0;
    double d    = 0;
	for(int i = 1; i < end; i++) {
		d = perpendiculardistance(segment[i], segment[0], segment[end]);
		if (d > dmax) {
			index = i;
			dmax  = d;
		}
	}

    // Recursive call if distance is greater that epsilon
	if(dmax > epsilon) {
		vector<vector <int>> Part1;
		vector<vector <int>> Part2;
		
        vector<vector <int>> first(segment.begin(), segment.begin()+index+1);
		vector<vector <int>> last(segment.begin()+index, segment.end());
		
        DouglasPeucker(first, epsilon, Part1);
		DouglasPeucker(last, epsilon, Part2);
 
		// Merge results
		out.assign(Part1.begin(), Part1.end()-1);
		out.insert(out.end(), Part2.begin(), Part2.end());
		
        if(out.size() < 2) {
            cout << "Output corrupted." << endl;
            return;
        }
	} 
	else {
        // succesfull simplification, return two points
        out.clear();
		out.push_back(segment[0]);
		out.push_back(segment[end]);
	}
}

void IMG_Processing::lines(vector<vector<int>> &spline, PGM &myimage) {
    // Guarda posición (x_1,y_1) y (x_2, y_2)
    int x_1 = 0;
    int y_1 = 0;
    int x_2 = 1;
    int y_2 = 1;
    
    float m   = 0;
    int pivot = 0;

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
                if(y_1 < y_2){
                    for (int y = y_1; y <= y_2; y++){
                        myimage.Image[x_1][y] = myimage.max_scale/2;
                    }
                }
                // y_1 > y_2
		        else {
                    for (int y = y_2; y <= y_1; y++){
                        myimage.Image[x_1][y] = myimage.max_scale/2;
                    }
                }
		    }		
            else if (x_1 < x_2){
                m     = (float) (y_2 - y_1)/(x_2 - x_1);

                for (int x = x_1; x <= x_2; x++){
                    pivot = (int) (m*(x-x_1) + y_1);
                        
                    myimage.Image[x][pivot] = myimage.max_scale/2;
                }
            }
            else {
                m     = (float) (y_2 - y_1)/(x_2 - x_1);

                for (int x = x_2; x <= x_1; x++){
                    pivot = (int) (m*(x-x_1) + y_1);
                      
                    myimage.Image[x][pivot] = myimage.max_scale/2;
                }
            }	
		}
		// |x_2 - x_1 | <= |y_2 - y_1 |
        else {
            // Pendiente indefinida
            if (y_1 == y_2){
                if(x_1 < x_2){
                    for (int x = x_1; x <= x_2; x++){
                        myimage.Image[x][y_1] = myimage.max_scale/2;
                    }
                }
                // x_1 > x_2
                else {
                    for (int x = x_2; x <= x_1; x++){
                        myimage.Image[x][y_1] = myimage.max_scale/2;
                    }
                }
            }		
            else if (y_1 < y_2){
                m     = (float) (x_2 - x_1)/(y_2 - y_1);

                for (int y = y_1; y < y_2; y++){
                    pivot = (int) (m*(y-y_1)+x_1);

                    myimage.Image[pivot][y]  = myimage.max_scale/2;
                }
            }
            // y_1 > y_2
            else {
                m     = (float) (x_2 - x_1)/(y_2 - y_1);

                for (int y = y_2; y < y_1; y++){
                    pivot = (int) (m*(y-y_1)+x_1);
                    myimage.Image[pivot][y]=myimage.max_scale/2;
                }
            }		            
        }
    }
}

void IMG_Processing::SortSegementation(PGM &myimage) {
    vector <vector < vector <int>>> sorted_segments;
    vector < vector <int>> sorted_segment;
    /* sort segments in shape order  */
    vector <int> tmp = {0,0};
    point p(0,0,-1);

    // find first bifurcation
    for (int i = 0; i < Segments.size(); i++) {
        // Find Bifurtation
        p = find_extreme(i,myimage);
        tmp[0] = p.x;
        tmp[1] = p.y;
        // add points    
        sorted_segment.push_back(tmp);

        queue <point> myqueue;
        // Find first segment element
        SpecialExtendedNeiborns(p.x,p.y,i+1,myqueue, myimage);
        point q(0,0,0);
        while (!myqueue.empty()) {
            q = myqueue.front();
            SpecialNeiborns(q.x,q.y,i+1,myqueue,myimage);
            tmp[0] = q.x;
            tmp[1] = q.y;
            sorted_segment.push_back(tmp);
            myqueue.pop();
        }
        sorted_segments.push_back(sorted_segment);
        sorted_segment.clear();
    }
    // Set space for new order
    Segments.clear();
    // copy sorted elements
    for (int i = 0; i < sorted_segments.size(); i++) {
        Segments.push_back(sorted_segments[i]);
    }
}

point IMG_Processing::find_extreme( int i, PGM &image) {
    vector <int> tmp = {0,0};
    point p(0,0,-1);
    for (int j = 0; j < Segments[i].size(); j++) {
        tmp = Segments[i][j];
        for (int k = 0; k < Bifurcations.size(); k++) {
            if (tmp[0] == Bifurcations[k].x && tmp[1] == Bifurcations[k].y ){
                p.x = tmp[0];
                p.y = tmp[1];
                p.label = 1;
                return p;
            }
        }
    }

    return p;
}

void IMG_Processing::SpecialNeiborns(int i, int j, int id, queue<point> &myqueue, PGM &myimage) {
    point p(0,0,0);
    
    // Reviso todos los casos
    if (j -1 >= 0){
        if (myimage.Figures[i][j-1] == id){
            p.x = i;
            p.y = j-1;
            myqueue.push(p);
            myimage.Figures[p.x][p.y] = 0;
        }
    }
    if (i -1 >= 0){
        if (myimage.Figures[i-1][j] == id ){
            p.x = i-1;
            p.y = j;
            myqueue.push(p);
            myimage.Figures[p.x][p.y] = 0;
        }
    }
    if (i - 1 >= 0 && j - 1 >= 0){
        if (myimage.Figures[i-1][j-1] == id){
            p.x = i-1;
            p.y = j-1;
            myqueue.push(p);
            myimage.Figures[p.x][p.y] = 0;
        }
    }
    if (i - 1 >= 0 && j + 1 <= myimage.cols){
        if (myimage.Figures[i-1][j+1] == id){
            p.x = i-1;
            p.y = j+1;
            myqueue.push(p);
            myimage.Figures[p.x][p.y] = 0;
        }
    }
    if (i + 1 <= myimage.rows && j - 1 >= 0){
        if (myimage.Figures[i+1][j-1] == id ){
            p.x = i+1;
            p.y = j-1;
            myqueue.push(p);
            myimage.Figures[p.x][p.y] = 0;

        }
    }
    if (j + 1 <= myimage.cols){
        if (myimage.Figures[i][j+1] == id){
            p.x = i;
            p.y = j+1;
            myqueue.push(p);
            myimage.Figures[p.x][p.y] = 0;
        }
    }
    if (i + 1 <= myimage.rows){
        if (myimage.Figures[i+1][j] == id){
            p.x = i+1;
            p.y = j;
            myqueue.push(p);
            myimage.Figures[p.x][p.y] = 0;
        }
    }
    if (i + 1 <= myimage.rows && j + 1 <= myimage.cols){
        if (myimage.Figures[i+1][j+1] == id){
            p.x = i+1;
            p.y = j+1;
            myqueue.push(p);
            myimage.Figures[p.x][p.y] = 0;
        }
    }
}

void IMG_Processing::SpecialExtendedNeiborns (int i, int j, int id, queue<point> &myqueoue, PGM &myimage) {
    /* Add bifurcations to segments bacause
       in the segmentation they was ignored */
    point p(0,0,-1);
    for (int x = -2; x <= 2; x++) {
        for (int y = -2; y <= 2; y++) {
            if (i+x >= 0 && j+y >= 0 && i+x < myimage.rows && j+y < myimage.cols){
                if (myimage.Figures[i+x][j+y] == id) {
                    p.x = i+x;
                    p.y = j+y;
                    myqueoue.push(p);
                    myimage.Figures[i+x][j+y] = 0;
                }
            }
        }
    }
}

int IMG_Processing::visvaligram_search (vector < vector <int>> &myvector, double epsilon) {
    double area_m = 2*epsilon;

    double a = 0;
    int index = -1;

    int total = myvector.size();

    for (int i = 1; i < total -1; i++) {
        point p1(myvector[i-1][0],myvector[i-1][1]);
        point p2(myvector[i][0],myvector[i][1]);
        point p3(myvector[i+1][0],myvector[i+1][1]);

        triangle mytriangle(p1,p2,p3);
        a = mytriangle.area();
        if (a < area_m) {
            area_m = a;
            index = i;
        }
    }
    if (area_m < epsilon) {
            return index;
    }
    else {
        return -1;
    }
}

void IMG_Processing::Visvaligram_local(vector <vector<int>> &segment, vector <vector<int>> &local, double epsilon) {
    // Create a local copy
    for (int i = 0; i < segment.size(); i++) {
        local.push_back(segment[i]);
    }

    if (local.size() < 4) {
        return;
    }

    int i = 1;
    while (i != -1) {
        i = visvaligram_search(local,epsilon);

        if (i != -1) {
            local.erase(local.begin() + i);
        }
    }
}

void IMG_Processing::Visvaligram(PGM &myimage, double epsilon) {
    Simplification.clear();

    vector <vector <int>> local;
    for (int i = 0; i < Segments.size(); i++) {
        Visvaligram_local(Segments[i], local, epsilon);
        Simplification.push_back(local);
        local.clear();
    }
}
