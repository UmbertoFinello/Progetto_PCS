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
    public:
        Lato(unsigned int& id , Punto& p1, Punto& p2);
        Lato(const Lato& lat);
        Lato(){}
        string Show();
    };

    class Triangolo
    {
    friend class IOMesh;
    friend class Mesh;
    protected:
        vector<Punto> _vertici;
        int _id;
        vector<Lato> _lati;
    public:
        bool CheckConnection(const Punto& a, const punto& b, Lato*& l);
        Triangolo(){}
        Triangolo(const Triangolo& triang);
        double CalcolaAngolo(const Lato& segm);
        void OrdinamentoAntiorario();
        string Show();
    };


    class Mesh
    {
    friend class IOMesh;
    protected:
        list<Triangolo> _listaTriangoli;
        list<Lato> _listaLati;
        list<Punto> _listaPunti;
        //MatrixXd<unsigned int> _adiacenza;
    public:
        Mesh(const vector<Punto>& listaPunti);
        Mesh(){}
        array<unsigned int, 2> DentroMesh(const Punto& p, Triangolo* dentro_triang);
    };


    class IOMesh
    {
        public:
            IOMesh(){}
            bool ImportPunti(vector<Punto>& listaPunti, const string& FileName);
            bool ExportMesh(const Mesh& mesh, const string& OutFilePath);
    };
   


}

#endif // __DELAUNAY_H
