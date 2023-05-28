#include "Delaunay_class.hpp"

namespace ProjectLibrary
{
    Punto::Punto(int& id , double& x, double& y):
        _id(id) , _x(x) , _y(y)
    {}

    Punto::Punto(const Punto& p):
        _id(p._id) , _x(p._x) , _y(p._y)
    {}

    string Punto::Show(){
        return to_string(_id) + " " + to_string(_x) + " " + to_string(_y);
    }

    Lato::Lato(unsigned int& id , Punto& p1, Punto& p2):
        _id(id), _p1(p1), _p2(p2)
    {
        _length = sqrt(pow((p1._x - p2._x),2) + pow((p1._y - p2._y),2));
    }

    Lato::Lato(const Lato& lat):
        _id(lat._id) , _p1(lat._p1) , _p2(lat._p2)
    {}
    
    string Lato::Show(){
        return to_string(_id) + " " + _p1.Show() + " " + _p2.Show() + " " + to_string(_length);
    }


    Triangolo::Triangolo(unsigned int& identificatore, Punto& p1, Punto& p2, Punto& p3, Lato& lat1, Lato& lat2, Lato& lat3):
        _id(identificatore)
    {
        _lati = {lat1, lat2, lat3};
        _vertici = {p1,p2,p3};
        this->OrdinamentoAntiorario();
    }

/*
    Triangolo::Triangolo(unsigned int& identificatore, Punto& p1, Punto& p2, Punto& p3, unsigned int& idlato):
        _id(identificatore)
    {
        _vertici = {p1,p2,p3};
        this->OrdinamentoAntiorario();
        for(unsigned int i = 0; i<3; i++){
            Lato l = Lato(idlato,_vertici[i],_vertici[(i+1)%3]);
            (Mesh._listaLati).push_back(l);
            _lati.push_back(l);
            idlato++;
        }
    }

    Triangolo::Triangolo(unsigned int& identificatore, Punto& p, Lato& lat, unsigned int& idlato):
        _id(identificatore)
    {
        _vertici = {p,lat._p1,lat._p2};
        this->OrdinamentoAntiorario();
        for(unsigned int i = 0; i<3; i++){
            if((_vertici[i]._id != (lat._p1)._id && _vertici[(i+1)%3]._id == (lat._p2)._id)||
                (_vertici[(i+1)%3]._id != (lat._p1)._id && _vertici[i]._id == (lat._p2)._id))
            {
                (Mesh._listaLati).push_back(lat);
                _lati.push_back(lat);
                idlato++;
            }else{
                Lato l = Lato(idlato,_vertici[i],_vertici[(i+1)%3]);
                (Mesh._listaLati).push_back(l);
                _lati.push_back(l);
                idlato++;
            }
        }
    }

    Triangolo::Triangolo(unsigned int& identificatore, Lato& lat1, Lato& lat2, unsigned int& idlato):
        _id(identificatore)
    {
        if(lat2._p1 != lat1._p1 && lat2._p1 != lat1._p2){
            Punto p = lat2._p1;
        }else{
            Punto p = lat2._p2;
        }
        _vertici = {p,lat1._p1,lat1._p2};
        this->OrdinamentoAntiorario();
        for(unsigned int i = 0; i<3; i++){
            if((_vertici[i]._id == (lat1._p1)._id && _vertici[(i+1)%3]._id == (lat1._p2)._id)||
                (_vertici[(i+1)%3]._id == (lat1._p1)._id && _vertici[i]._id == (lat1._p2)._id))
            {
                (Mesh._listaLati).push_back(lat1);
                _lati.push_back(lat1);
                idlato++;
            } else if((_vertici[i]._id == (lat2._p1)._id && _vertici[(i+1)%3]._id == (lat2._p2)._id)||
                       (_vertici[(i+1)%3]._id == (lat2._p1)._id && _vertici[i]._id == (lat2._p2)._id))
            {
                (Mesh._listaLati).push_back(lat2);
                _lati.push_back(lat2);
                idlato++;
            } else {
                Lato l = Lato(idlato,_vertici[i],_vertici[(i+1)%3]);
                (Mesh._listaLati).push_back(l);
                _lati.push_back(l);
                idlato++;

            }
        }
    }
*/

    Triangolo::Triangolo(const Triangolo& triang):
        _id(triang._id) , _lati(triang._lati) , _vertici(triang._vertici)
    {}
    
