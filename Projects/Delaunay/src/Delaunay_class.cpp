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
        _id(p._id), _x(p._x), _y(p._y)
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

    /*void Triangolo::OrdinamentoAntiorario()
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
    */

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

    /*
    array<unsigned int, 2> Triangolo::CheckConnection(const Punto& a, const Punto& b, vector<Lato>*& veclat){
        for (unsigned int k = 0; k<veclat->size(); k++){
            if((a == (*veclat)[k]._p1 && b == (*veclat)[k]._p2)||
                (b == (*veclat)[k]._p1 && a == (*veclat)[k]._p2)){
                return {1,(*veclat)[k]._id};
            }
        }
        return {0,0};
    }
    */

    Triangolo::Triangolo(unsigned int& identificatore, const Punto& p1, const Punto& p2, const Punto& p3, unsigned int& idlato,
                         vector<Lato>*& vlat):
        _id(identificatore)
    {
        _vertici = {p1,p2,p3};
        this->OrdinamentoAntiorario();
        Lato l;
        array<unsigned int, 2> CC = {0,0};
        for(unsigned int i = 0; i<3; i++){
            CC = this->CheckConnection(_vertici[i],_vertici[(i+1)%3],  vlat);
            if(CC[0]== 1){
                _lati[i] = (*vlat)[CC[1]];
                (((*vlat)[CC[1]])._listIdTr).push_back(identificatore);
            } else {
                l = Lato(idlato,_vertici[i],_vertici[(i+1)%3], identificatore);
                (*vlat).push_back(l);
                _lati[i] = l;
                idlato++;
            }
        }

    }

    Triangolo::Triangolo(const Triangolo& triang):
        _id(triang._id), _vertici(triang._vertici), _lati(triang._lati)
    {}

    string Triangolo::Show(){
        return to_string(_id) + " " + to_string(_vertici[0]._id) + "," + to_string(_vertici[1]._id) + "," +
               to_string(_vertici[2]._id) + " " + to_string(_lati[0]._id) + "," +to_string(_lati[1]._id) +
               "," + to_string(_lati[2]._id);
    }
    
    array<unsigned int, 2> Mesh::DentroMesh(const Punto& p, Triangolo* triang)
    {
        unsigned int ident = 0;
        array<unsigned int, 2> result = {0,0};
        Triangolo tr;
        for (unsigned int k = 0; k<_listaTriangoli.size(); k++) {
            tr = _listaTriangoli[k];
            double expr1 = tr._vertici[2]._x - tr._vertici[0]._x;
            double expr2 = tr._vertici[2]._y - tr._vertici[0]._y;
            Punto v1 = Punto(ident, expr1, expr2);
            expr1 = tr._vertici[1]._x - tr._vertici[0]._x;
            expr2 = tr._vertici[1]._y - tr._vertici[0]._y;
            Punto v2 = Punto(ident, expr1, expr2);
            expr1 = p._x - tr._vertici[0]._x;
            expr2 = p._y - tr._vertici[0]._y;
            Punto v3 = Punto(ident, expr1, expr2);
            double dot1 = crossProduct(v1, v3);
            double dot2 = crossProduct(v1, v2);
            if (dot1 >= 0 && dot2 >= 0 && (dot1 + dot2) <= crossProduct(v1, v2)){
                result  = {1, tr._id};
                triang = &_listaTriangoli[k];
                return result;
            }
        }
        return result;
    }

    void Mesh::ControlloDelaunay(list<array<unsigned int, 2>>& coda)
    {
        vector<Lato>* vl = &_listaLati;
        array<unsigned int, 2> fc; //fronte coda
        array<unsigned int, 2> nec; //nuovi elementi coda
        while(!(coda.empty())){
            fc = coda.front(); //indice coda
            Lato latcom = _listaLati[fc[0]]; //lato in comune
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
                    Lato ln = Lato(fc[0], vnl[0], vnl[1], fc[0]);
                    _listaLati[fc[0]] = ln;
                    Triangolo tn1 = Triangolo((latcom._listIdTr)[0], vnl[0], vnl[1], latcom._p1, fc[0], vl);
                    _listaTriangoli[(latcom._listIdTr)[0]] = tn1;
                    Triangolo tn2 = Triangolo((latcom._listIdTr)[1], vnl[0], vnl[1], latcom._p2 , fc[0], vl);
                    _listaTriangoli[(latcom._listIdTr)[1]] = tn2;
                    for(unsigned int j = 0; j<3; j++){
                        if ((tn1._lati)[j]._id != fc[0])
                            coda.push_back((tn1._lati)[j]._id);
                    }
                    for(unsigned int j = 0; j<3; j++){
                        if ((tn2._lati)[j]._id != fc[0])
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

        if((v[0] == v[3]) && (v[2] != v[1]))
        {
            punti_scelti[0]=v[0];
            punti_scelti[1]=v[1];
            punti_scelti[2]=v[2];
        }

        else if ((v[0] != v[3]) && (v[2] == v[1]))
        {
            punti_scelti[0]=v[0];
            punti_scelti[1]=v[1];
            punti_scelti[2]=v[3];
        }

        else
        {
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

        _listaPunti.push_back(punti_scelti[0]);
        _listaPunti.push_back(punti_scelti[1]);
        _listaPunti.push_back(punti_scelti[2]);
        Triangolo tr = Triangolo(idtriang, punti_scelti[0], punti_scelti[1], punti_scelti[2], idlato, ptr_lati);
        idtriang++;
        _listaTriangoli.push_back(tr);

        for(unsigned int j = 0; j<listaPunti.size(); j++){
            if(listaPunti[j] != punti_scelti[0] || listaPunti[j] != punti_scelti[1] ||
                listaPunti[j] != punti_scelti[2])
            PuntiNonEstr.push_back(listaPunti[j]);
        }

        Triangolo* pr = nullptr;
        Triangolo tng;
        list<unsigned int> lcd;
        array<unsigned int, 2> DM;
        for(Punto po : PuntiNonEstr){
            DM = this->DentroMesh(po, pr);
            PuntiNonEstr.pop_front();
            _listaPunti.push_back(po);
            if (DM[0] == 1){
                tr =  *pr;
                tng = Triangolo(DM[1], po, (tr._vertici)[0], (tr._vertici)[1], idlato, ptr_lati);
                _listaTriangoli[DM[1]] = tng;
                for(unsigned int k = 1; k<3; k++){
                    tng = Triangolo(idtriang, po, (tr._vertici)[k], (tr._vertici)[(k+1)%3], idlato, ptr_lati);
                    idtriang++;
                    _listaTriangoli.push_back(tng);                    
                }
                for(unsigned int i = 0; i<3; i++){
                        lcd.push_back((tr._lati)[i]._id);
                    }
                this->ControlloDelaunay(lcd);
            }else{
               // this->CollegaSenzaIntersezioni(po);
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


