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
    
    // Remove gray pixels
    FilteredPGM();
}

void PGM::FilteredPGM(){
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            if (Image[i][j] < max_scale/2){
                Image[i][j] = 0;
            }
            else {
                Image[i][j] = max_scale;   
            }
        }
    }
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

void PGM::GetMaxMin(){
    int MAXID = FiguresID[0][0];
    int MINID = FiguresID[0][0];
    int MAX   = FiguresID[0][1];
    int MIN   = FiguresID[0][1];

    for (int i = 1; i < FiguresID.size(); i++){
        if (FiguresID[i][1] > MAX){
            MAXID = FiguresID[i][0];
            MAX   = FiguresID[i][1];
        }
        if (FiguresID[i][1] < MIN){
            MINID = FiguresID[i][0];
            MIN   = FiguresID[i][1];         
        }
    }

    for (int i = 0; i <rows; i++){
        for (int j = 0; j < cols; j++){
            if (Figures[i][j] == MAXID || Figures[i][j] == MINID){
                Figures[i][j] = max_scale;
            }
            else{
                Figures[i][j] = 0;
            }
        }
    }
}

void PGM::WritePGM_MM(string filename) {
    //GetMaxMin();
    ofstream file(filename);
    // Comienza escritura
    file << "P2"<<endl;
    file << "# CREATED BY ESTEBANRS" << endl;
    file << rows << " " << cols << endl;
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

void PGM::GetConvexSet(){
    // Asigno memoria a matrix Image
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
            if (Image[i][j] != 0 && Figures[i][j] == 0) {
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
}

void PGM::PrintFigures(){
    cout << "** F i g u r e s **" << endl;
    for (int i = 0; i < FiguresID.size(); i++){
        cout << "[" << FiguresID[i][0] << "] " << FiguresID[i][1]<< endl;
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

