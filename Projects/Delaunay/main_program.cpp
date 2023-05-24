#include "Delaunay_class.hpp"


int main()
{
    ProjectLibrary::IOmesh IO = IOmesh();
    Vector<Punto> List_punti;
    if(!(IO.ImportMesh(List_punti,'./Dataset/Test1.csv')))
        return -1;
    else{
        ProjectLibrary::Mesh mesh = Mesh(List_punti);
        mesh.Show();
        if(!(IO.ExportMesh(mesh, './Result/')))
            return -2;
    }
    return 0;

}
