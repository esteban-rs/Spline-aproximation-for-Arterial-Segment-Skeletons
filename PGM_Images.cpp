#include <bits/stdc++.h>
#include "PGM_Images.hpp"

PGM::PGM(string filename) {
    ifstream file(filename);

    if (file.is_open() == 0){
        cerr << "No existe el archivo." <<endl;
        exit(-1);
    }

    string line;              // Variable local
    string word;
    /* **** C o m i e n z a  l e c t u r a **** */                 
    getline(file, line);
    if (line.compare("P2") != 0){
        cout << "Formato no válido." << endl;
    }

    getline(file, line);
    while (line[0] == '#'){  // Salto comentarios
        getline(file, line);
    }
    /* *********** A l o c a c i o n ********** */                 
    stringstream ss(line);  // Obtengo dimensiones
    
    getline(ss,word,' ');
    cols = stoi(word); 
    
    getline(ss,word,' ');     
    rows = stoi(word);

    Image.assign(rows, vector<int> (cols,0));
    
    file >> max_scale;

    /* ******* L e e  C o n t e n i d o ******* */                 
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            file >> Image[i][j];
        }
    }

    file.close();
}

void PGM::WritePGM(string filename) {
    ofstream file(filename);
    // Comienza escritura
    file << "P2"<<endl;
    file << "# CREATED BY ESTEBANRS" << endl;
    file << cols << " " << rows << endl;
    file << max_scale << endl;

    // Escribe valores
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            file << Image[i][j] << endl;
        }
    }
    file.close();

    cout << "¡Hurra! Se escribió imagen en " << filename << endl;
}

void PGM::WritePGM_Figures(string filename) {
    //GetMaxMin();
    ofstream file(filename);
    // Comienza escritura
    file << "P2"<<endl;
    file << "# CREATED BY ESTEBANRS" << endl;
    file << cols << " " << rows << endl;
    file << max_scale << endl;

    // Escribe valores
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            file << Figures[i][j] << endl;
        }
    }
    file.close();

    cout << "¡Hurra! Se escribió imagen en " << filename << endl;
}

int PGM::CheckNeibors(int i, int j){
    int pixels = 0;
    // Reviso todos los casos
    if (j -1 >= 0){
        if (Image[i][j-1] != 0){
            pixels++;
        }
    }
    if (i -1 >= 0){
        if (Image[i-1][j] != 0 ){
            pixels++;
        }
    }
    if (i - 1 >= 0 && j - 1 >= 0){
        if (Image[i-1][j-1] != 0){
            pixels++;

        }
    }
    if (i - 1 >= 0 && j + 1 <= cols){
        if (Image[i -1][j+1] != 0){
            pixels++;

        }
    }
    if (i + 1 <= rows && j - 1 >= 0){
        if (Image[i+1][j-1] != 0 ){
            pixels++;

        }
    }
    if (j + 1 <= cols){
        if (Image[i][j+1] != 0){
            pixels++;

        }
    }
    if (i + 1 <= rows){
        if (Image[i+1][j] != 0){
                        pixels++;

        }
    }
    if (i + 1 <= rows && j + 1 <= cols){
        if (Image[i+1][j+1] != 0){

            pixels++;
        }
    }
    
    return pixels;
}

void PGM::GetConvexSet() {
    Figures.assign(rows, vector<int> (cols,0));

    queue<vector<int>> MyQueue;// Cola para pixeles de figura

    vector <int> position;     // Posicion Pixel Figura
    position.assign(2,0);

    vector <int> FindSet;      // Figura Encontrada
    FindSet.assign(2,0);       // [0] id [1] size

    int figure_id   = 1;
    int figure_size = 0;
    int tmp = 0;
    // Recorro Imagen
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            // Reviso si hay figura y no se ha marcado
            if (Image[i][j] == max_scale && Figures[i][j] == 0) {
                // Agrego punto a cola
                position[0] = i;
                position[1] = j;
                MyQueue.push(position);
                // Actualizo tamaño
                figure_size++;
                // Marco Pixel con indice
                Figures[i][j] = figure_id;
                
            
                while (!MyQueue.empty()){
                    position = MyQueue.front();
                    MyQueue.pop();
                    figure_size += CheckNeibors(position,figure_id,MyQueue);
                }
                // Guardo indice y tamaño
                FindSet[0] = figure_id;
                FindSet[1] = figure_size;
                FiguresID.push_back(FindSet);
                // Actualizo indice y reinicio tamaño
                figure_id++; 
                figure_size = 0;
            }           
        } //EndFor j
    } // EndFor i
    position.clear();
    FindSet.clear();
    convex = Figures.size();
    

    // Add pixels to a point list
    vector <vector <int>> points;
    vector <int> mypoint = {0,0};

    for (int k = 0; k < FiguresID.size(); k++) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j< cols; j++) {
                if (Figures[i][j] == FiguresID[k][0]) {
                    mypoint[0] = i;
                    mypoint[1] = j;
                    points.push_back(mypoint);
                }
            }
        }
        Segmentation.push_back(points);
        points.clear();
    }
    
}

