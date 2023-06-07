#include "Delaunay_class.hpp"

using namespace ProjectLibrary;

int main()
{
    IOMesh IO = IOMesh();
    vector<Punto> List_punti;

    if(!(IO.ImportPunti(List_punti,"./Dataset/Test1.csv")))
        return -1;
    else{
        Mesh mesh = Mesh(List_punti);
        //Mesh.Show();
        if(!(IO.ExportMesh(mesh, "./Result/")))
            return -2;
    }
    return 0;


}
