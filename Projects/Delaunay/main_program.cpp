#include "Delaunay_class.hpp"

using namespace ProjectLibrary;

int main()
{
    vector<Punto> List_punti;
    string inputfile = "../Delaunay/Dataset/Test2.csv";
    if(!(ImportPunti(List_punti, inputfile)))
        return -1;
    else{
        Mesh griglia = Mesh(List_punti);
        cout << griglia.Show();
        if(!(griglia.ExportMesh("../Delaunay/Result/", "Test2.csv")))
            return -2;
    }
    return 0;
}
