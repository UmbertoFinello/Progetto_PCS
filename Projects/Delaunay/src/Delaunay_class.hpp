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
        bool _inserito = false;

        static constexpr double geometricTol = 1.0e-12;
        static constexpr double geometricTol_Squared = max_tolerance(Punto::geometricTol * Punto::geometricTol,
                                                                     numeric_limits<double>::epsilon());
        Punto(unsigned int& id, double& x, double& y);
        Punto(const Punto& p);
        Punto(){}
        string Show();

        friend Punto operator-(const Punto& punto1, const Punto& punto2){
            unsigned int id = 0;
            double xpunto = punto1._x-punto2._x;
            double ypunto = punto1._y-punto2._y;
            return Punto(id, xpunto, ypunto);
        }

        inline Punto& operator=(const Punto& p){_id = p._id; _x = p._x; _y = p._y; _inserito = p._inserito;
            return *this;}
    };

    inline double Norm(const double& x, const double& y)
    {
      if(abs(x)<Punto::geometricTol){
          return abs(y);
      }else if (abs(y)<Punto::geometricTol){
          return abs(x);
      }else{
          if(abs(x)<abs(y)){
              return abs(y)*sqrt(1+pow(x/y,2));
          }else if(abs(x)>abs(y)){
              return abs(x)*sqrt(1+pow(y/x,2));
          }else{
              return abs(x)*sqrt(2);
          }
      }
    }

    inline bool operator==(const Punto& p1, const Punto& p2)
    {
      return (Norm(p1._x - p2._x, p1._y - p2._y) <= Punto::geometricTol * max(Norm(p1._x, p1._y), Norm(p2._x, p2._y)));
    }

    inline bool operator!=(const Punto& p1, const Punto& p2)
    {
      return !(p1 == p2);
    }

    inline double crossProduct(const Punto& p1, const Punto& p2){
      return (p1._x * p2._y - p1._y * p2._x);
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
        unsigned int _prec = 0;
        unsigned int _succ = 0;
    public:
        Lato(unsigned int& id , const Punto& p1, const Punto& p2, unsigned int& idtr);
        Lato(const Lato& lat);
        Lato(){}
        string Show();
        Lato& operator=(const Lato& l){_id = l._id; _p1 = l._p1; _p2 = l._p2; _length = l._length;
            _listIdTr = l._listIdTr; _prec = l._prec; _succ = l._succ; return *this;}
    };

    class Triangolo
    {
    friend class IOMesh;
    friend class Mesh;
    protected:
        unsigned int _id;
        array<unsigned int, 3> _vertici;
        array<unsigned int, 3> _lati;
    protected:
        double CalcolaAngolo(const Lato& segm, vector<Lato>& lst_lat);
    public:
        Triangolo(unsigned int& ident, const unsigned int& p1, const unsigned int& p2, const unsigned int& p3,
                  const unsigned int& l1, const unsigned int& l2, const unsigned int& l3);
        Triangolo(const Triangolo& triang);
        Triangolo(){}        
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
        unsigned int _hullBeginLato = 0;
        list<array<unsigned int,2>> _codaDelaunay;
    protected:
        void ControlloDelaunay();
        void CollegaSenzaIntersezioni(const Punto& p, unsigned int& id_t, unsigned int& id_l);
        bool accettabile(const Punto& pnew, const Punto& v);
        array<unsigned int, 2> DentroMesh(const Punto& p);
        void PuntoInterno(const Punto& po, unsigned int& itr, unsigned int& id_tr, unsigned int& id_lt);
        void PuntoBordoTriang(const Punto& po, unsigned int& ilt, unsigned int& id_tr, unsigned int& id_lt);
        void PuntoBordoHull(const Punto& po, unsigned int& ilt, unsigned int& id_tr, unsigned int& id_lt);
    public:
        Mesh(const vector<Punto>& listaPunti);
        Mesh(){}
        string Show();
    };

    class IOMesh
    {
        public:
            IOMesh(){}
            bool ImportPunti(vector<Punto>& listaPunti, const string& FileName);
            bool ExportMesh(const Mesh& mesh, const string& OutFilePath, const string& OutFileNameParz);
    };
}

#endif // __DELAUNAY_H
