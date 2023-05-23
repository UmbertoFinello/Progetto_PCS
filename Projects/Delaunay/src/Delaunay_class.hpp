#ifndef __DELAUNAY_H
#define __DELAUNAY_H

#include "Eigen/Eigen"
#include <iostream>
#include <fstream>

using namespace std;
using namespace Eigen;

namespace ProjectLibrary
{
  class Punto
  {
    protected:
      unsigned int _id;
      double _x;
      double _y;

    public:
      Punto(unsigned int& id , double& x, double& y);
      Punto(const Punto& p);
  };

  class Lato
  {
    unsigned int _id;
    Punto _p1;
    Punto _p2;
    double _length;
    public:
      Lato(unsigned int& id , Punto& p1, Punto& p2);
  };

  class Mesh
  {
    protected:
        vector<Triangolo> _listaTriangoli;
        vector<Lato> _listaLati;
        vector<Punto> _listaPunti;
        MatrixXd<unsigned int> _adiacenza;
    public:
        Mesh(const vector<Punto>& listaPunti , string OutFilePath);
  };

  class IOMesh
  {
    public:
        vector<Punto> ImportMesh(const string& FileName);
        void ExportMesh(const Mesh& mesh);
  };
}

#endif // __DELAUNAY_H
