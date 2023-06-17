#include "Delaunay_class.hpp"
#include "sorting.hpp"

namespace ProjectLibrary
{
    Punto::Punto(unsigned int& id , double& x, double& y):
        _id(id) , _x(x) , _y(y)
    {}

    Punto::Punto(const Punto& p):
        _id(p._id), _x(p._x), _y(p._y), _inserito(p._inserito)
    {}

    string Punto::Show(){
        return to_string(_id) + " " + to_string(_x) + " " + to_string(_y);
    }

    Lato::Lato(unsigned int& id , const Punto& p1, const Punto& p2, unsigned int& idtr):
        _id(id), _p1(p1), _p2(p2)
    {
        _listIdTr.push_back(idtr);
        _length = sqrt(pow((p1._x - p2._x),2) + pow((p1._y - p2._y),2));
    }

    Lato::Lato(const Lato& lat):
        _id(lat._id) , _p1(lat._p1), _p2(lat._p2), _length(lat._length), _listIdTr(lat._listIdTr),
        _prec(lat._prec), _succ(lat._succ)
    {}
    
    string Lato::Show(){
        string a;
        if(_listIdTr.size() == 1)
            a = to_string(_listIdTr[0]);
        else
            a = to_string(_listIdTr[0]) + "," + to_string(_listIdTr[1]);

        return to_string(_id) + " " + to_string(_p1._id) + " " + to_string(_p2._id) + " " + to_string(_length) + " " + a;
    }

    double Triangolo::CalcolaAngolo(const Lato& segm, vector<Lato>& lst_lat)
    {
        unsigned int l1 = 3;
        unsigned int l2 = 3;
        for (unsigned int i = 0; i < 3; i++){
            if (_lati[i] != segm._id){
                if(l1 == 3){
                   l1 = i;
                } else if (l2 == 3) {
                   l2 = i;
                }
            }
        }

        double cosenoAngolo = (pow((lst_lat[_lati[l1]])._length, 2) + pow(lst_lat[_lati[l2]]._length,2) - (segm._length * segm._length))
                                / (2 * lst_lat[_lati[l1]]._length * lst_lat[_lati[l2]]._length);
        double angoloRad = acos(cosenoAngolo);
        // double angoloDeg = angoloRad * 180.0 / M_PI;
        return angoloRad;
    }

