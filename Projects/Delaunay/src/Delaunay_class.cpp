#include "Delaunay_class.hpp"

namespace ProjectLibrary
{
    Triangolo::Triangolo(vector<Punto>& v, unsigned int identificatore, const vector<Lato>& l):
        _vertici(v), _id(identificatore), _lati(l)
    {
        this->OrdinamentoAntiorario();
    }

    Triangolo::OrdinamentoAntiorario()
    {
        double Area = (0.5)*(_vertici[0]_x*_vertici[1]_y + _vertici[0]_y*_vertici[2]_x + _vertici[1]_x*_vertici[2]_y - _vertici[2]_x*_vertici[1]_y - _vertici[2]_y*vertici[0]_x - _vertici[1]_x*_vertici[0]_y);
        if (Area < 0)       //TOLLERANZA
        {
            Punto a = _vertici[1];
            _vertici[1] = Punto(_vertici[2]);
            _vertici[2] = a;
        }
        return 0;
    }

    Triangolo::CalcolaAngolo(const Lato& segm)
    {
        unsigned int l1 = 3;
        unsigned int l2 = 3;
        for (unsigned int i = 0; i < 3; i++)
        {
            if ( _lati[i]._id != segm._id)
            {
               if(  l1 == 3)
               {
                   l1 = i;
               }
               else if ( l2 == 3)
               {
                   l2 = i;
               }
            }
        }


        double cosenoAngolo = ((l[0]._length)^2 + (l[1]._length)^2 - (segm._length * segm._length) / (2 * l[0]._length * l[1]._length);
        double angoloRad = acos(cosenoAngolo);
        // double angoloDeg = angoloRad * 180.0 / M_PI;
        return angoloRad;
    }

};
