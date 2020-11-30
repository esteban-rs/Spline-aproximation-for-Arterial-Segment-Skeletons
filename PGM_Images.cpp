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
    int bn   = 0;

    // Pixel is black

    // 2 <= B(P1) <= 6
    bn = B_Neiborns(i,j);
    if (bn >= 2 && bn <=6) {
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
    int bn   = 0;
    // Pixel is already black

    // 2 <= B(P1) <= 6
    bn = B_Neiborns(i,j);
    if (bn >= 2 && bn <=6) {
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
        // Nop
        return 0;
    }

}

void PGM::Skeletonization(){
    // Vector to add to changes
    vector <int> tmp = {0,0};
    // Check current changes
    int _changes = 1;

    int contador = 0;
    while (_changes != 0) {
        _changes = 0;
        // First Condition 
        for (int i = 1; i < rows -1; i++) {
            for (int j = 1; j < cols -1; j++) {
                if (Image[i][j] == max_scale) {
                    if (condition1(i,j) == 1) {
                        tmp[0] = i;
                        tmp[1] = j;
                        Changes.push_back(tmp);
                        _changes++;
                    }
                }
            }
        } 
        // Change colors
        for (int i = 0; i < Changes.size(); i++) {
            Image[Changes[i][0]][Changes[i][1]] = 0;
        }
        Changes.clear(); 
        // Second Condition
        for (int i = 1; i < rows -1; i++) {
            for (int j = 1; j < cols -1; j++) {
                if (Image[i][j] == max_scale) {
                    if (condition2(i,j) == 1) {
                        tmp[0] = i;
                        tmp[1] = j;
                        Changes.push_back(tmp);
                        _changes++;
                    }
                }
            }
        }  
        // Change colors
        for (int i = 0; i < Changes.size(); i++) {
            Image[Changes[i][0]][Changes[i][1]] = 0;
        }
        Changes.clear(); 

    }
    
}