    Triangolo::Triangolo(unsigned int& ident, const unsigned int& p1, const unsigned int& p2, const unsigned int& p3,
                         const unsigned int& l1, const unsigned int& l2, const unsigned int& l3):
        _id(ident)
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
        return to_string(_id) + " vertici:" + to_string(_vertici[0]) + "," + to_string(_vertici[1]) + "," +
               to_string(_vertici[2]) + " lati:" + to_string(_lati[0]) + "," +to_string(_lati[1]) +
               "," + to_string(_lati[2]);
    }
    

    array<unsigned int, 2> Mesh::DentroMesh(const Punto& p)
    {
        // 0:interno        1:bordo interno
        // 2:bordo hull     3:esterno
        array<unsigned int, 2> result = {0,0};// {tipo, id lati o triangoli}
        // ricerca esterno/interno
        unsigned int pointer = _hullBeginLato;
        bool inizio = true; // variabile per non uscire alla prima iteraz.
        Punto v1 = _listaLati[pointer]._p2 - _listaLati[pointer]._p1;
        Punto v2 = p - _listaLati[pointer]._p1;
        while((pointer != _hullBeginLato) || inizio){
            inizio=false;
            if(abs(crossProduct(v2,v1))<Punto::geometricTol){
                result[0]= 2;
                result[1]= pointer;
                return result;
            }
            else if (crossProduct(v2,v1)>0){
                result[0]=3;
                return result;
            }
            pointer = _listaLati[pointer]._succ;
        }
        array <unsigned int, 3> idPunti;
        array <unsigned int, 3> idLati;
        bool ext = true; // true: punto fuori triangolo
        Lato l = _listaLati[_hullBeginLato];
        unsigned int idCorr = l._listIdTr[0]; // id triangolo vecchio per ricerca
        while(ext)
        {
            // punti del triangolo
            idPunti = (_listaTriangoli[idCorr])._vertici; //id dei punti del triangolo
            idLati  = (_listaTriangoli[idCorr])._lati; //id dei lati del triangolo
            // verifico che il punto sia interno al triangolo considerato
            for(unsigned int i = 0; i<3;i++)
            {
               v1 = _listaPunti[idPunti[(i+1)%3]]-_listaPunti[idPunti[i]];
               v2 = p - _listaPunti[idPunti[i]];
               if(abs(crossProduct(v2,v1))<Punto::geometricTol)
               {
                   result[0]=1;
                   for(unsigned int j = 0; j<3;j++) // j cicla sui lati
                   {
                       // trova il lato su cui è il punto
                       if( (p._x >= min(_listaLati[idLati[j]]._p1._x,_listaLati[idLati[(j+1)%3]]._p2._x)) &&
                           (p._x <= max(_listaLati[idLati[j]]._p1._x,_listaLati[idLati[(j+1)%3]]._p2._x)) &&
                           (p._y >= min(_listaLati[idLati[j]]._p1._y,_listaLati[idLati[(j+1)%3]]._p2._y)) &&
                           (p._y <= max(_listaLati[idLati[j]]._p1._y,_listaLati[idLati[(j+1)%3]]._p2._y)))
                       {
                           result[1]=idLati[j];// gli passo l'id del triangolo
                           return result;
                       }
                   }
               }
               else if (crossProduct(v2,v1)<0)
               {
                   result[0]=0;
                   result[1]=idCorr;
                   return result;
               }
            }
            Punto pmax = _listaPunti[idPunti[0]]; //primo elemento dei vertici,
            //punto più distante dal punto di cui stiamo cercando la sistemazione
            double dmax = normSquared(pmax._x - p._x, pmax._y - p._y);
            // cerco punto a distanza massima
            for (unsigned int i=1; i<3; i++)
            {
               double dist = normSquared(_listaPunti[idPunti[i]]._x - p._x, _listaPunti[idPunti[i]]._y - p._y);
               if(dist > dmax)
               {
                   dmax = dist;
                   pmax = _listaPunti[idPunti[i]];
               }
            }
            // cerco il lato che non ha il punto massimo
            for (unsigned int i=1; i<3; i++)
            {
               if((_listaLati[idLati[i]]._p1 != pmax) && (_listaLati[idLati[i]]._p2 != pmax))
               {
                   l=_listaLati[idLati[i]];
                   break;
               }
            }
            // vado sul triangolo giusto adiacente al lato
            if(l._listIdTr[0]==idCorr)
            {
               idCorr=l._listIdTr[1];
            }
            else{
               idCorr=l._listIdTr[0];
            }
        }
        return result;
    }

    void Mesh::ControlloDelaunay()
    {
        array<unsigned int, 2> fc; //fronte coda
        array<unsigned int, 2> nec; //nuovi elementi coda
        array<unsigned int, 2> ix; //indici triangoli
        array<unsigned int, 2> tv_i;
        array<unsigned int, 2> nt_i;
        array<Punto, 2> pv;
        Lato latcom;
        unsigned int B;
        unsigned int l1;
        unsigned int C;
        unsigned int l2;
        unsigned int id_nt; //id nuovo triangolo controllo
        while(!(_codaDelaunay.empty())){            
            fc = _codaDelaunay.front(); //indice coda
            ix[0] = fc[1];
            latcom = _listaLati[fc[0]]; //lato in comune
            pv[0] = latcom._p1;
            pv[1] = latcom._p2;
            double somang; //somma angoli
            array<Punto, 2> vnl; //vertici nuovo lato
            somang = _listaTriangoli[(latcom._listIdTr)[0]].CalcolaAngolo(latcom, _listaLati) +
                     _listaTriangoli[(latcom._listIdTr)[1]].CalcolaAngolo(latcom, _listaLati);
            if (somang > M_PI){
                for(unsigned int k = 0; k<2; k++){
                    if (latcom._listIdTr[k] != fc[1])
                        id_nt = latcom._listIdTr[k];
                }
                ix[1] = id_nt;
                for(unsigned int j = 0; j<3; j++){
                    if ((_listaTriangoli[id_nt]._lati)[j] != fc[0] &&
                        _listaLati[(_listaTriangoli[id_nt]._lati)[j]]._listIdTr.size() == 2){
                        nec = {(_listaTriangoli[id_nt]._lati)[j], id_nt};
                        _codaDelaunay.push_back(nec);
                    }
                }
                for(unsigned int h = 0; h<3; h++){
                    if((_listaPunti[(_listaTriangoli[fc[1]]._vertici)[h]] != latcom._p1) &&
                        _listaPunti[(_listaTriangoli[fc[1]]._vertici)[h]] != latcom._p2){
                        vnl[0] = _listaPunti[(_listaTriangoli[fc[1]]._vertici)[h]];
                    }
                    if((_listaPunti[(_listaTriangoli[id_nt]._vertici)[h]] != latcom._p1) &&
                        _listaPunti[(_listaTriangoli[id_nt]._vertici)[h]] != latcom._p2){
                        vnl[1] = _listaPunti[(_listaTriangoli[id_nt]._vertici)[h]];
                    }
                }
                Lato ln = Lato(fc[0], vnl[0], vnl[1], fc[1]);
                _listaLati[fc[0]] = ln;
                _listaLati[fc[0]]._listIdTr.push_back(id_nt);
                for (unsigned int i = 0; i<3; i++){
                    if(latcom._p1 == _listaPunti[(_listaTriangoli[fc[1]]._vertici)[i]])
                        tv_i[0] = i;
                    if(latcom._p1 == _listaPunti[(_listaTriangoli[id_nt]._vertici)[i]])
                        nt_i[0] = i;
                    if(latcom._p2 == _listaPunti[(_listaTriangoli[fc[1]]._vertici)[i]])
                        tv_i[1] = i;
                    if(latcom._p2 == _listaPunti[(_listaTriangoli[id_nt]._vertici)[i]])
                        nt_i[1] = i;
                }
                Triangolo tr_v = _listaTriangoli[fc[1]];
                Triangolo tr_n = _listaTriangoli[id_nt];
                for(unsigned int g = 0; g<2; g++){
                    if (_listaPunti[(tr_v._vertici)[(tv_i[g]+1)%3]] != pv[(g+1)%2]){
                        B = (tr_v._vertici)[(tv_i[g]+1)%3];
                        l1 = (tr_v._lati)[tv_i[g]];
                        C = (tr_n._vertici)[(nt_i[(g+1)%2]+1)%3];
                        l2 = (tr_n._lati)[(nt_i[(g+1)%2]+1)%3];
                    }else{
                        B = (tr_n._vertici)[(nt_i[g]+1)%3];
                        l1 = (tr_n._lati)[nt_i[g]];
                        C = (tr_v._vertici)[(tv_i[(g+1)%2]+1)%3];
                        l2 = (tr_v._lati)[(tv_i[(g+1)%2]+1)%3];
                    }
                    for(unsigned int z = 0; z<(_listaLati[l1]._listIdTr).size(); z++)
                        if ((_listaLati[l1]._listIdTr)[z] == ix[(g+1)%2])
                            (_listaLati[l1]._listIdTr)[z] = ix[g];
                    for(unsigned int z = 0; z<(_listaLati[l2]._listIdTr).size(); z++)
                        if ((_listaLati[l2]._listIdTr)[z] == ix[(g+1)%2])
                            (_listaLati[l2]._listIdTr)[z] = ix[g];
                    Triangolo tn = Triangolo(ix[g], pv[g]._id, B, C , l1, ln._id, l2);
                    _listaTriangoli[ix[g]] = tn;
                }
            }
            _codaDelaunay.pop_front();
        }
	}

    bool Mesh::accettabile(const Punto& pnew, const Punto& point)
    {
        unsigned int counter = 0;
        bool result = true;
        unsigned int ilato = _hullBeginLato;
        bool inizio = true;
        Punto dir1 = pnew - point;
        unsigned int iv = 0;
        double x_pt;
        double y_pt;
        while((_hullBeginLato != ilato) || inizio)
        {
            // ricontrollare per l'antisimmetria del prodotto scalare
            Punto dir2 = _listaLati[ilato]._p2 - _listaLati[ilato]._p1;
            if(abs(crossProduct(dir1,dir2)) < Punto::geometricTol) // da rivedere, tolleranza è dei punti, andrà bene uguale?
            {
                // non mi interessa l'id
                // point è la slz del sistema lineare
                x_pt = (((point)._x - pnew._x)*dir2._y-((point)._y - pnew._y)*dir2._x) / crossProduct(dir1,dir2);
                y_pt = (( (point)._y - pnew._y)*dir1._x-((point)._x - pnew._x)*dir1._y)/crossProduct(dir1,dir2);
                Punto pt = Punto(iv, x_pt, y_pt);

                if( (pt._x >= min((_listaLati[ilato]._p1)._x,(_listaLati[ilato]._p2)._x)) &&
                    (pt._x <= max((_listaLati[ilato]._p1)._x,(_listaLati[ilato]._p2)._x)) &&
                    (pt._y >= min((_listaLati[ilato]._p1)._y,(_listaLati[ilato]._p2)._y)) &&
                    (pt._y <= max((_listaLati[ilato]._p1)._y,(_listaLati[ilato]._p2)._y)) &&
                    (pt._x >= min(pnew._x,(point)._x)) && (pt._x <= max(pnew._x,(point)._x)) &&
                    (pt._y >= min(pnew._y,(point)._y)) && (pt._y <= max(pnew._y,(point)._y)) )
                {
                    if(pt == _listaLati[ilato]._p1 || pt == _listaLati[ilato]._p2)
                    {
                        counter = counter + 1;
                        if (counter >2)
                        {
                            result = false;
                            break;
                        }
                    }
                    else
                    {
                        result = false;
                        break;
                    }
                }
            }
            ilato = _listaLati[ilato]._succ;
        }
        return result;
    }

    void Mesh::CollegaSenzaIntersezioni(const Punto& pnew, unsigned int& id_t, unsigned int& id_l)
    {
        // trovo il primo e l'ultimo lato utile
        unsigned int fineLato = _hullBeginLato;
        unsigned int inizioLato = _listaLati[_hullBeginLato]._prec;
        bool cambiaInizio = false;
        bool b = true; // variabile booleana di comodo
        unsigned int varaus;
        Punto paus;
        if(accettabile(pnew, _listaLati[fineLato]._p1))
        {
            while(b)
            {
                fineLato = _listaLati[fineLato]._succ;
                b = accettabile(pnew,_listaLati[fineLato]._p1);
            }
            // in v abbiamo l'utimo false, il prec del puntato di finePunto sarà l'ultimo accettabile
            fineLato = _listaLati[fineLato]._prec;
            if(accettabile(pnew,_listaLati[fineLato]._p1))
            {
                cambiaInizio = true;
                b = true;
                while(b)
                {
                    inizioLato = _listaLati[inizioLato]._prec;
                    b = accettabile(pnew, _listaLati[inizioLato]._p1);
                }
                // in q abbiamo il primo false, il succ del puntato di q sarà il primo accettabile
                inizioLato = _listaLati[inizioLato]._succ;
            }
            else
            {
                inizioLato = _hullBeginLato;
            }
        }
        else
        {
            // il controllo sopra lo faccio su v, se è falso metto comunque in q
            // quello che io misuro come il primo vero, quindi passo a v e modifico
            // opportunamente q, rifaccio iol ciclo
            inizioLato = _listaLati[_hullBeginLato]._succ;
            b = accettabile(pnew, _listaLati[inizioLato]._p1);
            while(!b)
            {
                inizioLato = _listaLati[inizioLato]._succ;
                b = accettabile(pnew,_listaLati[inizioLato]._p1);
            }
            // q a questo punto avrà il primo accettabile
            fineLato = _listaLati[inizioLato]._succ;
            b = accettabile(pnew, _listaLati[fineLato]._p1);
            while(b)
            {
                fineLato = _listaLati[fineLato]._succ;
                b = accettabile(pnew, _listaLati[fineLato]._p1);
            }
            // in v ho il falso, lo riposto all'ultimo vero
            fineLato = _listaLati[fineLato]._prec;
        }

        //creo tutti i lati utili
        vector<Lato> newLati;
        unsigned int copiaLato = inizioLato;
        Lato l;
        // id_t lo modificheremo quando creeremo i triangoli, per ora gli passo
        // fittiziamente solo una variabile che incremento di volta in volta
        unsigned int conta = 0;
        while(copiaLato == _listaLati[fineLato]._succ)
        {
            varaus = id_t + conta;
            paus = _listaLati[copiaLato]._p1;
            l = Lato(id_l, paus, pnew, varaus);

            if((copiaLato == inizioLato)||(_listaLati[copiaLato]._succ == fineLato)){
                conta++;
            }else{
                conta++;
                _listaLati[id_l]._listIdTr.push_back(id_t + conta);
            }
            id_l++;
            newLati.push_back(l);
        }
        // la variabile id_t non è stata cambiata ed è come è stata passata al programma

        // creo triangoli
        // da finire
        copiaLato = inizioLato;
        unsigned int i = 0;
        Triangolo t;
        array<unsigned int, 2> lac; //lati da aggiungere a coda di Delaunay
        while(copiaLato == _listaLati[fineLato]._succ)
        {
            lac = {_listaLati[copiaLato]._id, id_t};
            _codaDelaunay.push_back(lac);
            t = Triangolo(id_t, _listaLati[copiaLato]._id, pnew._id, _listaLati[copiaLato]._succ,
                          newLati[i]._id, newLati[i+1]._id, copiaLato);
            copiaLato = _listaLati[copiaLato]._succ;
            _listaTriangoli.push_back(t);
            i++;
            id_t++;
        }
        // aggiornamento HullLato
        _listaLati[newLati[0]._id]._succ = (newLati[newLati.size()-1])._id;
        _listaLati[(newLati[newLati.size()-1])._id]._prec = newLati[0]._id;
        _listaLati[inizioLato]._succ = newLati[0]._id;
        _listaLati[fineLato]._prec = (newLati[newLati.size()-1])._id;
        // cambia inizio dell'Hull se serve
        if (cambiaInizio) {
            _hullBeginLato= inizioLato;
        }
    }

    void Mesh::PuntoInterno(const Punto& po, unsigned int& itr, unsigned int& id_tr, unsigned int& id_lt)
    {
        array<unsigned int, 2> il_it;
        Triangolo tr =  _listaTriangoli[itr];
        Lato l1 = Lato(id_lt, po, _listaLati[(tr._lati)[0]]._p1, itr);
        unsigned int idl1 = id_lt;
        _listaLati.push_back(l1);
        id_lt++;

        Lato l2 = Lato(id_lt, _listaLati[(tr._lati)[0]]._p2, po, itr);
        unsigned int idl2 = id_lt;
        _listaLati.push_back(l2);
        id_lt++;

        Triangolo tng = Triangolo(itr, po._id, (tr._vertici)[0], (tr._vertici)[1], idl1, (tr._lati)[0], idl2);
        _listaTriangoli[itr] = tng;
        if (_listaLati[(tr._lati)[0]]._listIdTr.size() == 2){
            il_it = {(tr._lati)[0], tng._id};
            _codaDelaunay.push_back(il_it);
        }

        Lato l3 = Lato(id_lt, _listaLati[(tr._lati)[1]]._p2, po, id_tr);
        unsigned int idl3 = id_lt;
        _listaLati.push_back(l3);
        id_lt++;
        (_listaLati[idl2]._listIdTr).push_back(id_tr);
        for(unsigned int i = 0; i<2; i++){
            if((_listaLati[(tr._lati)[1]]._listIdTr)[i] == itr){
                (_listaLati[(tr._lati)[1]]._listIdTr)[i] = tng._id;
                break;
            }
        }
        tng = Triangolo(id_tr, po._id, (tr._vertici)[1], (tr._vertici)[2], idl2, (tr._lati)[1], idl3);
        id_tr++;
        _listaTriangoli.push_back(tng);
        if (_listaLati[(tr._lati)[1]]._listIdTr.size() == 2){
            il_it = {(tr._lati)[1], tng._id};
            _codaDelaunay.push_back(il_it);
        }

        (_listaLati[idl1]._listIdTr).push_back(id_tr);
        (_listaLati[idl3]._listIdTr).push_back(id_tr);
        tng = Triangolo(id_tr, po._id, (tr._vertici)[2], (tr._vertici)[0], idl3, (tr._lati)[2], idl1);
        id_tr++;
        _listaTriangoli.push_back(tng);
        if (_listaLati[(tr._lati)[2]]._listIdTr.size() == 2){
            il_it = {(tr._lati)[2], tng._id};
            _codaDelaunay.push_back(il_it);
        }
        for(unsigned int i = 0; i<2; i++){
            if((_listaLati[(tr._lati)[1]]._listIdTr)[i] == itr){
                (_listaLati[(tr._lati)[1]]._listIdTr)[i] = tng._id;
                break;
            }
        }
    }

    void Mesh::PuntoBordoTriang(const Punto& po, unsigned int& ilt, unsigned int& id_tr, unsigned int& id_lt)
    {
        unsigned int itr;
        unsigned int itx;
        unsigned int cn = 1;
        unsigned int lid;
        Triangolo tr;
        array<unsigned int, 2> il_it;
        Triangolo tng;
        Lato l1;
        Lato l2;
        Lato l3;
        Lato l4;
        bool a1;
        bool a2;
        for(unsigned int k = 0; k<_listaLati[ilt]._listIdTr.size(); k++){
            itr = (_listaLati[ilt]._listIdTr)[k];
            tr = _listaTriangoli[itr];
            for(unsigned int h = 0; h<3; h++){
                if (((po._x*_listaPunti[(tr._vertici)[h]]._y) + (po._y*_listaPunti[(tr._vertici)[(h+1)%3]]._x)
                     + (_listaPunti[(tr._vertici)[h]]._x*_listaPunti[(tr._vertici)[(h+1)%3]]._y)
                     - (_listaPunti[(tr._vertici)[(h+1)%3]]._x*_listaPunti[(tr._vertici)[h]]._y)
                     - (_listaPunti[(tr._vertici)[(h+1)%3]]._y*po._x)
                     - (_listaPunti[(tr._vertici)[h]]._x*po._y)) != 0)
                {
                    switch (cn){
                    case 1:
                        if (_listaPunti[(tr._vertici)[h]] == _listaLati[ilt]._p1 ||
                            _listaPunti[(tr._vertici)[h]] == _listaLati[ilt]._p2){
                            l1 = Lato(ilt, po, _listaPunti[(tr._vertici)[h]], itr);
                            _listaLati[ilt] = l1;
                            l2 = Lato(id_lt, _listaPunti[(tr._vertici)[(h+1)%3]], po, itr);
                            _listaLati.push_back(l2);
                            tng = Triangolo(itr, po._id, (tr._vertici)[h], (tr._vertici)[(h+1)%3],
                                            ilt, (tr._lati)[h], id_lt);
                            a1 = true;
                        } else {
                            l2 = Lato(ilt, po, _listaPunti[(tr._vertici)[h]], itr);
                            _listaLati.push_back(l2);
                            l1 = Lato(id_lt, _listaPunti[(tr._vertici)[(h+1)%3]], po, itr);
                            _listaLati[ilt] = l1;
                            tng = Triangolo(itr, po._id, (tr._vertici)[h], (tr._vertici)[(h+1)%3],
                                            id_lt, (tr._lati)[h], ilt);
                            a1 = false;
                        }
                        _listaTriangoli[itr] = tng;
                        id_lt++;
                        cn++;
                        itx = itr;
                        break;
                    case 2:
                        if(a1){
                            l3 = Lato(id_lt, po,  _listaPunti[(tr._vertici)[(h+1)%3]], id_tr);
                            tng = Triangolo(id_tr, po._id, (tr._vertici)[h], (tr._vertici)[(h+1)%3],
                                            l2._id, (tr._lati)[h], id_lt);
                        } else {
                            l3 = Lato(id_lt, po,  _listaPunti[(tr._vertici)[h]], id_tr);
                            tng = Triangolo(id_tr, po._id, (tr._vertici)[h], (tr._vertici)[(h+1)%3],
                                            id_lt, (tr._lati)[h], l2._id);
                        }
                        _listaLati[l2._id]._listIdTr.push_back(id_tr);
                        _listaLati.push_back(l3);
                        _listaTriangoli.push_back(tng);
                        itx = id_tr;
                        id_lt++;
                        id_tr++;
                        cn++;
                        break;
                    case 3:
                        if (_listaPunti[(tr._vertici)[h]] == _listaLati[ilt]._p1 ||
                            _listaPunti[(tr._vertici)[h]] == _listaLati[ilt]._p2){
                            l4 = Lato(id_lt, po,  _listaPunti[(tr._vertici)[(h+1)%2]], itr);
                            a2 = true;
                            if(a1)
                                lid = l3._id;
                            else
                                lid = l1._id;
                            tng = Triangolo(itr, po._id, (tr._vertici)[h], (tr._vertici)[(h+1)%3],
                                            lid, (tr._lati)[h], id_lt);
                        } else {
                            a2 = false;
                            l4 = Lato(id_lt, po,  _listaPunti[(tr._vertici)[h]], itr);
                            if(a1)
                                lid = l1._id;
                            else
                                lid = l3._id;
                            tng = Triangolo(itr, po._id, (tr._vertici)[h], (tr._vertici)[(h+1)%3],
                                            id_lt, (tr._lati)[h], lid);
                        }
                        _listaLati[lid]._listIdTr.push_back(itr);
                        _listaLati.push_back(l4);
                        _listaTriangoli[itr] = tng;
                        id_lt++;
                        cn++;
                        itx = itr;
                        break;
                    case 4:
                        if(a2){
                            if(a1)
                                lid = l1._id;
                            else
                                lid = l3._id;
                            tng = Triangolo(id_tr, po._id, (tr._vertici)[h], (tr._vertici)[(h+1)%3],
                                            l4._id, (tr._lati)[h], lid);
                        } else {
                            if(a1)
                                lid = l3._id;
                            else
                                lid = l1._id;
                            tng = Triangolo(id_tr, po._id, (tr._vertici)[h], (tr._vertici)[(h+1)%3],
                                            lid, (tr._lati)[h], l4._id);
                        }
                        _listaLati[lid]._listIdTr.push_back(id_tr);
                        _listaLati[l4._id]._listIdTr.push_back(id_tr);
                        _listaTriangoli.push_back(tng);
                        itx = id_tr;
                        id_tr++;
                        break;
                    }
                    for (unsigned int j = 0; j< _listaLati[(tr._lati)[h]]._listIdTr.size(); j++){
                        if ((_listaLati[(tr._lati)[h]]._listIdTr)[j] == itr)
                            (_listaLati[(tr._lati)[h]]._listIdTr)[j] = itx;
                    }
                    if(_listaLati[(tr._lati)[h]]._listIdTr.size() == 2){
                        il_it = {(tr._lati)[h], itx};
                        _codaDelaunay.push_back(il_it);
                    }
                }
            }
        }
    }

    void Mesh::PuntoBordoHull(const Punto& po, unsigned int& ilt, unsigned int& id_tr, unsigned int& id_lt)
    {
        Lato l = _listaLati[ilt];
        array<unsigned int, 2> il_it;
        bool cambio_inizio = false;
        if(_hullBeginLato == ilt)
            cambio_inizio =true;
        unsigned int itr = (_listaLati[ilt]._listIdTr)[0];
        unsigned int pos;
        Triangolo tr = _listaTriangoli[itr];
        unsigned int pnl;
        for (unsigned h = 0; h<3; h++){
            if(_listaPunti[(_listaTriangoli[itr]._vertici)[h]] != _listaLati[ilt]._p1 ||
                _listaPunti[(_listaTriangoli[itr]._vertici)[h]] != _listaLati[ilt]._p2)
            {
                pnl = (_listaTriangoli[itr]._vertici)[h];
                pos = h;
            }
        }
        Lato l1 = Lato(ilt, _listaLati[ilt]._p1, po, itr);
        _listaLati[ilt] = l1;
        Lato l2 = Lato(id_lt, po, _listaPunti[pnl], itr);
        _listaLati.push_back(l2);
        Triangolo tng = Triangolo(itr, _listaLati[ilt]._p1._id, po._id, pnl, ilt, (tr._lati)[pos], id_lt);
        _listaTriangoli[itr] = tng;
        id_lt++;
        if(_listaLati[(tr._lati)[pos]]._listIdTr.size() == 2){
            il_it = {(tr._lati)[pos], itr};
            _codaDelaunay.push_back(il_it);
        }
        Lato l3 = Lato(id_lt, po, _listaLati[ilt]._p1, id_tr);
        _listaLati.push_back(l3);
        tng = Triangolo(id_tr, po._id, _listaLati[ilt]._p2._id, pnl, id_lt, (tr._lati)[(pos+2)%3],
                        l2._id);
        (_listaLati[l2._id]._listIdTr).push_back(id_tr);
        _listaTriangoli.push_back(tng);
        for (unsigned int j = 0; j< _listaLati[(tr._lati)[pos-1]]._listIdTr.size(); j++){
            if ((_listaLati[(tr._lati)[(pos+2)%3]]._listIdTr)[j] == itr)
                (_listaLati[(tr._lati)[(pos+2)%3]]._listIdTr)[j] = id_tr;
        }
        if(_listaLati[(tr._lati)[(pos+2)%3]]._listIdTr.size() == 2){
            il_it = {(tr._lati)[(pos+2)%3], id_tr};
            _codaDelaunay.push_back(il_it);
        }
        id_tr++;
        id_lt++;
        _listaLati[l1._id]._prec = l._prec;
        _listaLati[l3._id]._prec = l1._id;
        _listaLati[l1._id]._succ = l3._id;
        _listaLati[l3._id]._succ = l._succ;
        if(cambio_inizio)
            _hullBeginLato = l1._id;
    }

    Mesh::Mesh(const vector<Punto>& listaPunti):
        _listaPunti(listaPunti)
    {
        // aggiungere inizializzazione di esterni
        unsigned int idlato = 0;
        unsigned int idtriang = 0;
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
        double Area = ((punti_scelti[0]._x*punti_scelti[1]._y) + (punti_scelti[0]._y*punti_scelti[2]._x)
                        + (punti_scelti[1]._x*punti_scelti[2]._y) - (punti_scelti[2]._x*punti_scelti[1]._y)
                        - (punti_scelti[2]._y*punti_scelti[0]._x) - (punti_scelti[1]._x*punti_scelti[0]._y));

        if (Area < 0)       //TOLLERANZA
        {
            Punto a = punti_scelti[1];
            punti_scelti[1] = punti_scelti[2];
            punti_scelti[2] = a;
        }
        _listaPunti[punti_scelti[0]._id]._inserito = true;
        _listaPunti[punti_scelti[1]._id]._inserito = true;
        _listaPunti[punti_scelti[2]._id]._inserito = true;
        Lato l;
        for(unsigned int i = 0; i<3; i++){
            l = Lato(idlato, punti_scelti[i], punti_scelti[(i+1)%3], idtriang);
            _listaLati.push_back(l);
            idlato++;
        }
        _listaLati[0]._prec = 2;
        _listaLati[1]._prec = 0;
        _listaLati[2]._prec = 1;
        _listaLati[0]._succ = 1;
        _listaLati[1]._succ = 2;
        _listaLati[2]._succ = 0;
        Triangolo tr = Triangolo(idtriang, punti_scelti[0]._id , punti_scelti[1]._id, punti_scelti[2]._id,
                                 _listaLati[0]._id, _listaLati[1]._id, _listaLati[2]._id);
        idtriang++;
        _listaTriangoli.push_back(tr);

        Triangolo tng;
        array<unsigned int, 2> DM;
        Punto po;        
        for(unsigned int i = 0; i<_listaPunti.size(); i++){
            po = _listaPunti[i];
            if (!(po._inserito)){
                DM = this->DentroMesh(po);

                switch (DM[0]) {
                case 0: {//punto interno
                    this->PuntoInterno(po, DM[1], idtriang, idlato);                    
                    break;
                }
                case 1: {//sul bordo del triangolo
                    this->PuntoBordoTriang(po, DM[1], idtriang, idlato);
                    break;
                }
                case 2: { //bordo hull
                    this->PuntoBordoHull(po, DM[1], idtriang, idlato);
                    break;
                }

                case 4: {//esterno
                    this->CollegaSenzaIntersezioni(po, idtriang, idlato);
                    break;
                }
                }
                if(!(_codaDelaunay.empty()))
                    this->ControlloDelaunay();
            }
        }
        cout<<"Mesh Costruita"<<endl;
    }

    string Mesh::Show()
    {
        string result = "Punti\n";
        result = result  + "Id x y\n";
        for(unsigned int k = 0; k<_listaPunti.size(); k++){
            result = result + _listaPunti[k].Show() + "\n";
        }
        result = result + "Lati\n";
        result = result + "Id p1 p2 Length TriangoliAdiacenti\n";
        for(unsigned int k = 0; k<_listaLati.size(); k++){
           result = result + _listaLati[k].Show() + "\n";
        }
        result = result + "Triangoli\n";
        result = result + "Id p1 p2 p3 l1 l2 l3\n";
        for(unsigned int k = 0; k<_listaTriangoli.size(); k++){
            result = result + to_string(_listaTriangoli[k]._id) + " " + to_string((_listaTriangoli[k]._vertici)[0])
                    + " " + to_string((_listaTriangoli[k]._vertici)[1]) + " "
                    + to_string((_listaTriangoli[k]._vertici)[2]) + " "
                    + to_string((_listaTriangoli[k]._lati)[0]) + " " + to_string((_listaTriangoli[k]._lati)[1])
                    + " " + to_string((_listaTriangoli[k]._lati)[2]) + "\n";
        }
        return result;
    }

    bool IOMesh::ImportPunti(vector<Punto>& listaPunti, const string& FileName){
        ifstream file;
        file.open(FileName);
        if(file.fail()){
            cerr<< "file open failed"<< endl;
            return false;
        }

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
        cout<<"Punti Importati"<<endl;
        return true;
    }

    bool IOMesh::ExportMesh(const Mesh& mesh , const string& OutFilePath, const string& OutFileNameParz)
    {
        ofstream file;
        file.open(OutFilePath + "Lati_Mesh_" + OutFileNameParz);
        if (file.fail())
            return false;

        file << "id p1 p2 length\n";

        for(unsigned int i = 0; i < ((mesh._listaLati).size()-1) ; i++){
            file << mesh._listaLati[i]._id << " " << mesh._listaLati[i]._p1._id << " "
                 << mesh._listaLati[i]._p2._id << " " << mesh._listaLati[i]._length << endl;
        }
        file.close();

        // dubbio, posso "sovrascrivere" sull'oggetto ofstream ?
        file.open(OutFilePath + "Triangoli_Mesh_" + OutFileNameParz);
        if (file.fail())
            return false;

        file << "id_triangolo p1 p2 p3 l1 l2 l3\n" << endl;

        for(unsigned int i = 0; i < ((mesh._listaLati).size()-1) ; i++){
            file << mesh._listaTriangoli[i]._id << " ";

            for (unsigned int j=0; j < 3 ; j++){
                file << mesh._listaTriangoli[i]._vertici[j] << " ";
            }

            for (unsigned int j=0; j < 3 ; j++){
                // faccio questi if perchè non voglio lo spazio a fine della riga
                if ( j == 2)
                    file << endl;
                else
                    file << mesh._listaTriangoli[i]._lati[j] << " ";
            }
        }
        file.close();
        cout<<"Mesh Esportata"<<endl;
        return true;
    }
}


