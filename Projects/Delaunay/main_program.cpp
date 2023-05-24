#include "Delaunay_class.hpp"

using namespace ProjectLibrary;

int main()
{
    IOMesh IO = IOMesh();
    vector<Punto> List_punti;
    if(!(IO.ImportMesh(List_punti,'./Dataset/Test1.csv')))
        return -1;
    else{
        Mesh Mesh = Mesh(List_punti);
        //Mesh.Show();
        if(!(IO.ExportMesh(Mesh, './Result/')))
            return -2;
    }
    return 0;

}
