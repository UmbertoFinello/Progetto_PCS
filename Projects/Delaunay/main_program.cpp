#include "Delaunay_class.hpp"

using namespace ProjectLibrary;

int main()
{
    IOMesh IO = IOMesh();
    vector<Punto> List_punti;
    string inputfile = "../Delaunay/Dataset/Test1.csv";
    if(!(IO.ImportPunti(List_punti, inputfile)))
        return -1;
    else{
        Mesh mesh = Mesh(List_punti);
        string vedmesh = mesh.Show();
        cout << vedmesh;
        if(!(IO.ExportMesh(mesh, "../Delaunay/Result/", "Test1.csv")))
            return -2;
    }
    return 0;


}