void PGM::Skeletonization(){
    // Vector to add to changes
    vector <int> tmp = {0,0};
    vector<vector<int>>  Changes;

    // Check current changes
    int changes1 = 1;
    int changes2 = 1;
    while (changes1 || changes2){
        changes1 = changes2 = 0;
        // First Condition 
        for (int i = 1; i < rows -1; i++) {
            for (int j = 1; j < cols -1; j++) {
                if (Image[i][j] == max_scale) {
                    if (condition1(i,j) == 1) {
                        changes1++;
                        tmp[0] = i;
                        tmp[1] = j;
                        Changes.push_back(tmp);
                    }
                }
            }
        } 
        // Change color in original image
        for (int i = 0; i < Changes.size(); i++) {
            Image[Changes[i][0]][Changes[i][1]] = 0;
        }
        Changes.clear();
        
        // Second Condition
        for (int i = 1; i < rows -1; i++) {
            for (int j = 1; j < cols -1; j++) {
                if (Image[i][j] == max_scale) {
                    if (condition2(i,j) == 1) {
                        changes2++;
                        tmp[0] = i;
                        tmp[1] = j;
                        Changes.push_back(tmp);
                    }
                }
            }
        }
        // Change color in original image
        for (int i = 0; i < Changes.size(); i++) {
            Image[Changes[i][0]][Changes[i][1]] = 0;
        }
        Changes.clear();  
    }

}

int PGM::A_transitions(int i, int j) {
    int transitions = 0;
    /*  
    Eight Neibors Transtitions 0-> 255
        j-1___j___j+1
    i-1 |P9   P2  P3|
    i   |P8   P1  P4|
    i+1 |P7___P6__P5|
    in sequence P2,P3,P4,P5,P6,P7,P8,P9,P2
    */
    // P2 TO P3
    if (Image[i-1][j] == 0 && Image[i-1][j+1] == max_scale) {
        transitions++;
    }
    // P3 TO P4
    if (Image[i-1][j+1] == 0 && Image[i][j+1] == max_scale) {
        transitions++;
    }
    // P4 TO P5
    if (Image[i][j+1] == 0 && Image[i+1][j+1] == max_scale) {
        transitions++;
    }
    // P5 TO P6
    if (Image[i+1][j+1] == 0 && Image[i+1][j] == max_scale) {
        transitions++;
    }
    // P6 TO P7
    if (Image[i+1][j] == 0 && Image[i+1][j-1] == max_scale) {
        transitions++;
    }
    // P7 TO P8
    if (Image[i+1][j-1] == 0 && Image[i][j-1] == max_scale) {
        transitions++;
    }
    // P8 TO P9
    if (Image[i][j-1] == 0 && Image[i-1][j-1] == max_scale) {
        transitions++;
    }
    // P9 TO P2
    if (Image[i-1][j-1] == 0 && Image[i-1][j] == max_scale) {
        transitions++;
    }

    return transitions;
}

int PGM::B_Neiborns(int i, int j) {
    int pixels = 0;
    /*  Sum of white pixels
        j-1___j___j+1
    i-1 |P9   P2  P3|
    i   |P8   P1  P4|
    i+1 |P7___P6__P5|
    in sequence P2,P3,P4,P5,P6,P7,P8,P9
    */
    pixels += Image[i-1][j];
    pixels += Image[i-1][j+1];
    pixels += Image[i][j+1];
    pixels += Image[i+1][j+1];
    pixels += Image[i+1][j];
    pixels += Image[i+1][j-1];
    pixels += Image[i][j-1];
    pixels += Image[i-1][j-1];

    pixels = pixels/max_scale;

    return pixels;
}

int PGM::condition1(int i, int j) {
    /*  j-1___j___j+1
    i-1 |P9   P2  P3|
    i   |P8   P1  P4|
    i+1 |P7___P6__P5| */

    int flag = 0;

    // 2 <= B(P1) <= 6
    int bn = B_Neiborns(i,j);
    if (bn >= 2 && bn <= 6) {
        flag ++;
    }
    // A(P1) = 1
    if (A_transitions(i,j) == 1){
        flag ++;
    }
    // at least P2,P4 or P6 is cero
    if (Image[i-1][j]*Image[i][j+1]*Image[i+1][j] == 0) {
        flag ++;
    }
    // at least P4,P6 or P8 is cero
    if (Image[i][j+1]*Image[i+1][j]*Image[i][j-1] == 0) {
        flag ++;
    }

    if (flag == 4){
        // Yes
        return 1;
    }
    else {
        // Nope
        return 0;
    }
}

