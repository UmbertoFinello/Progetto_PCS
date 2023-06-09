#include "Delaunay_class.hpp"
#include "sorting.hpp"

namespace ProjectLibrary
{
    Punto::Punto(unsigned int& id , double& x, double& y):
        _id(id) , _x(x) , _y(y)
    {}

    Punto::Punto(const Punto& p):
        _id(p._id), _x(p._x), _y(p._y)
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
        _id(lat._id) , _p1(lat._p1) , _p2(lat._p2), _length(lat._length), _listIdTr(lat._listIdTr),
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
        Lato* pointer = _hullBeginLato;
        bool inizio = true; // variabile per non uscire alla prima iteraz.
        Punto v1 = (*pointer)._p2 - (*pointer)._p1;
        Punto v2 = p - (*pointer)._p1;
        while((pointer != _hullBeginLato) || (inizio==true)){
            inizio=false;
            if(abs(crossProduct(v2,v1))<Punto::geometricTol){
                result[0]=2;
                result[1]=(*pointer)._id;
                return result
            }
            else if (crossProduct(v2,v1)>0){
                result[0]=3;
                return result;
            }
            pointer = (*pointer)._succ;
        }
        array <unsigned int, 3> idPunti;
        array <unsigned int, 3> idLati;
        bool ext = true; // true: punto fuori triangolo
        Lato l = (*_hullBeginLato);
        unsigned int idCorr=l._listIdTr[0]; // id triangolo vecchio per ricerca
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
            return result;
        }
    }

    void Mesh::ControlloDelaunay()
    {
        array<unsigned int, 2> fc; //fronte coda
        array<unsigned int, 2> nec; //nuovi elementi coda
        array<unsigned int, 2> ix; //indici triangoli
        array<unsigned int, 2> tv_i;
        array<unsigned int, 2> nt_i;
        array<Punto, 2> pv;
        unsigned int B;
        unsigned int l1;
        unsigned int C;
        unsigned int l2;
        unsigned int id_nt; //id nuovo triangolo controllo
        while(!(_codaDelaunay.empty())){
            fc = _codaDelaunay.front(); //indice coda
            ix[0] = fc[1];
            Lato latcom = _listaLati[fc[0]]; //lato in comune
            pv[0] = latcom._p1;
            pv[1] = latcom._p2;
            if(latcom._listIdTr.size() == 2){
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
                        if ((_listaTriangoli[id_nt]._lati)[j] != fc[0])
                            nec = {(_listaTriangoli[id_nt]._lati)[j], id_nt};
                        _codaDelaunay.push_back(nec);
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
                    for(unsigned int g = 0; g<2; g++){
                        if (_listaPunti[(_listaTriangoli[fc[1]]._vertici)[tv_i[g]+1]] != pv[g]){
                            B = (_listaTriangoli[fc[1]]._vertici)[tv_i[g]+1];
                            l1 = (_listaTriangoli[fc[1]]._lati)[tv_i[g]];
                            C = (_listaTriangoli[id_nt]._vertici)[nt_i[g]+1];
                            l2 = (_listaTriangoli[id_nt]._lati)[nt_i[g]];
                        }else{
                            B = (_listaTriangoli[id_nt]._vertici)[nt_i[g]+1];
                            l1 = (_listaTriangoli[id_nt]._lati)[nt_i[g]];
                            C = (_listaTriangoli[fc[1]]._vertici)[tv_i[g]+1];
                            l2 = (_listaTriangoli[fc[1]]._lati)[tv_i[g]];
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
            }
            _codaDelaunay.pop_front();
        }
	}

    bool Mesh::accettabile(const Punto& pnew, const Punto& point)
    {
        unsigned int counter = 0;
        bool result = true;
        Lato* lato = _hullBeginLato;
        bool inizio = true;
        Punto dir1 = pnew - point;
        unsigned int iv = 0;
        double x_pt;
        double y_pt;
        while((_hullBeginLato != lato) || (inizio==true))
        {
            // ricontrollare per l'antisimmetria del prodotto scalare
            Punto dir2 = (*lato)._p2 - (*lato)._p1;
            if(abs(crossProduct(dir1,dir2)) < Punto::geometricTol) // da rivedere, tolleranza è dei punti, andrà bene uguale?
            {
                // non mi interessa l'id
                // point è la slz del sistema lineare
                x_pt = (((point)._x - pnew._x)*dir2._y-((point)._y - pnew._y)*dir2._x) / crossProduct(dir1,dir2);
                y_pt = (( (point)._y - pnew._y)*dir1._x-((point)._x - pnew._x)*dir1._y)/crossProduct(dir1,dir2);
                Punto pt = Punto(iv, x_pt, y_pt);

                if( (pt._x >= min(((*lato)._p1)._x,((*lato)._p2)._x)) &&
                    (pt._x <= max(((*lato)._p1)._x,((*lato)._p2)._x)) &&
                    (pt._y >= min(((*lato)._p1)._y,((*lato)._p2)._y)) &&
                    (pt._y <= max(((*lato)._p1)._y,((*lato)._p2)._y)) &&
                    (pt._x >= min(pnew._x,(point)._x)) && (pt._x <= max(pnew._x,(point)._x)) &&
                    (pt._y >= min(pnew._y,(point)._y)) && (pt._y <= max(pnew._y,(point)._y)) )
                {
                    if(pt == (*lato)._p1 || pt == (*lato)._p2)
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
            lato = (*lato)._succ;
        }
        return result;
    }

    void Mesh::CollegaSenzaIntersezioni(const Punto& pnew, unsigned int& id_t, unsigned int& id_l)
    {
        // trovo il primo e l'ultimo lato utile
        Lato* fineLato = _hullBeginLato;
        Lato* inizioLato = (*_hullBeginLato)._prec;
        bool cambiaInizio = false;
        bool b = true; // variabile booleana di comodo
        unsigned int varaus;
        Punto paus;
        if(accettabile(pnew,(*fineLato)._p1))
        {
            while(b)
            {
                fineLato = (*fineLato)._succ;
                b = accettabile(pnew,(*fineLato)._p1);
            }
            // in v abbiamo l'utimo false, il prec del puntato di finePunto sarà l'ultimo accettabile
            fineLato = (*fineLato)._prec;
            if(accettabile(pnew,(*fineLato)._p1))
            {
                cambiaInizio = true;
                b = true;
                while(b)
                {
                    inizioLato = (*inizioLato)._prec;
                    b = accettabile(pnew,(*inizioLato)._p1);
                }
                // in q abbiamo il primo false, il succ del puntato di q sarà il primo accettabile
                inizioLato = (*inizioLato)._succ;
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
            inizioLato = (*_hullBeginLato)._succ;
            b = accettabile(pnew,(*inizioLato)._p1);
            while(!b)
            {
                inizioLato = (*inizioLato)._succ;
                b = accettabile(pnew,(*inizioLato)._p1);
            }
            // q a questo punto avrà il primo accettabile
            fineLato = (*inizioLato)._succ;
            b = accettabile(pnew,(*fineLato)._p1);
            while(b)
            {
                fineLato = (*fineLato)._succ;
                b = accettabile(pnew,(*fineLato)._p1);
            }
            // in v ho il falso, lo riposto all'ultimo vero
            fineLato = (*fineLato)._prec;
        }

        // mi creo tutti i lati utili
        vector<Lato> newLati;
        Lato* copiaLato = inizioLato;
        Lato l;
        // id_t lo modificheremo quando creeremo i triangoli, per ora gli passo
        // fittiziamente solo una variabile che incremento di volta in volta
        unsigned int conta = 0;
        while(copiaLato == (*fineLato)._succ)
        {
            varaus = id_t + conta;
            paus = (*copiaLato)._p1;
            l = Lato(id_l, paus, pnew, varaus);

            if((copiaLato == inizioLato)||((*copiaLato)._succ == fineLato)){
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
        while(copiaLato == (*fineLato)._succ)
        {
            lac = {(*copiaLato)._id, id_t};
            _codaDelaunay.push_back(lac);
            t = Triangolo(id_t,(*copiaLato)._id,pnew._id,((*copiaLato)._succ)->_id, newLati[i]._id,newLati[i+1]._id,(*copiaLato)._id);
            copiaLato = (*copiaLato)._succ;
            _listaTriangoli.push_back(t);
            i++;
            id_t++;
        }
        // aggiornamento HullLato
        _listaLati[newLati[0]._id]._succ = &_listaLati[(newLati[newLati.size()-1])._id];
        _listaLati[(newLati[newLati.size()-1])._id]._prec = &_listaLati[newLati[0]._id];
        (*inizioLato)._succ = &_listaLati[newLati[0]._id];
        (*fineLato)._prec = &_listaLati[(newLati[newLati.size()-1])._id];
        // cambia inizio dell'Hull se serve
        if (cambiaInizio) {
            _hullBeginLato= inizioLato;
        }
    }

    Mesh::Mesh(const vector<Punto>& listaPunti):
        _listaPunti(listaPunti)
    {
        // aggiungere inizializzazione di esterni
        unsigned int idlato = 0;
        unsigned int idtriang = 0;
        list<Punto> PuntiNonEstr;
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
        Lato l;
        for(unsigned int i = 0; i<3; i++){
            l = Lato(idlato,_listaPunti[i],_listaPunti[(i+1)%3], idtriang);
            _listaLati.push_back(l);
            idlato++;
        }
        _hullBeginLato = &_listaLati[0];
        _listaLati[0]._prec = &_listaLati[2];
        _listaLati[1]._prec = &_listaLati[0];
        _listaLati[2]._prec = &_listaLati[1];
        _listaLati[0]._succ = &_listaLati[1];
        _listaLati[1]._succ = &_listaLati[2];
        _listaLati[2]._succ = &_listaLati[0];
        Triangolo tr = Triangolo(idtriang, _listaPunti[0]._id ,_listaPunti[1]._id, _listaPunti[2]._id,
                                 _listaLati[0]._id, _listaLati[1]._id, _listaLati[2]._id);
        idtriang++;
        _listaTriangoli.push_back(tr);

        for(unsigned int j = 0; j<listaPunti.size(); j++){
            if(!(listaPunti[j] == punti_scelti[0] || listaPunti[j] == punti_scelti[1] ||
                 listaPunti[j] == punti_scelti[2]))
            PuntiNonEstr.push_back(listaPunti[j]);
        }

        unsigned int idl1;
        unsigned int idl2;
        unsigned int idl3;
        Triangolo tng;
        array<unsigned int, 2> il_it;
        array<unsigned int, 2> DM;
        Lato l1;
        Lato l2;
        Lato l3;
        Lato l4;
        for(Punto po : PuntiNonEstr){
            DM = this->DentroMesh(po);
            switch (DM[0]) {
            case 0: {//punto interno
                tr =  _listaTriangoli[DM[1]];
                l1 = Lato(idlato, po, _listaLati[(tr._lati)[0]]._p1, DM[1]);
                idl1 =idlato;
                _listaLati.push_back(l1);
                idlato++;

                l2 = Lato(idlato, _listaLati[(tr._lati)[0]]._p2, po, DM[1]);
                idl2 = idlato;
                _listaLati.push_back(l2);
                idlato++;

                tng = Triangolo(DM[1], po._id, (tr._vertici)[0], (tr._vertici)[1], idl1, (tr._lati)[0], idl2);
                _listaTriangoli[DM[1]] = tng;
                il_it = {(tr._lati)[0], tng._id};
                _codaDelaunay.push_back(il_it);

                l3 = Lato(idlato, _listaLati[(tr._lati)[1]]._p2, po, idtriang);
                idl3 = idlato;
                _listaLati.push_back(l3);
                idlato++;
                (_listaLati[idl2]._listIdTr).push_back(idtriang);
                for(unsigned int i = 0; i<2; i++){
                    if((_listaLati[(tr._lati)[1]]._listIdTr)[i] == DM[1]){
                        (_listaLati[(tr._lati)[1]]._listIdTr)[i] = tng._id;
                        break;
                    }
                }
                tng = Triangolo(idtriang, po._id, (tr._vertici)[1], (tr._vertici)[2], idl2, (tr._lati)[1], idl3);
                idtriang++;
                _listaTriangoli.push_back(tng);
                il_it = {(tr._lati)[1], tng._id};
                _codaDelaunay.push_back(il_it);

                (_listaLati[idl1]._listIdTr).push_back(idtriang);
                (_listaLati[idl3]._listIdTr).push_back(idtriang);
                tng = Triangolo(idtriang, po._id, (tr._vertici)[2], (tr._vertici)[0], idl3, (tr._lati)[2], idl1);
                idtriang++;
                _listaTriangoli.push_back(tng);
                il_it = {(tr._lati)[2], tng._id};
                _codaDelaunay.push_back(il_it);
                for(unsigned int i = 0; i<2; i++){
                    if((_listaLati[(tr._lati)[1]]._listIdTr)[i] == DM[1]){
                        (_listaLati[(tr._lati)[1]]._listIdTr)[i] = tng._id;
                        break;
                    }
                }
                break;
            }
            case 1: {//sul bordo del triangolo
                unsigned int idtr;
                unsigned int itx;
                unsigned int cn = 1;
                unsigned int lid;
                bool a1;
                bool a2;
                array<unsigned int, 2> addcoda;
                for(unsigned int k = 0; k<_listaLati[DM[1]]._listIdTr.size(); k++){
                    idtr = (_listaLati[DM[1]]._listIdTr)[k];
                    tr = _listaTriangoli[idtr];
                    for(unsigned int h = 0; h<3; h++){
                        if (((po._x*_listaPunti[(tr._vertici)[h]]._y) + (po._y*_listaPunti[(tr._vertici)[(h+1)%3]]._x)
                             + (_listaPunti[(tr._vertici)[h]]._x*_listaPunti[(tr._vertici)[(h+1)%3]]._y)
                             - (_listaPunti[(tr._vertici)[(h+1)%3]]._x*_listaPunti[(tr._vertici)[h]]._y)
                             - (_listaPunti[(tr._vertici)[(h+1)%3]]._y*po._x)
                             - (_listaPunti[(tr._vertici)[h]]._x*po._y)) != 0)
                        {
                            switch (cn){
                            case 1:
                                if (_listaPunti[(tr._vertici)[h]] == _listaLati[DM[1]]._p1 ||
                                    _listaPunti[(tr._vertici)[h]] == _listaLati[DM[1]]._p2){
                                        l1 = Lato(DM[1], po, _listaPunti[(tr._vertici)[h]], idtr);
                                        _listaLati[DM[1]] = l1;
                                        l2 = Lato(idlato, _listaPunti[(tr._vertici)[(h+1)%3]], po, idtr);
                                        _listaLati.push_back(l2);
                                        tng = Triangolo(idtr, po._id, (tr._vertici)[h], (tr._vertici)[(h+1)%3],
                                                        DM[1], (tr._lati)[h], idlato);
                                        a1 = true;
                                } else {
                                        l2 = Lato(DM[1], po, _listaPunti[(tr._vertici)[h]], idtr);
                                        _listaLati.push_back(l2);
                                        l1 = Lato(idlato, _listaPunti[(tr._vertici)[(h+1)%3]], po, idtr);
                                        _listaLati[DM[1]] = l1;
                                        tng = Triangolo(idtr, po._id, (tr._vertici)[h], (tr._vertici)[(h+1)%3],
                                                        idlato, (tr._lati)[h], DM[1]);
                                        a1 = false;
                                }
                                _listaTriangoli[idtr] = tng;
                                idlato++;
                                cn++;
                                itx = idtr;
                                break;
                            case 2:
                                if(a1){
                                    l3 = Lato(idlato, po,  _listaPunti[(tr._vertici)[(h+1)%3]], idtriang);
                                    tng = Triangolo(idtriang, po._id, (tr._vertici)[h], (tr._vertici)[(h+1)%3],
                                                    l2._id, (tr._lati)[h], idlato);                                    
                                } else {
                                    l3 = Lato(idlato, po,  _listaPunti[(tr._vertici)[h]], idtriang);
                                    tng = Triangolo(idtriang, po._id, (tr._vertici)[h], (tr._vertici)[(h+1)%3],
                                                    idlato, (tr._lati)[h], l2._id);
                                }
                                _listaLati[l2._id]._listIdTr.push_back(idtriang);
                                _listaLati.push_back(l3);
                                _listaTriangoli.push_back(tng);
                                itx = idtriang;
                                idlato++;
                                idtriang++;
                                cn++;
                                break;
                            case 3:
                                if (_listaPunti[(tr._vertici)[h]] == _listaLati[DM[1]]._p1 ||
                                    _listaPunti[(tr._vertici)[h]] == _listaLati[DM[1]]._p2){
                                    l4 = Lato(idlato, po,  _listaPunti[(tr._vertici)[(h+1)%2]], idtr);
                                    a2 = true;
                                    if(a1)
                                        lid = l3._id;
                                    else
                                        lid = l1._id;
                                    tng = Triangolo(idtr, po._id, (tr._vertici)[h], (tr._vertici)[(h+1)%3],
                                                    lid, (tr._lati)[h], idlato);
                                } else {
                                    a2 = false;
                                    l4 = Lato(idlato, po,  _listaPunti[(tr._vertici)[h]], idtr);
                                    if(a1)
                                        lid = l1._id;
                                    else
                                        lid = l3._id;
                                    tng = Triangolo(idtr, po._id, (tr._vertici)[h], (tr._vertici)[(h+1)%3],
                                                    idlato, (tr._lati)[h], lid);
                                }
                                _listaLati[lid]._listIdTr.push_back(idtr);
                                _listaLati.push_back(l4);
                                _listaTriangoli[idtr] = tng;
                                idlato++;
                                cn++;
                                itx = idtr;
                                break;
                            case 4:
                                if(a2){
                                    if(a1)
                                        lid = l1._id;
                                    else
                                        lid = l3._id;
                                    tng = Triangolo(idtriang, po._id, (tr._vertici)[h], (tr._vertici)[(h+1)%3],
                                                    l4._id, (tr._lati)[h], lid);
                                } else {
                                    if(a1)
                                        lid = l3._id;
                                    else
                                        lid = l1._id;
                                    tng = Triangolo(idtriang, po._id, (tr._vertici)[h], (tr._vertici)[(h+1)%3],
                                                    lid, (tr._lati)[h], l4._id);
                                }
                                _listaLati[lid]._listIdTr.push_back(idtriang);
                                _listaLati[l4._id]._listIdTr.push_back(idtriang);
                                _listaTriangoli.push_back(tng);
                                itx = idtriang;
                                idtriang++;
                                break;
                            }
                            for (unsigned int j = 0; j< _listaLati[(tr._lati)[h]]._listIdTr.size(); j++){
                                if ((_listaLati[(tr._lati)[h]]._listIdTr)[j] == idtr)
                                    (_listaLati[(tr._lati)[h]]._listIdTr)[j] = itx;
                            }
                            addcoda = {(tr._lati)[h], itx};
                            _codaDelaunay.push_back(addcoda);
                        }
                    }
                }
                break;
            }
            case 2: { //bordo hull
                Lato l = _listaLati[DM[1]];
                unsigned int idtr = (_listaLati[DM[1]]._listIdTr)[0];
                unsigned int pos;
                tr = _listaTriangoli[idtr];
                unsigned int pnl;
                for (unsigned h = 0; h<3; h++){
                    if(_listaPunti[(_listaTriangoli[idtr]._vertici)[h]] != _listaLati[DM[1]]._p1 ||
                       _listaPunti[(_listaTriangoli[idtr]._vertici)[h]] != _listaLati[DM[1]]._p2)
                    {
                        pnl = (_listaTriangoli[idtr]._vertici)[h];
                        pos = h;
                    }
                }
                l1 = Lato(DM[1], _listaLati[DM[1]]._p1, po, idtr);
                _listaLati[DM[1]] = l1;
                l2 = Lato(idlato, po, _listaPunti[pnl], idtr);
                _listaLati.push_back(l2);
                tng = Triangolo(idtr, _listaLati[DM[1]]._p1._id, po._id, pnl, DM[1], (tr._lati)[pos], idlato);
                _listaTriangoli[idtr] = tng;
                 idlato++;
                l3 = Lato(idlato, po, _listaLati[DM[1]]._p1, idtriang);
                _listaLati.push_back(l3);
                tng = Triangolo(idtriang, po._id, _listaLati[DM[1]]._p2._id, pnl, idlato, (tr._lati)[pos-1],
                                l2._id);
                (_listaLati[l2._id]._listIdTr).push_back(idtriang);
                _listaTriangoli.push_back(tng);
                for (unsigned int j = 0; j< _listaLati[(tr._lati)[pos-1]]._listIdTr.size(); j++){
                    if ((_listaLati[(tr._lati)[pos-1]]._listIdTr)[j] == idtr)
                        (_listaLati[(tr._lati)[pos-1]]._listIdTr)[j] = idtriang;
                }
                idtriang++;
                idlato++;
                _listaLati[l1._id]._prec = l._prec;
                _listaLati[l3._id]._prec = &_listaLati[l1._id];
                _listaLati[l1._id]._succ = &_listaLati[l3._id];
                _listaLati[l3._id]._succ = l._succ;
                break;
            }

            case 4: {//esterno
                this->CollegaSenzaIntersezioni(po, idtriang, idlato);
                break;
            }
            }
            this->ControlloDelaunay();
        }
    }

    string Mesh::Show()
    {
        string result = "Punti\n";
        result + "Id x y\n";
        for(unsigned int k = 0; k<_listaPunti.size(); k++){
            result + _listaPunti[k].Show() + "\n";
        }
        result + "Lati\n";
        result + "Id p1 p2 TriangoliAdiacenti\n";
        for(unsigned int k = 0; k<_listaLati.size(); k++){
            result + _listaLati[k].Show() + "\n";
        }
        result + "Triangoli\n";
        result + "Id p1 p2 p3 l1 l2 l3\n";
        for(unsigned int k = 0; k<_listaTriangoli.size(); k++){
            result + to_string(_listaTriangoli[k]._id) + " " + to_string((_listaTriangoli[k]._vertici)[0])
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

        return true;
    }

    bool IOMesh::ExportMesh(const Mesh& mesh , const string& OutFilePath)
    {
        ofstream file;
        file.open(OutFilePath + "Lati_Mesh.csv");
        if (file.fail())
            return false;

        file << "id p1 p2 length\n";

        for(unsigned int i = 0; i < ((mesh._listaLati).size()-1) ; i++){
            file << mesh._listaLati[i]._id << " " << mesh._listaLati[i]._p1._id << " "
                 << mesh._listaLati[i]._p2._id << " " << mesh._listaLati[i]._length << endl;
        }
        file.close();

        // dubbio, posso "sovrascrivere" sull'oggetto ofstream ?
        file.open(OutFilePath + "Triangoli_Mesh.csv");
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
        return true;
    }
}


