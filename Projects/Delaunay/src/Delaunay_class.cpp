#include "Delaunay_class.hpp"
#include "sorting.hpp"

namespace ProjectLibrary
{
    double crossProduct(Punto& p1, Punto& p2){
        return (p1._x * p2._y - p1._y * p2._x);
    }

    Punto::Punto(unsigned int& id , double& x, double& y):
        _id(id) , _x(x) , _y(y)
    {}

    Punto::Punto(const Punto& p):
        _id(p._id), _x(p._x), _y(p._y), _succ(p._succ), _prec(p._prec)
    {}

    string Punto::Show(){
        return to_string(_id) + " " + to_string(_x) + " " + to_string(_y);
    }

    Lato::Lato(unsigned int& id , Punto& p1, Punto& p2, unsigned int& idtr):
        _id(id), _p1(p1), _p2(p2)
    {
        _listIdTr.push_back(idtr);
        _length = sqrt(pow((p1._x - p2._x),2) + pow((p1._y - p2._y),2));
    }

    Lato::Lato(const Lato& lat):
        _id(lat._id) , _p1(lat._p1) , _p2(lat._p2), _length(lat._length), _listIdTr(lat._listIdTr)
    {}
    
    string Lato::Show(){
        string a;
        if(_listIdTr.size() == 1)
            a = to_string(_listIdTr[0]);
        else
            a = to_string(_listIdTr[0]) + "," + to_string(_listIdTr[1]);

        return to_string(_id) + " " + to_string(_p1._id) + " " + to_string(_p2._id) + " " + to_string(_length) + " " + a;
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

    Triangolo::Triangolo(unsigned int& identificatore, const Punto& p1, const Punto& p2, const Punto& p3, const Lato& l1, const Lato& l2, const Lato& l3):
        _id(identificatore)
    {
        _vertici[0] = p1;
        _vertici[1] = p2;
        _vertici[2] = p3;
        _lati[0] = l1;
        _lati[1] = l2;
        _lati[2] = l3;
    }

    Triangolo::Triangolo(const Triangolo& triang):
        _id(triang._id), _vertici(triang._vertici), _lati(triang._lati)
    {}

    string Triangolo::Show(){
        return to_string(_id) + " " + to_string(_vertici[0]._id) + "," + to_string(_vertici[1]._id) + "," +
               to_string(_vertici[2]._id) + " " + to_string(_lati[0]._id) + "," +to_string(_lati[1]._id) +
               "," + to_string(_lati[2]._id);
    }
    
    array<unsigned int, 2> Mesh::DentroMesh(const Punto& p)
    {
        unsigned int ident = 0;
        array<unsigned int, 2> result = {0,0};
        Triangolo tr;
        for (unsigned int k = 0; k<_listaTriangoli.size(); k++) {
            tr = _listaTriangoli[k];
            Punto v1 = tr._vertici[2]-tr._vertici[0];
            Punto v2 = tr._vertici[1]-tr._vertici[0];
            Punto v3 = p-tr._vertici[0];
            double dot1 = crossProduct(v1, v3);
            double dot2 = crossProduct(v1, v2);
            if (dot1 >= 0 && dot2 >= 0 && (dot1 + dot2) <= crossProduct(v1, v2)){
                result  = {1, tr._id};
                return result;
            }
        }
        return result;
    }

    void Mesh::ControlloDelaunay(list<unsigned int>& coda)
    {
        vector<Lato>* vl = &_listaLati;
        while(!(coda.empty())){
            unsigned int ic = coda.front(); //indice coda
            Lato latcom = _listaLati[ic]; //lato in comune
            if(latcom._listIdTr.size() == 2){
                double somang; //somma angoli
                array<Punto, 2> vnl; //vertici nuovo lato
                somang = _listaTriangoli[(latcom._listIdTr)[0]].CalcolaAngolo(latcom) + _listaTriangoli[(latcom._listIdTr)[1]].CalcolaAngolo(latcom);
                if (somang > M_PI){
                    for(unsigned int h = 0; h<3; h++){
                        if(((_listaTriangoli[(latcom._listIdTr)[0]]._vertici)[h] != latcom._p1) and
                                (_listaTriangoli[(latcom._listIdTr)[0]]._vertici)[h] != latcom._p2){
                            vnl[0] = (_listaTriangoli[(latcom._listIdTr)[0]]._vertici)[h];
                        }
                        if(((_listaTriangoli[(latcom._listIdTr)[1]]._vertici)[h] != latcom._p1) and
                                (_listaTriangoli[(latcom._listIdTr)[1]]._vertici)[h] != latcom._p2){
                            vnl[1] = (_listaTriangoli[(latcom._listIdTr)[1]]._vertici)[h];
                        }
                    }
                    Lato ln = Lato(ic, vnl[0], vnl[1], ic);
                    _listaLati[ic] = ln;
                    Triangolo tn1 = Triangolo((latcom._listIdTr)[0], vnl[0], vnl[1], latcom._p1, ic, vl);
                    _listaTriangoli[(latcom._listIdTr)[0]] = tn1;
                    Triangolo tn2 = Triangolo((latcom._listIdTr)[1], vnl[0], vnl[1], latcom._p2 ,ic, vl);
                    _listaTriangoli[(latcom._listIdTr)[1]] = tn2;
                    for(unsigned int j = 0; j<3; j++){
                        if ((tn1._lati)[j]._id != ic)
                            coda.push_back((tn1._lati)[j]._id);
                    }
                    for(unsigned int j = 0; j<3; j++){
                        if ((tn2._lati)[j]._id != ic)
                            coda.push_back((tn2._lati)[j]._id);
                    }
                }
            }
            coda.pop_front();
        }
    }

    Mesh::Mesh(const vector<Punto>& listaPunti)
    {
        unsigned int idlato = 0;
        unsigned int idtriang = 0;
        list<Punto> PuntiNonEstr;
        vector<Lato>* ptr_lati = &_listaLati;
        unsigned int n = listaPunti.size() - 1;
        vector<Punto> vx = listaPunti;
        bool ax = true;        
        SortLibrary::MergeSort(vx, 0, n, ax);
        ax = false;
        vector<Punto> vy = listaPunti;
        SortLibrary::MergeSort(vy, 0, n, ax);

        array<Punto, 4> v = {vx[0], vx[n], vy[0], vy[n]};
        array<Punto, 3> punti_scelti;

        if((v[0] == v[3]) && (v[2] != v[1])){
            punti_scelti[0]=v[0];
            punti_scelti[1]=v[1];
            punti_scelti[2]=v[2];
        } else if ((v[0] != v[3]) && (v[2] == v[1])) {
            punti_scelti[0]=v[0];
            punti_scelti[1]=v[1];
            punti_scelti[2]=v[3];
        } else {
            if ((v[0] == v[3]) && (v[2] == v[1])){
                v[0]= vx[1];
                v[2]= vy[1];
            }

            double AreaMax = 0;
            double Area = 0;

            array<unsigned int,3> indici_scelti = {0,0,0};

            for (unsigned int i = 0; i< 4;i++)
            {
                Area = 0.5*abs ( (v[(i+1)%4]._x - v[i]._x)*(v[(i+2)%4]._y - v[i]._y) -
                                 (v[(i+1)%4]._y - v[i]._y)*(v[(i+2)%4]._x - v[i]._x)   );

                if (AreaMax < Area) //TOLLERANZA
                {
                    AreaMax = Area;
                    for (unsigned int j = 0; j<3;j++)
                    {
                        indici_scelti[j] = (i+j)%4;
                    }
                }
            }
            punti_scelti[0]= v[indici_scelti[0]];
            punti_scelti[1]=v[indici_scelti[1]];
            punti_scelti[2]=v[indici_scelti[2]];
        }        
        double Area = (0.5)*( (punti_scelti[0]._x*punti_scelti[1]._y) +
                               (punti_scelti[0]._y*punti_scelti[2]._x) + (punti_scelti[1]._x*punti_scelti[2]._y)
                               - (punti_scelti[2]._x*punti_scelti[1]._y) - (punti_scelti[2]._y*punti_scelti[0]._x)
                               - (punti_scelti[1]._x*punti_scelti[0]._y) );

        if (Area < 0)       //TOLLERANZA
        {
            Punto a = punti_scelti[1];
            punti_scelti[1] = punti_scelti[2];
            punti_scelti[2] = a;
        }
        _listaPunti.push_back(punti_scelti[0]);
        _listaPunti.push_back(punti_scelti[1]);
        _listaPunti.push_back(punti_scelti[2]);
        _hullBegin = &_listaPunti[0];
        _listaPunti[0]._prec = &_listaPunti[2];
        _listaPunti[1]._prec = &_listaPunti[0];
        _listaPunti[2]._prec = &_listaPunti[1];
        _listaPunti[0]._succ = &_listaPunti[1];
        _listaPunti[1]._succ = &_listaPunti[2];
        _listaPunti[2]._succ = &_listaPunti[0];
        Lato l;
        for(unsigned int i = 0; i<3; i++){
            l = Lato(idlato,_listaPunti[i],_listaPunti[(i+1)%3], idtriang);
            _listaLati.push_back(l);
            idlato++;
        }
        Triangolo tr = Triangolo(idtriang, _listaPunti[0] ,_listaPunti[1], _listaPunti[2],
                                 _listaLati[0], _listaLati[1], _listaLati[2]);
        idtriang++;
        _listaTriangoli.push_back(tr);

        for(unsigned int j = 0; j<listaPunti.size(); j++){
            if(listaPunti[j] != punti_scelti[0] || listaPunti[j] != punti_scelti[1] ||
                listaPunti[j] != punti_scelti[2])
            PuntiNonEstr.push_back(listaPunti[j]);
        }

        unsigned int idl1;
        unsigned int idl2;
        unsigned int idl3;
        Triangolo tng;
        array<unsigned int, 2> il_it;
        list<unsigned int> lcd;
        array<unsigned int, 2> DM;
        for(Punto po : PuntiNonEstr){
            DM = this->DentroMesh(po);
            PuntiNonEstr.pop_front();
            _listaPunti.push_back(po);
            if (DM[0] == 1){
                tr =  _listaTriangoli[DM[1]];
                l1 = Lato(idlato, po, (tr._lati)[0]._p1, DM[1]);
                idl1 =idlato;
                _listaLati.push_back(l1);
                idlato++;

                l2 = Lato(idlato, (tr._lati)[0]._p2, po, DM[1]);
                idl2 = idlato;
                _listaLati.push_back(l2);
                idlato++;

                tng = Triangolo(DM[1], po, (tr._vertici)[0], (tr._vertici)[1], l1, (tr._lati)[0], l2);
                _listaTriangoli[DM[1]] = tng;
                il_it = {(tr._lati)[0]._id, tng._id};
                lcd.push_back(il_it);

                l3 = Lato(idlato, (tr._lati)[1]._p2, po, idtriang);
                idl3 = idlato;
                _listaLati.push_back(l3);
                idlato++;
                (_listaLati[idl2]._listIdTr)[1] = idtriang;
                for(unsigned int i = 0; i<2; i++){
                    if((_listaLati[(tr._lati)[1]._id]._listIdTr)[i] == DM[1]){
                        (_listaLati[(tr._lati)[1]._id]._listIdTr)[i] = tng._id;
                        break;
                    }
                }
                tng = Triangolo(idtriang, po, (tr._vertici)[1], (tr._vertici)[2], l2, (tr._lati)[1], l3);

                idtriang++;
                _listaTriangoli.push_back(tng);
                il_it = {(tr._lati)[1]._id, tng._id};
                lcd.push_back(il_it);
                (_listaLati[idl1]._listIdTr)[1] = idtriang;
                (_listaLati[idl3]._listIdTr)[1] = idtriang;
                tng = Triangolo(idtriang, po, (tr._vertici)[2], (tr._vertici)[0], l3, (tr._lati)[2], l1);
                idtriang++;
                _listaTriangoli.push_back(tng);
                il_it = {(tr._lati)[2]._id, tng._id};
                lcd.push_back(il_it);
                for(unsigned int i = 0; i<2; i++){
                    if((_listaLati[(tr._lati)[1]._id]._listIdTr)[i] == DM[1]){
                        (_listaLati[(tr._lati)[1]._id]._listIdTr)[i] = tng._id;
                        break;
                    }
                }

                this->ControlloDelaunay(lcd);
            } else if (DM[0] == 2){

            }else{
               // this->CollegaSenzaIntersezioni(po);
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

        for(unsigned int i = 0; i < ((mesh._listaLati).size()-1) ; i++){
            file << mesh._listaLati[i]._id << " " << mesh._listaLati[i]._p1._id << " "
                 << mesh._listaLati[i]._p2._id << " " << mesh._listaLati[i]._length << endl;
        }
        file.close();


        // dubbio, posso "sovrascrivere" sull'oggetto ofstream ?
        file.open(OutFilePath + "Triangoli_Mesh.csv");
        if (file.fail())
            return false;

        file << "id_triangolo p1 p2 p3 l1 l2 l3 \n" << endl;

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


    



}