int PGM::condition2(int i, int j) {
    /*  j-1___j___j+1
    i-1 |P9   P2  P3|
    i   |P8   P1  P4|
    i+1 |P7___P6__P5| */
    
    int flag = 0;

    // 2 <= B(P1) <= 6
    int bn = B_Neiborns(i,j);
    if (bn >= 2 && bn <= 6) {
        flag ++;
    }
    // A(P1) = 1
    if (A_transitions(i,j) == 1){
        flag ++;
    }
    // at least P2,P4 or P8 is cero
    if (Image[i-1][j]*Image[i][j+1]*Image[i][j-1] == 0) {
        flag ++;
    }
    // at least P2,P6 or P8 is cero
    if (Image[i-1][j]*Image[i+1][j]*Image[i][j-1] == 0) {
        flag ++;
    }

    if (flag == 4){
        // Yes
        return 1;
    }
    else {
        // Nope
        return 0;
    }

}

void PGM::CheckLocalNeibors( int i, int j, int id, queue<vector<int>> &myQueue){
    // Vector para agregar elementos a cola
    vector <int> tmp;
    tmp.assign(2,0);
    // Guardo posicion
    tmp[0] = i;
    tmp[1] = j;
    // Agrego a cola
    myQueue.push(tmp);
    // Marco pixel
    Figures[i][j] = id;
    // Libero posicion temporal
    tmp.clear();
}

int PGM::CheckNeibors(vector <int> &indexed, int figure_id, queue<vector<int>> &myQueue){
    int i = indexed[0];
    int j = indexed[1];
    int neibors = 0;

    // Reviso todos los casos
    if (i >= 0 && j -1 >= 0){
        if (Image[i][j-1] != 0 && Figures[i][j-1] == 0){
            CheckLocalNeibors(i,j-1,figure_id,myQueue);
            // Agrego pixel a tamaño
            neibors++;
        }
    }

    if (i -1 >= 0 && j >= 0){
        if (Image[i-1][j] != 0 && Figures[i-1][j] == 0){
            CheckLocalNeibors(i-1,j,figure_id,myQueue);
            // Agrego pixel a tamaño
            neibors++;
        }
    }

    if (i - 1 >= 0 && j - 1 >= 0){
        if (Image[i-1][j-1] != 0 && Figures[i-1][j-1] == 0){
            CheckLocalNeibors(i-1,j-1,figure_id,myQueue);
            // Agrego pixel a tamaño
            neibors++;
        }
    }

    if (i - 1 >= 0 && j + 1 < cols){
        if (Image[i -1][j+1] != 0 && Figures[i-1][j+1] == 0){
            CheckLocalNeibors(i-1,j+1,figure_id,myQueue);
            // Agrego pixel a tamaño
            neibors++;
        }
    }

    if (i + 1 < rows && j - 1 >= 0){
        if (Image[i+1][j-1] != 0 && Figures[i+1][j-1] == 0){
            CheckLocalNeibors(i+1,j-1,figure_id,myQueue);
            // Agrego pixel a tamaño
            neibors++;
        }
    }

    if (i >= 0 && j + 1 < cols){
        if (Image[i][j+1] != 0 && Figures[i][j+1] == 0){
            CheckLocalNeibors(i,j+1,figure_id,myQueue);
            // Agrego pixel a tamaño
            neibors++;
        }
    }

    if (i + 1 < rows && j >= 0){
        if (Image[i+1][j] != 0 && Figures[i+1][j] == 0){
            CheckLocalNeibors(i+1,j,figure_id,myQueue);
            // Agrego pixel a tamaño
            neibors++;
        }
    }

    if (i + 1 < rows && j + 1 < cols){
        if (Image[i+1][j+1] != 0 && Figures[i+1][j+1] == 0){
            CheckLocalNeibors(i+1,j+1,figure_id,myQueue);
            // Agrego pixel a tamaño
            neibors++;
        }
    }
    return neibors;
}

void PGM::PrintFigures(){
    cout << "** F i g u r e s **" << endl;
    for (int i = 0; i < FiguresID.size(); i++){
        cout << "[" << FiguresID[i][0] << "] " << FiguresID[i][1]<< endl;
        //cout << "[" << i << "] " << Segmentation[i].size() << endl;
    }

}