#ifndef __DELAUNAY_H
#define __DELAUNAY_H


#include "Eigen/Eigen"
#include <iostream>
#include <cmath>
#include <fstream>
#include <Eigen/Dense>

#include <queue>
#include <map>

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
        // la tolleranza voglio poterla cambiare abbastanza liberamente
        static constexpr double geometricTol = 1.0e-12;
        static constexpr double geometricTol_Squared = max_tolerance(Punto::geometricTol * Punto::geometricTol,
                                                                     numeric_limits<double>::epsilon());
        Punto(unsigned int& id, double& x, double& y);
        Punto(const Punto& p);
        Punto(){}
        string Show();
    };

    inline double normSquared(const double& x, const double& y)
    {
      return x * x + y * y;
    }

    inline bool operator==(const Punto& p1, const Punto& p2)
    {
      return (normSquared(p1._x - p2._x, p1._y - p2._y) <=
              Punto::geometricTol * Punto::geometricTol *
              max(normSquared(p1._x, p1._y), normSquared(p2._x, p2._y)));
    }

    inline bool operator!=(const Punto& p1, const Punto& p2)
    {
      return !(p1 == p2);
    }


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
        unsigned int _id;
        vector<Lato> _lati;
    public:
        bool CheckConnection(const Punto& a, const Punto& b, Lato*& l);
        Triangolo(unsigned int& identificatore, Punto& p1, Punto& p2, Punto& p3, unsigned int& idlato);
        Triangolo(const Triangolo& triang);
        Triangolo(){}
        double CalcolaAngolo(const Lato& segm);
        void OrdinamentoAntiorario();

        string Show();
    };

    struct


    class Mesh
    {
    friend class IOMesh;
    protected:
        list<Triangolo> _listaTriangoli;
        list<Lato> _listaLati;
        list<Punto> _listaPunti;
        // un triangolo è adiacente al più a 3 triangoli
        vector <array <Triangolo *>> _adiacenza;
    public:
        Mesh(const vector<Punto>& listaPunti);
        Mesh(){}
        array<unsigned int, 2> DentroMesh(const Punto& p, Triangolo* dentro_triang);
        void ControlloDelaunay(Triangolo& triang);
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
