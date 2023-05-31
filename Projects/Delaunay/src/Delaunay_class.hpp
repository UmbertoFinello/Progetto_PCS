#ifndef __DELAUNAY_H
#define __DELAUNAY_H

#include "Eigen/Eigen"
#include <iostream>
#include <cmath>
#include <fstream>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

namespace ProjectLibrary
{

    class Punto
    {
    friend class Lato;
    friend class Triangolo;
    friend class Mesh;
    friend class IOMesh;
    friend double crossProduct(Punto& p1, Punto& p2);
    protected:
        int _id;
        double _x;
        double _y;
    public:
        Punto(unsigned int& id, double& x, double& y);
        Punto(const Punto& p);
        Punto(){}
        string Show();
    };


    class Lato
    {
    friend class Triangolo;
    friend class Mesh;
    friend class IOMesh;
    protected:
        unsigned int _id;
        Punto _p1;
        Punto _p2;
        double _length;
        array<unsigned int,2> _listIdTr;
    public:
        Lato(unsigned int& id , Punto& p1, Punto& p2, unsigned int& idtr);
        Lato(const Lato& lat);
        Lato(){}
        string Show();
    };

    class Triangolo
    {
    friend class IOMesh;
    friend class Mesh;
    protected:
        unsigned int _id;
        array<Punto, 3> _vertici;
        array<Lato, 3> _lati;
    public:
        array<unsigned int,2> CheckConnection(const Punto& a, const Punto& b, vector<Lato>*& veclat);
        Triangolo(unsigned int& identificatore, const Punto& p1, const Punto& p2, const Punto& p3, unsigned int& idlato,
                  vector<Lato>*& vlat);
        Triangolo(const Triangolo& triang);
        Triangolo(){}
        double CalcolaAngolo(const Lato& segm);
        void OrdinamentoAntiorario();
        string Show();
    };


    class Mesh
    {
    friend class IOMesh;
    protected:
        vector<Triangolo> _listaTriangoli;
        vector<Lato> _listaLati;
        vector<Punto> _listaPunti;
        //MatrixXd<unsigned int> _adiacenza;
    public:
        Mesh(const vector<Punto>& listaPunti);
        Mesh(){}
        array<unsigned int, 2> DentroMesh(const Punto& p, Triangolo* triang);
        void ControlloDelaunay(Triangolo& triang);
    };


    class IOMesh
    {
        public:
            IOMesh(){}
            bool ImportPunti(vector<Punto>& listaPunti, const string& FileName);
            bool ExportMesh(const Mesh& mesh, const string& OutFilePath);
    };
   
    double crossProduct(const Punto& p1, const Punto& p2);

}

#endif // __DELAUNAY_H
