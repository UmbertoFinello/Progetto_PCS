#include "Delaunay_class.hpp"


int main()
{
<<<<<<< HEAD
  //string InputFileName = "./Test1.csv";


  //InputFileName = "./Test2.csv";


  return 0;
=======
    ProjectLibrary::IOmesh IO = IOmesh();
    Vector<Punti> List_punti;
    if(!(IO.ImportMesh(List_punti,'./Dataset/Test1.csv')))
        return -1;
    else{
        ProjectLibrary::Mesh mesh = Mesh(List_punti);
        mesh.Show();
        if(!(IO.ExportMesh(mesh, './Result/')))
            return -2;
    }
    return 0;
>>>>>>> main
}
