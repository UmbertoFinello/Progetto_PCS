#include "Delaunay_class.hpp"

namespace ProjectLibrary
{
    Punto::Punto(unsigned int& id , double& x, double& y):
        _id(id) , _x(x) , _y(y)
    {}

    Punto::Punto(const Punto& p):
        _id(p._id) , _x(p._x) , _y(p._y)
    {}

    string Punto::Show()
    {
        return to_string(_id) + " " + to_string(_x) + " " + to_string(_y);
    }

    Lato::Lato(unsigned int& id , Punto& p1, Punto& p2):
        _id(id), _p1(p1), _p2(p2)
    {
        _length = sqrt(pow((p1._x - p2._x),2) + pow((p1._y - p2._y),2));
    }
    
    string Lato::Show()
    {
        return to_string(_id) + " " + _p1.Show() + " " + _p2.Show() + " " + to_string(_length);
    }

    Triangolo::Triangolo(Punto& p1, Punto& p2, Punto& p3, unsigned int& identificatore, vector<Lato>& l):
        _id(identificatore), _lati(l)
    {
        _vertici = {p1,p2,p3};
        this->OrdinamentoAntiorario();
    }

    
    array<unsigned int, 2> DentroMesh(const Punto p)
    {
        array<unsigned int, 2> result = {0,0};
        for (unsigned int i = 0; i < ( _listaTriangoli.size() -1 ) ; i++)
        {

        }



        return result;

    }

    Mesh::Mesh(const vector<Punto>& listaPunti):
        _listaPunti(listaPunti)
    {
        unsigned int idlato = 0;
        idlato++;
        Lato l1 = Lato(idlato,_listaPunti[0],_listaPunti[1]);
        idlato++;
        _listaLati.push_back(l1);
        Lato l2 = Lato(idlato,_listaPunti[1],_listaPunti[2]);
        _listaLati.push_back(l2);
        idlato++;
        Lato l3 = Lato(idlato,_listaPunti[2],_listaPunti[0]);
        _listaLati.push_back(l3);
        vector<Lato> lati = {l1,l2,l3};
        unsigned int progress_triang = 0;
        Triangolo t1 = Triangolo(_listaPunti[0], _listaPunti[1], _listaPunti[2],progress_triang,lati);
        _listaTriangoli.push_back(t1);

        unsigned int n = _listaPunti.size();
        vector<Punto> vr;
        array<unsigned int, 2> DM;
        for(unsigned int j = 3; j<n; j++){
            DM = Mesh.DentroMesh(_listaPunti[j]);
            if (DM[0] == 1){
                vector<Punto> vr = _listaTriangoli[DM[1]]._vertici;
                    l1 = Lato(idlato,(_listaTriangoli[DM[1]]._lati)[0]._p1,_listaPunti[j]);
                    l2 = Lato(idlato,(_listaTriangoli[DM[1]]._lati)[0]._p2,_listaPunti[j]);
                    _listaLati.push_back(l1);
                    _listaLati.push_back(l2);
                    lati = {l1,l2,(_listaTriangoli[DM[1]]._lati)[0]};
                    t1 = Triangolo(_listaTriangoli[DM[1]]._lati)[0]._p1,_listaTriangoli[DM[1]]._lati)[0]._p2, _listaPunti[j], DM[1], lati);
                    _listaTriangoli[DM[1]] = t1;


                    t1 = Triangolo(vr[k], vr[(k+1)%3], _listaPunti[j], DM[1], progress_lato);
                    _listaTriangoli.push_back(t1);
                    _listaLati.push_back(t1._lati[0]);
                    _listaLati.push_back(t1._lati[1]);
                    _listaLati.push_back(t1._lati[2]);
                    if(_listaTriangoli.size() >= 3)
                        ControlloDelaunay(t1);
                }
            }
        }

    bool IOMesh::ImportPunti(vector<Punto>& listaPunti, const string& FileName){
        ifstream file;
        file.open(FileName);
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
            listaPunti.push_back(p);
        }

        return true;
    }

    bool IOMesh::ExportMesh(const Mesh& mesh , const string& OutFilePath)
    {
        ofstream file;
        file.open(OutFilePath + "Lati_Mesh.csv");
        if (file.fail())
            return false;

        file << "id p1.id p2.id length\n";

        for(unsigned int i = 0; i < ((mesh._listaLati).size()-1) ; i++)
        {
            file << mesh._listaLati[i]._id << " " << mesh._listaLati[i]._p1._id << " "
                 << mesh._listaLati[i]._p2._id << " " << mesh._listaLati[i]._length << endl;
        }
        file.close();


        // dubbio, posso "sovrascrivere" sull'oggetto ofstream ?
        file.open(OutFilePath + "Triangoli_Mesh.csv");
        if (file.fail())
            return false;

        file << "id_triangolo p1.id p2.id p3.id l1.id l2.id l3.id \n" << endl;

        for(unsigned int i = 0; i < ((mesh._listaLati).size()-1) ; i++)
        {
            file << mesh._listaTriangoli[i]._id << " ";

            for (unsigned int j=0; j < 3 ; j++)
            {
                file << mesh._listaTriangoli[i]._vertici[j]._id << " ";
            }

            for (unsigned int j=0; j < 3 ; j++)
            {
                // faccio questi if perchè non voglio lo
                // spazio a fine della riga

                if ( j == 2)
                    file << endl;
                else
                    file << mesh._listaTriangoli[i]._lati[j]._id << " ";
            }

        }
        file.close();

        return true;
    }



    void Triangolo::OrdinamentoAntiorario()
    {
        double Area = (0.5)*( (_vertici[0]._x*_vertici[1]._y) +
                      (_vertici[0]._y*_vertici[2]._x) + (_vertici[1]._x*_vertici[2]._y)
                    - (_vertici[2]._x*_vertici[1]._y) - (_vertici[2]._y*_vertici[0]._x)
                    - (_vertici[1]._x*_vertici[0]._y) );

        if (Area < 0)       //TOLLERANZA
        {
            Punto a = _vertici[1];
            _vertici[1] = _vertici[2];
            _vertici[2] = a;
        }

    }

    double Triangolo::CalcolaAngolo(const Lato& segm)
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


        double cosenoAngolo = (pow(_lati[l1]._length,2) + pow(_lati[l2]._length,2) - (segm._length * segm._length)) / (2 * _lati[l1]._length * _lati[l2]._length);
        double angoloRad = acos(cosenoAngolo);
        // double angoloDeg = angoloRad * 180.0 / M_PI;
        return angoloRad;
    }

    string Triangolo::Show()
    {
        string final;
        final = to_string(_id) + " ";

        for (unsigned int i = 0 ; i<3 ; i++)
        {
            final = final + " " + _vertici[i].Show();
        }

        for (unsigned int i = 0 ; i<3 ; i++)
        {
            final = final + " " + _lati[i].Show();
        }

        return final;
    }
    

}


