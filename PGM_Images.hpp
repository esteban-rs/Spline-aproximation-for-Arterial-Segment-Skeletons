#ifndef PGM_HPP
#define PGM_HPP

using namespace std;

class PGM {
    friend class IMG_Processing;
    private:
        int rows   = 0;
        int cols   = 0;
        vector<vector<int>>  Image;    // Original Image
        vector<vector<int>>  Figures;  // Segmentation
        vector<vector< vector <int>>>  Segmentation;
        vector<vector< int>> FiguresID;// Info (id, size)
        
        int CheckNeibors(int i, int j);

        // Conjuntos Conexos
        int CheckNeibors(vector <int> &indexed, int figure_index, queue<vector<int>> &myQueue);
        void CheckLocalNeibors( int i, int j, int figure_index, queue<vector<int>> &myQueue);


        int A_transitions(int i, int j);
        int B_Neiborns(int i, int j);
        int condition1(int i, int j);
        int condition2(int i, int j);

    
    public:
        int convex    = 0;
        int max_scale = 0;

        PGM(string filename);
        // Conjuntos Conexos
        void GetConvexSet();
        void PrintFigures();

        // Skeletonization
        void Skeletonization();

        // Write File
        void WritePGM(string filename);
        void WritePGM_Figures(string filename);

};

#endif
