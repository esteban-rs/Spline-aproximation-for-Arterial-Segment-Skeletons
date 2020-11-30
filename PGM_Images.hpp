#ifndef PGM_HPP
#define PGM_HPP

using namespace std;

class PGM {
    private:
        int rows   = 0;
        int cols   = 0;
        vector<vector<int>>  Image;    // Guarda Imagen Original
        vector<vector<int>>  Changes;  // Enumero figuras
        
        void FilteredPGM();            // Filtra pixeles grises
        void GetMaxMin();              // Escribe Max y Min en Figures

        // Conjuntos Conexos
        int CheckNeibors(int i, int j);

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

        // Write Files
        void WritePGM(string filename);   // Escribe imagen original
};

#endif
