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
        _length = Norm(p1._x - p2._x,p1._y - p2._y);
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

    unsigned int Mesh::DentroMesh(const Punto& p, Posizione& DM)
    {
        double mx;
        double Mx;
        double my;
        double My;
        unsigned int counter = 0;
        unsigned int result = 0;// {tipo, id lati o triangoli}
        // ricerca esterno/interno
        unsigned int pointer = _hullBeginLato;
        bool inizio = true; // variabile per non uscire alla prima iteraz.
        Punto v1;
        Punto v2;
        while((pointer != _hullBeginLato) || inizio){
            inizio=false;
            v1 = _listaLati[pointer]._p2 - _listaLati[pointer]._p1;
            v2 = p - _listaLati[pointer]._p1;
            mx = min(_listaLati[pointer]._p1._x, _listaLati[pointer]._p2._x) - Punto::geometricTol;
            Mx = max(_listaLati[pointer]._p1._x, _listaLati[pointer]._p2._x) + Punto::geometricTol;
            my = min(_listaLati[pointer]._p1._y, _listaLati[pointer]._p2._y) - Punto::geometricTol;
            My = max(_listaLati[pointer]._p1._y, _listaLati[pointer]._p2._y) + Punto::geometricTol;
            if(abs(crossProduct(v2,v1))<Punto::geometricTol && (p._x > mx) && (p._y > my) && (p._x < Mx)
                && (p._y < My))
            {
                DM = Posizione::HULL;
                result = pointer;
                return result;
            }
            else if (crossProduct(v2,v1)>0){
                DM = Posizione::ESTERNO;
                return result;
            }
            pointer = _listaLati[pointer]._succ;
        }
        array <unsigned int, 3> idPunti;
        array <unsigned int, 3> idLati;
        for(unsigned int idCorr = 0; idCorr <_listaTriangoli.size(); idCorr++)
        {
            counter = 0;
            idPunti = (_listaTriangoli[idCorr])._vertici; //id dei punti del triangolo
            idLati  = (_listaTriangoli[idCorr])._lati; //id dei lati del triangolo
            // verifico che il punto sia interno al triangolo considerato _A_

            for(unsigned int i = 0; i<3; i++)
            {
               // passo dai punti perchè non ho la certezza che i lati siano ordinati
               // bene per fareil prodotto vettoriale
               v1 = _listaPunti[idPunti[(i+1)%3]]-_listaPunti[idPunti[i]];
               v2 = p - _listaPunti[idPunti[i]];
               mx = min(_listaPunti[idPunti[i]]._x, _listaPunti[idPunti[(i+1)%3]]._x) - Punto::geometricTol;
               Mx = max(_listaPunti[idPunti[i]]._x, _listaPunti[idPunti[(i+1)%3]]._x) + Punto::geometricTol;
               my = min(_listaPunti[idPunti[i]]._y, _listaPunti[idPunti[(i+1)%3]]._y) - Punto::geometricTol;
               My = max(_listaPunti[idPunti[i]]._y, _listaPunti[idPunti[(i+1)%3]]._y) + Punto::geometricTol;
               if(abs(crossProduct(v2,v1))<Punto::geometricTol && (p._x > mx) && (p._y > my) && (p._x < Mx)
                   && (p._y < My))
               {
                   for(unsigned int j = 0; j<3; j++) // j cicla sui lati, cerco il lato dove ho il punto
                   {

                       if((_listaLati[idLati[j]]._p1 ==_listaPunti[idPunti[(i+1)%3]] &&
                              _listaLati[idLati[j]]._p2 ==_listaPunti[idPunti[i]])||
                          (_listaLati[idLati[j]]._p2 ==_listaPunti[idPunti[(i+1)%3]] &&
                              _listaLati[idLati[j]]._p1 ==_listaPunti[idPunti[i]]))
                       {
                            DM = Posizione::LATO_NON_FRONTIERA;
                            result = idLati[j];// gli passo l'id del triangolo
                           return result;
                       }
                   }
               }
               else if (crossProduct(v2,v1)<0)
               {
                    counter++;
                    if(counter==3) {
                       DM = Posizione::INTERNO;
                       result = idCorr;
                       return result;
                    }
               }
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
            vector<unsigned int> lat_cd;
            latcom = _listaLati[fc[0]]; //lato in comune
            pv[0] = latcom._p1;
            pv[1] = latcom._p2;
            double somang; //somma angoli
            array<Punto, 2> vnl; //vertici nuovo lato
            somang = _listaTriangoli[(latcom._listIdTr)[0]].CalcolaAngolo(latcom, _listaLati) +
                     _listaTriangoli[(latcom._listIdTr)[1]].CalcolaAngolo(latcom, _listaLati);
            if ((somang-M_PI)> Punto::geometricTol){
                for(unsigned int k = 0; k<2; k++){
                    if (latcom._listIdTr[k] != fc[1])
                        id_nt = latcom._listIdTr[k];
                }
                ix[1] = id_nt;
                for(unsigned int j = 0; j<3; j++){
                    if ((_listaTriangoli[id_nt]._lati)[j] != fc[0] &&
                        _listaLati[(_listaTriangoli[id_nt]._lati)[j]]._listIdTr.size() == 2){
                        lat_cd.push_back((_listaTriangoli[id_nt]._lati)[j]);
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
            for(unsigned int z = 0; z<lat_cd.size(); z++){
                for(unsigned int h = 0; h<_listaLati[lat_cd[z]]._listIdTr.size(); h++){
                    if ((_listaLati[lat_cd[z]]._listIdTr)[h] == id_nt)
                        nec = {lat_cd[z], id_nt};
                    else if ((_listaLati[lat_cd[z]]._listIdTr)[h] == fc[1])
                        nec = {lat_cd[z], fc[1]};
                }
                _codaDelaunay.push_back(nec);
            }
            _codaDelaunay.pop_front();
        }
	}

    bool Mesh::accettabile(const Punto& newpoint, const Punto& point)
    {
        unsigned int counter = 0;
        unsigned int ilato = _hullBeginLato;
        bool inizio = true;
        Punto dir1 = newpoint - point;
        unsigned int iv = 0;// variabile da passare perchè il costruttore è costante, tanto non ci interessa l'id di questo punto
        double alfa;
        double x_pt;
        double y_pt;
        double m1x;  //variabili che uso per far si che la soluzione
        double m2x;  //del sistema appartnìenga al segmento
        double M1x;  // 1,2: relativo a dir 1,2
        double M2x;
        double m1y;  //variabili che uso per far si che la soluzione
        double m2y;  //del sistema appartnìenga al segmento
        double M1y;  // 1,2: relativo a dir 1,2
        double M2y;
        Punto dir2;
        Punto dir3;
        while((_hullBeginLato != ilato) || inizio)
        {
            inizio = false;
            // ricontrollare per l'antisimmetria del prodotto scalare
            dir2 = _listaLati[ilato]._p1 - _listaLati[ilato]._p2;
            dir3 = _listaLati[ilato]._p2 - point;
            if(abs(crossProduct(dir1,dir2)) > Punto::geometricTol)
            {
                // pt è la slz del sistema lineare
                alfa = (-dir2._y*dir3._x + dir2._x*dir3._y)/(dir1._y*dir2._x - dir1._x*dir2._y);
                x_pt = point._x + alfa*dir1._x;
                y_pt = point._y + alfa*dir1._y;
                Punto pt = Punto(iv, x_pt, y_pt);
                m1x = min(newpoint._x,(point)._x) - Punto::geometricTol;
                M1x = max(newpoint._x,(point)._x) + Punto::geometricTol;
                m1y = min(newpoint._y,(point)._y) - Punto::geometricTol;
                M1y = max(newpoint._y,(point)._y) + Punto::geometricTol;
                m2x = min((_listaLati[ilato]._p1)._x,(_listaLati[ilato]._p2)._x) - Punto::geometricTol;
                M2x = max((_listaLati[ilato]._p1)._x,(_listaLati[ilato]._p2)._x) + Punto::geometricTol;
                m2y = min((_listaLati[ilato]._p1)._y,(_listaLati[ilato]._p2)._y) - Punto::geometricTol;
                M2y = max((_listaLati[ilato]._p1)._y,(_listaLati[ilato]._p2)._y) + Punto::geometricTol;
                if((pt._x >= m1x) && (pt._x >= m2x) && (pt._y >= m1y) && (pt._y >= m2y) &&
                   (pt._x <= M1x) && (pt._x <= M2x) && (pt._y <= M1y) && (pt._y <= M2y))
                {
                    if(pt == _listaLati[ilato]._p1 || pt == _listaLati[ilato]._p2)
                    {
                        counter = counter + 1;
                        if (counter >2)
                        {
                            return false;
                        }
                    }
                    else
                    {
                        return false;
                    }
                }
            // newpoint non può appartenere al lato, ma se ho paralleli coincidenti ho sicuramente point
            // uguale ad uno dei due lati che sto considerando
            }
            else if(_listaLati[ilato]._p1 == point || _listaLati[ilato]._p2 == point)
            {
                double min_dist = min(Norm(newpoint._x-_listaLati[ilato]._p1._x, newpoint._y-_listaLati[ilato]._p1._y),
                                      Norm(newpoint._x-_listaLati[ilato]._p2._x, newpoint._y-_listaLati[ilato]._p2._y));
                double d = Norm(newpoint._x-point._x, newpoint._y-point._y);

                if(abs(min_dist - d) >= Punto::geometricTol*max(min_dist,d))
                    return false;
            }
            ilato = _listaLati[ilato]._succ;
        }
        return true;
    }

    void Mesh::CollegaSenzaIntersezioni(const Punto& nuovoPunto, unsigned int& id_t, unsigned int& id_l)
    {
        //_listaPunti.push_back(nuovoPunto)
        // trovo il primo e l'ultimo lato utile
        unsigned int fineLato = _hullBeginLato;
        unsigned int inizioLato = _listaLati[_hullBeginLato]._prec;
        bool cambiaInizio = false;
        bool b = true; // variabile booleana di comodo
        unsigned int varaus;
        Punto paus;
        if(accettabile(nuovoPunto, _listaLati[fineLato]._p1))
        {
            cambiaInizio = true;
            while(b)
            {
                fineLato = _listaLati[fineLato]._succ;
                b = accettabile(nuovoPunto,_listaLati[fineLato]._p1);
            }
            // in v abbiamo l'utimo false, il prec del puntato di finePunto sarà l'ultimo accettabile
            fineLato = _listaLati[fineLato]._prec;
            if(accettabile(nuovoPunto,_listaLati[inizioLato]._p1))
            {                
                b = true;
                while(b)
                {
                    inizioLato = _listaLati[inizioLato]._prec;
                    b = accettabile(nuovoPunto, _listaLati[inizioLato]._p1);
                }
                // in b abbiamo il primo false, il succ del puntato di b sarà il primo accettabile
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
            b = accettabile(nuovoPunto, _listaLati[inizioLato]._p1);
            while(!b)
            {
                inizioLato = _listaLati[inizioLato]._succ;
                b = accettabile(nuovoPunto,_listaLati[inizioLato]._p1);
            }
            // q a questo punto avrà il primo accettabile
            fineLato = _listaLati[inizioLato]._succ;
            b = accettabile(nuovoPunto, _listaLati[fineLato]._p1);
            while(b)
            {
                fineLato = _listaLati[fineLato]._succ;
                b = accettabile(nuovoPunto, _listaLati[fineLato]._p1);
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
        while(copiaLato != _listaLati[fineLato]._succ)
        {
            varaus = id_t + conta;
            paus = _listaLati[copiaLato]._p1;
            if(copiaLato == fineLato){
                varaus = varaus - 1;
                l = Lato(id_l, nuovoPunto, paus, varaus);
            }else{
                _listaLati[copiaLato]._listIdTr.push_back(varaus);
                l = Lato(id_l, paus, nuovoPunto, varaus);
            }
            if((copiaLato != inizioLato)&&(copiaLato != fineLato)){
                 varaus = varaus - 1;
                l._listIdTr.push_back(varaus);
            }
            _listaLati.push_back(l);
            newLati.push_back(l);
            copiaLato = _listaLati[copiaLato]._succ;
            conta++;
            id_l++;
        }
        // la variabile id_t non è stata cambiata ed è come è stata passata al programma

        // creo triangoli
        copiaLato = inizioLato;
        unsigned int i = 0;
        Triangolo t;
        array<unsigned int, 2> lac; //lati da aggiungere a coda di Delaunay
        while(copiaLato != fineLato)
        {
            lac = {_listaLati[copiaLato]._id, id_t};
            _codaDelaunay.push_back(lac);
            t = Triangolo(id_t, _listaLati[copiaLato]._p1._id, nuovoPunto._id, _listaLati[copiaLato]._p2._id,
                          newLati[i]._id, newLati[i+1]._id, copiaLato);
            copiaLato = _listaLati[copiaLato]._succ;
            _listaTriangoli.push_back(t);
            i++;
            id_t++;
        }
        // aggiornamento HullLato
        _listaLati[fineLato]._prec = (newLati[newLati.size()-1])._id;
        _listaLati[(newLati[newLati.size()-1])._id]._prec = newLati[0]._id;
        _listaLati[(newLati[newLati.size()-1])._id]._succ = fineLato;
        _listaLati[newLati[0]._id]._prec = (_listaLati[_listaLati[inizioLato]._prec])._id;
        _listaLati[newLati[0]._id]._succ = (newLati[newLati.size()-1])._id;
        _listaLati[_listaLati[inizioLato]._prec]._succ = newLati[0]._id;
        // cambia inizio dell'Hull se serve
        if (cambiaInizio) {
            _hullBeginLato= fineLato;
        }
    }

    void Mesh::PuntoInterno(const Punto& po, unsigned int& itr, unsigned int& id_tr, unsigned int& id_lt)
    {
        array<unsigned int, 2> il_it;
        Triangolo tr =  _listaTriangoli[itr];
        Lato l1 = Lato(id_lt, po, _listaPunti[(tr._vertici)[0]], itr);
        unsigned int idl1 = id_lt;
        _listaLati.push_back(l1);
        id_lt++;

        Lato l2 = Lato(id_lt, _listaPunti[(tr._vertici)[1]], po, itr);
        unsigned int idl2 = id_lt;
        _listaLati.push_back(l2);
        id_lt++;

        Triangolo tng = Triangolo(itr, po._id, (tr._vertici)[0], (tr._vertici)[1], idl1, (tr._lati)[0], idl2);
        _listaTriangoli[itr] = tng;
        if (_listaLati[(tr._lati)[0]]._listIdTr.size() == 2){
            il_it = {(tr._lati)[0], tng._id};
            _codaDelaunay.push_back(il_it);
        }

        Lato l3 = Lato(id_lt, _listaPunti[(tr._vertici)[2]], po, id_tr);
        unsigned int idl3 = id_lt;
        _listaLati.push_back(l3);
        id_lt++;
        (_listaLati[idl2]._listIdTr).push_back(id_tr);
        tng = Triangolo(id_tr, po._id, (tr._vertici)[1], (tr._vertici)[2], idl2, (tr._lati)[1], idl3);
        for(unsigned int i = 0; i<2; i++){
            if((_listaLati[(tr._lati)[1]]._listIdTr)[i] == itr){
                (_listaLati[(tr._lati)[1]]._listIdTr)[i] = tng._id;
                break;
            }
        }
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
            if((_listaLati[(tr._lati)[2]]._listIdTr)[i] == itr){
                (_listaLati[(tr._lati)[2]]._listIdTr)[i] = tng._id;
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
        Lato l_o = _listaLati[ilt];
        Lato l1;
        Lato l2;
        Lato l3;
        Lato l4;
        bool a1;
        bool a2;
        for(unsigned int k = 0; k<_listaLati[ilt]._listIdTr.size(); k++){
            itr = (l_o._listIdTr)[k];
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
                            a1 = true;
                        } else {
                            l2 = Lato(ilt, po, _listaPunti[(tr._vertici)[h]], itr);
                            _listaLati[ilt] = l2;
                            l1 = Lato(id_lt, _listaPunti[(tr._vertici)[(h+1)%3]], po, itr);
                            _listaLati.push_back(l1);
                            a1 = false;
                        }
                        tng = Triangolo(itr, po._id, (tr._vertici)[h], (tr._vertici)[(h+1)%3],
                                        ilt, (tr._lati)[h], id_lt);
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
                        if (_listaPunti[(tr._vertici)[h]] == l_o._p1 ||
                            _listaPunti[(tr._vertici)[h]] == l_o._p2){
                            l4 = Lato(id_lt, po, _listaPunti[(tr._vertici)[(h+1)%3]], itr);
                            a2 = true;
                            if(a1)
                                lid = l3._id;
                            else
                                lid = l1._id;
                            tng = Triangolo(itr, po._id, (tr._vertici)[h], (tr._vertici)[(h+1)%3],
                                            lid, (tr._lati)[h], id_lt);
                        } else {
                            a2 = false;
                            l4 = Lato(id_lt, po, _listaPunti[(tr._vertici)[h]], itr);
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
        Lato lat = _listaLati[ilt];
        array<unsigned int, 2> il_it;
        unsigned int itr = (_listaLati[ilt]._listIdTr)[0];
        unsigned int pos;
        Triangolo tr = _listaTriangoli[itr];
        unsigned int pnl;
        for (unsigned h = 0; h<3; h++){
            if(!(_listaPunti[(_listaTriangoli[itr]._vertici)[h]] == lat._p1 ||
                  _listaPunti[(_listaTriangoli[itr]._vertici)[h]] == lat._p2))
            {
                pnl = (_listaTriangoli[itr]._vertici)[h];
                pos = h;
            }
        }
        Lato l1 = Lato(ilt, _listaLati[ilt]._p1, po, itr);
        _listaLati[ilt] = l1;
        Lato l2 = Lato(id_lt, po, _listaPunti[pnl], itr);
        _listaLati.push_back(l2);
        Triangolo tng = Triangolo(itr, lat._p1._id, po._id, pnl, ilt, id_lt, (tr._lati)[pos]);
        _listaTriangoli[itr] = tng;
        id_lt++;
        if(_listaLati[(tr._lati)[pos]]._listIdTr.size() == 2){
            il_it = {(tr._lati)[pos], itr};
            _codaDelaunay.push_back(il_it);
        }
        Lato l3 = Lato(id_lt, po, lat._p2, id_tr);
        _listaLati.push_back(l3);
        tng = Triangolo(id_tr, po._id, lat._p2._id, pnl, id_lt, (tr._lati)[(pos+2)%3],
                        l2._id);
        (_listaLati[l2._id]._listIdTr).push_back(id_tr);
        _listaTriangoli.push_back(tng);
        for (unsigned int j = 0; j< _listaLati[(tr._lati)[(pos+2)%3]]._listIdTr.size(); j++){
            if ((_listaLati[(tr._lati)[(pos+2)%3]]._listIdTr)[j] == itr)
                (_listaLati[(tr._lati)[(pos+2)%3]]._listIdTr)[j] = id_tr;
        }
        if(_listaLati[(tr._lati)[(pos+2)%3]]._listIdTr.size() == 2){
            il_it = {(tr._lati)[(pos+2)%3], id_tr};
            _codaDelaunay.push_back(il_it);
        }
        id_tr++;
        id_lt++;
        _listaLati[l1._id]._prec = lat._prec;
        _listaLati[l3._id]._prec = l1._id;
        _listaLati[l1._id]._succ = l3._id;
        _listaLati[l3._id]._succ = lat._succ;
        _listaLati[lat._succ]._prec = l3._id;
    }

    void Mesh::PrimoTriangolo()
    {
        // aggiungere inizializzazione di esterni
        unsigned int id_lt = 0;
        unsigned int id_tr = 0;
        unsigned int n = _listaPunti.size() - 1;
        vector<Punto> vx = _listaPunti;
        bool ax = true;
        SortLibrary::MergeSort(vx, 0, n, ax);
        ax = false;
        vector<Punto> vy = _listaPunti;
        SortLibrary::MergeSort(vy, 0, n, ax);

        array<Punto, 4> v = {vx[0], vx[n], vy[0], vy[n]};
        array<Punto, 3> punti_scelti;

        if(((v[0] == v[3]) && (v[2] != v[1]))||
            ((v[1] == v[3]) && (v[0] != v[2]))){
            punti_scelti[0]=v[0];
            punti_scelti[1]=v[1];
            punti_scelti[2]=v[2];
        } else if (((v[0] != v[3]) && (v[2] == v[1]))||
                   ((v[0] == v[2]) && (v[1] != v[3]))) {
            punti_scelti[0]=v[0];
            punti_scelti[1]=v[1];
            punti_scelti[2]=v[3];
        } else {
            unsigned int k = 1;
            while((v[0] == v[3]) && (v[2] == v[1])){
                v[0]= vx[k];
                v[2]= vy[k];
                k++;
            }
            double AreaMax = 0;
            double Area = 0;
            array<unsigned int,3> indici_scelti = {0,0,0};
            for (unsigned int i = 0; i< 4;i++)
            {
                Area = 0.5* abs((v[(i+1)%4]._x - v[i]._x)*(v[(i+2)%4]._y - v[i]._y) -
                                 (v[(i+1)%4]._y - v[i]._y)*(v[(i+2)%4]._x - v[i]._x));
                if (abs(AreaMax - Area) > Punto::geometricTol_Squared*max(AreaMax,Area))
                {
                    AreaMax = Area;
                    for (unsigned int j = 0; j<3;j++)
                    {
                        indici_scelti[j] = (i+j)%4;
                    }
                }
            }
            punti_scelti[0]= v[indici_scelti[0]];
            punti_scelti[1]= v[indici_scelti[1]];
            punti_scelti[2]= v[indici_scelti[2]];
        }
        double Area = ((punti_scelti[0]._x*punti_scelti[1]._y) + (punti_scelti[0]._y*punti_scelti[2]._x)
                       + (punti_scelti[1]._x*punti_scelti[2]._y) - (punti_scelti[2]._x*punti_scelti[1]._y)
                       - (punti_scelti[2]._y*punti_scelti[0]._x) - (punti_scelti[1]._x*punti_scelti[0]._y));

        if (Area < Punto::geometricTol)
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
            l = Lato(id_lt, punti_scelti[i], punti_scelti[(i+1)%3], id_tr);
            _listaLati.push_back(l);
            id_lt++;
        }
        _listaLati[0]._prec = 2;
        _listaLati[1]._prec = 0;
        _listaLati[2]._prec = 1;
        _listaLati[0]._succ = 1;
        _listaLati[1]._succ = 2;
        _listaLati[2]._succ = 0;
        Triangolo tr = Triangolo(id_tr, punti_scelti[0]._id , punti_scelti[1]._id, punti_scelti[2]._id,
                                 _listaLati[0]._id, _listaLati[1]._id, _listaLati[2]._id);
        _listaTriangoli.push_back(tr);
    }

    Mesh::Mesh(const vector<Punto>& listaPunti):
        _listaPunti(listaPunti)
    {
        this->PrimoTriangolo();

        unsigned int idlato = 3;
        unsigned int idtriang = 1;
        unsigned int id_result;
        Posizione DM;
        Punto po;        
        for(unsigned int i = 0; i<_listaPunti.size(); i++){
            po = _listaPunti[i];
            if (!(po._inserito)){
                id_result = this->DentroMesh(po, DM);
                switch (DM) {
                    case Posizione::INTERNO: {
                        this->PuntoInterno(po, id_result, idtriang, idlato);
                        break;
                    }
                    case Posizione::LATO_NON_FRONTIERA: {//sul bordo del triangolo
                        this->PuntoBordoTriang(po, id_result, idtriang, idlato);
                        break;
                    }
                    case Posizione::HULL: {
                        this->PuntoBordoHull(po, id_result, idtriang, idlato);
                        break;
                    }
                    case Posizione::ESTERNO: {
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

        file << "id x1 y1 x2 y2 length\n";

        for(unsigned int i = 0; i <(mesh._listaLati).size(); i++){
            file << to_string(mesh._listaLati[i]._id) << " " << to_string(mesh._listaLati[i]._p1._x) << " " << to_string(mesh._listaLati[i]._p1._y)
                 << " " << to_string(mesh._listaLati[i]._p2._x) << " " << to_string(mesh._listaLati[i]._p2._y) << " " <<
                to_string(mesh._listaLati[i]._length) << endl;
        }
        file.close();

        // dubbio, posso "sovrascrivere" sull'oggetto ofstream ?
        file.open(OutFilePath + "Triangoli_Mesh_" + OutFileNameParz);
        if (file.fail())
            return false;

        file << "id_triangolo p1 p2 p3 l1 l2 l3\n";

        for(unsigned int i = 0; i < (mesh._listaTriangoli).size(); i++){
            file << mesh._listaTriangoli[i]._id << " ";

            for (unsigned int j=0; j < 3 ; j++){
                file << mesh._listaTriangoli[i]._vertici[j] << " ";
            }

            for (unsigned int j=0; j < 3 ; j++){
                // faccio questi if perchè non voglio lo spazio a fine della riga
                if ( j == 2)
                    file << mesh._listaTriangoli[i]._lati[j]<< endl;
                else
                    file << mesh._listaTriangoli[i]._lati[j] << " ";
            }
        }
        file.close();
        cout<<"Mesh Esportata"<<endl;
        return true;
    }
}


