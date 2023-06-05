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
    constexpr double max_tolerance(const double& x, const double& y)
    {
      return x > y ? x : y;
    }

    class Punto
    {
    public:
        unsigned int _id;
        double _x;
        double _y;

        static constexpr double geometricTol = 1.0e-12;
        static constexpr double geometricTol_Squared = max_tolerance(Punto::geometricTol * Punto::geometricTol,
                                                                     numeric_limits<double>::epsilon());
        Punto(unsigned int& id, double& x, double& y);
        Punto(const Punto& p);
        Punto(){}
        string Show();

        Punto& operator=(const Punto& p){_id = p._id; _x = p._x; _y = p._y; return *this;}
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
        vector<unsigned int> _listIdTr;
    public:
        Lato(unsigned int& id , Punto& p1, Punto& p2, unsigned int& idtr);
        Lato(const Lato& lat);
        Lato(){}
        string Show();

        Lato& operator=(const Lato& l){_id = l._id; _p1 = l._p1; _p2 = l._p2; _length = l._length;
            _listIdTr = l._listIdTr; return *this;}
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
        Triangolo(unsigned int& identificatore, const Punto& p1, const Punto& p2, const Punto& p3,
                  unsigned int& idlato, vector<Lato>*& vlat);
        Triangolo(const Triangolo& triang);
        Triangolo(){}
        array<unsigned int,2> CheckConnection(const Punto& a, const Punto& b, vector<Lato>*& veclat);
        double CalcolaAngolo(const Lato& segm);
        void OrdinamentoAntiorario();
        string Show();
        Triangolo& operator=(const Triangolo& t){_id = t._id; _vertici = t._vertici; _lati = t._lati;
            return *this;}
    };

    class Mesh
    {
    friend class IOMesh;
    protected:
        vector<Triangolo> _listaTriangoli;
        vector<Lato> _listaLati;
        vector<Punto> _listaPunti;
    public:
        Mesh(const vector<Punto>& listaPunti);
        Mesh(){}
        array<unsigned int, 2> DentroMesh(const Punto& p, Triangolo* triang);
        void ControlloDelaunay(list<array<unsigned int, 2>>& coda);
        void CollegaSenzaIntersezioni(Punto& newp);

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
