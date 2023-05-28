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
        Triangolo(unsigned int& identificatore, Punto& p1, Punto& p2, Punto& p3, Lato& lat1, Lato& lat2, Lato& lat3);
        //Triangolo(unsigned int& identificatore, Punto& p1, Punto& p2, Punto& p3, unsigned int& idlato);
        //Triangolo(unsigned int& identificatore, Punto& p, Lato& lat, unsigned int& idlato);
        //Triangolo(unsigned int& identificatore, Lato& lat1, Lato& lat2, unsigned int& idlato);
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
        vector<Triangolo> _listaTriangoli;
        vector<Lato> _listaLati;
        vector<Punto> _listaPunti;
        //MatrixXd<unsigned int> _adiacenza;
    public:
        Mesh(const vector<Punto>& listaPunti);
        Mesh(){}
        array<unsigned int, 2> DentroMesh(const Punto p);
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
