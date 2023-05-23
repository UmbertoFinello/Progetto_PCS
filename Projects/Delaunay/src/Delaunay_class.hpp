#ifndef __DELAUNAY_H
#define __DELAUNAY_H

#include "Eigen/Eigen"
#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;
using namespace Eigen;

namespace ProjectLibrary
{

    class Punto
    {
    friend class Lato;
    friend class Triangolo;
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
    friend class Punto;
    friend class Triangolo;
    protected:
        unsigned int _id;
        Punto _p1;
        Punto _p2;
        double _length;
    public:
        Lato(unsigned int& id , Punto& p1, Punto& p2);

    };

    class Triangolo
    {
    friend class Punto;
    friend class Lato;
    protected:
        vector<Punto> _vertici;
        int _id;
        vector<Lato> _lati;
    public:
        Triangolo(vector<Punto>& v, unsigned int identificatore, const vector<Lato>& l);
        double CalcolaAngolo(const Lato& segm);
        void OrdinamentoAntiorario();
    };

    class Mesh
    {
    protected:
        vector<Triangolo> _listaTriangoli;
        vector<Lato> _listaLati;
        vector<Punto> _listaPunti;
        //MatrixXd<unsigned int> _adiacenza;
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