    array<unsigned int, 2> Mesh::DentroMesh(const Punto p)
    {
        array<unsigned int, 2> result = {0,0};
        for (unsigned int i = 0; i < ( _listaTriangoli.size() -1 ) ; i++)
        {

        }



        return result;

    }

    Mesh::Mesh(const vector<Punto>& listaPunti)
    {
        unsigned int idlato = 0;
        unsigned int idtriang = 0;
        list<Punto> PuntiNonEstr;
        for(unsigned int j = 0; j<listaPunti.size(); j++)
            PuntiNonEstr.push_back(listaPunti[j]);
        double Area = 0;
        unsigned int i = 1;
        while (Area == 0){
            i++;
            double Area = (listaPunti[0]._x*listaPunti[1]._y) + (listaPunti[0]._y*listaPunti[i]._x) +
                          (listaPunti[1]._x*listaPunti[i]._y) - (listaPunti[i]._x*listaPunti[1]._y) -
                          (listaPunti[i]._y*listaPunti[0]._x) - (listaPunti[1]._x*listaPunti[0]._y);

        }        
        _listaPunti.push_back(listaPunti[0]);
        _listaPunti.push_back(listaPunti[1]);
        _listaPunti.push_back(listaPunti[i]);
        PuntiNonEstr.remove(listaPunti[0]);
        PuntiNonEstr.remove(listaPunti[1]);
        PuntiNonEstr.remove(listaPunti[i]);

        Lato l1 = Lato(idlato,_listaPunti[0],_listaPunti[1]);
        idlato++;
        _listaLati.push_back(l1);
        Lato l2 = Lato(idlato,_listaPunti[1],_listaPunti[i]);
        _listaLati.push_back(l2);
        idlato++;
        Lato l3 = Lato(idlato,_listaPunti[i],_listaPunti[0]);
        _listaLati.push_back(l3);

        Triangolo t1 = Triangolo(idtriang,_listaPunti[0], _listaPunti[1], _listaPunti[i], l1, l2, l3);
        idtriang++;
        _listaTriangoli.push_back(t1);

        Triangolo tr;
        array<unsigned int, 2> DM;
        for(Punto po : PuntiNonEstr){
            DM = this->DentroMesh(po);
            if (DM[0] == 1){
                tr =  _listaTriangoli[DM[1]];
                idlato++;
                l1 = Lato(idlato,(tr._lati)[0]._p1, po);
                idlato++;
                l2 = Lato(idlato, po, (tr._lati)[0]._p2);
                _listaLati.push_back(l1);
                _listaLati.push_back(l2);
                t1 = Triangolo(DM[1],"?","?", po, (tr._lati)[0],l1,l2);
                _listaTriangoli[DM[1]] = t1;
                _listaTriangoli.push_back(t1);
                if(_listaTriangoli.size() >= 3)
                    this->ControlloDelaunay(t1);
            }else{
                this->CalcolaPuntiVicini(po);
                t1 = Triangolo();
                this ->ControlloDelaunay(t1);

            }
            PuntiNonEstr.pop_front();
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

        for(unsigned int i = 0; i < ((mesh._listaLati).size()-1) ; i++){
            file << mesh._listaLati[i]._id << " " << mesh._listaLati[i]._p1._id << " "
                 << mesh._listaLati[i]._p2._id << " " << mesh._listaLati[i]._length << endl;
        }
        file.close();


        // dubbio, posso "sovrascrivere" sull'oggetto ofstream ?
        file.open(OutFilePath + "Triangoli_Mesh.csv");
        if (file.fail())
            return false;

        file << "id_triangolo p1.id p2.id p3.id l1.id l2.id l3.id \n" << endl;

        for(unsigned int i = 0; i < ((mesh._listaLati).size()-1) ; i++){
            file << mesh._listaTriangoli[i]._id << " ";

            for (unsigned int j=0; j < 3 ; j++){
                file << mesh._listaTriangoli[i]._vertici[j]._id << " ";
            }

            for (unsigned int j=0; j < 3 ; j++){
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

        for (unsigned int i = 0 ; i<3 ; i++){
            final = final + " " + _vertici[i].Show();
        }

        for (unsigned int i = 0 ; i<3 ; i++){
            final = final + " " + _lati[i].Show();
        }

        return final;
    }
    

}


