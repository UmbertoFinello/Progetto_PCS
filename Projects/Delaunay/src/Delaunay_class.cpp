#include "Delaunay_class.hpp"

namespace ProjectLibrary
{
    Punto::Punto(unsigned int& id , double& x, double& y):
        _id(id) , _x(x) , _y(y)
    {}

    Punto::Punto(const Punto& p):
        _id(p._id) , _x(p._x) , _y(p._y)
    {}

    Lato::Lato(unsigned int& id , Punto& p1, Punto& p2):
        _id(id) , _p1(p1) , _p2(p2)
    {
        _length = sqrt( (p1._x - p2._x)^2 + (p1._y - p2._y)^2 );
    }

    Mesh::Mesh(const vector<Punto>& listaPunti):
        _listaPunti(listaPunti)
    {}

    vector <Punto> ImportMesh(const string& FileName)
    // la faccio booleana per stopparla e passo per referenza il
    // vettore da aggiornare per metterci i punti.
    {
        std::ifstream file;
        vector<Punto> result;

        file.open("./FileName");

        if(file.fail())
          return false;

        list<string> listaRighe;
        string riga;
        while(getline(file, riga))
            listaRighe.push_back(riga);
        file.close();
        listaRighe.pop_front();

        unsigned int id;
        double x,y;

        for(const string& line : listaRighe)
        {
            istringstream converter(line);
            converter >> id >> x >> y;
            Punto p = Punto(id,x,y);
            result.push_back(p);
        }

        return result;
    }

    void ExportMesh(const Mesh& mesh , string OutFilePath)
    {
        ofstream file;
        file.open(Out);
        if (file.fail)

        file << "Punti:\n";
        file << "   id  x   y\n";

        for(unsigned int i = 0; i < ((mesh._listaPunti).size()-1) ; i++)
        {
            file << "   " <<
        }
    }

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

