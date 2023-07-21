#include "Delaunay_class.hpp"
#include <chrono>

using namespace ProjectLibrary;

int main()
{
    auto start = chrono::steady_clock::now();

    vector<Punto> List_punti;
    string inputfile = "../Delaunay/Dataset/Test1.csv";
    if(!(ImportPunti(List_punti, inputfile)))
        return -1;
    else{
        Mesh griglia = Mesh(List_punti);
        cout << griglia.Show();
        if(!(griglia.ExportMesh("../Delaunay/Result/", "Test1.csv")))
            return -2;
    }
    auto end = chrono::steady_clock::now();

    cout << "Elapsed time in microseconds: "
         << chrono::duration_cast<chrono::microseconds>(end - start).count()
         << endl;

    return 0;
}
