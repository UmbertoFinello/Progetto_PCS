#ifndef __TEST_H
#define __TEST_H

#include <gtest/gtest.h>
#include "Delaunay_class.hpp"
#include "sorting.hpp"

using namespace testing;
using namespace ProjectLibrary;
using namespace SortLibrary;

class LatoTest : public Lato
{
public:
    LatoTest(unsigned int& id, const Punto& p1, const Punto& p2, unsigned int& idtr):Lato(id, p1, p2, idtr){}
    LatoTest(const LatoTest& lat):Lato(lat){}
    LatoTest(){}
    unsigned int& Prec(){return _prec;}
    unsigned int& Succ(){return _succ;}
    vector<unsigned int>& LIT(){return _listIdTr;}
    double& LTH(){return _length;}
    Punto& P2(){return _p2;}
    Punto& P1(){return _p1;}
    unsigned int& ID(){return _id;}
    string Show(){return Lato::Show();}
    LatoTest& operator=(const LatoTest& l) {_id = l._id; _p1 = l._p1; _p2 = l._p2; _length = l._length;
        _listIdTr = l._listIdTr; _prec = l._prec; _succ = l._succ; return *this;
    }
};

class TriangoloTest: public Triangolo
{
public:
    TriangoloTest(unsigned int& ident, const unsigned int& p1, const unsigned int& p2, const unsigned int& p3,
                  const unsigned int& l1, const unsigned int& l2, const unsigned int& l3):
        Triangolo(ident, p1, p2, p3, l1, l2, l3){}
    TriangoloTest(const TriangoloTest& triang):Triangolo(triang){}
    TriangoloTest():Triangolo(){}
    unsigned int& ID(){return _id;}
    array<unsigned int, 3> VRT(){return _vertici;}
    array<unsigned int, 3> LT(){return _lati;}
    double CalcolaAngolo(const Lato& segm, vector<Lato>& lst_lat) {return Triangolo::CalcolaAngolo(segm, lst_lat);}
    string Show(){return Triangolo::Show();}
    TriangoloTest& operator=(const TriangoloTest& t) {_id = t._id; _vertici = t._vertici; _lati = t._lati;
        return *this;
    }
};

class MeshTest: public Mesh
{
public:
    MeshTest(const vector<Punto>& listaPunti):Mesh(listaPunti){}
    vector<Triangolo>& LTR(){return _listaTriangoli;}
    vector<Lato>& LL(){return _listaLati;}
    vector<Punto>& LP(){return _listaPunti;}
    unsigned int& HB(){return _hullBeginLato;}
    list<array<unsigned int,2>>& CD(){return _codaDelaunay;}
    MeshTest(){}
    void ControlloDelaunay(){Mesh::ControlloDelaunay();}
    array<unsigned int, 2> DentroMesh(const Punto& p) {return Mesh::DentroMesh(p);}
    void CollegaSenzaIntersezioni(const Punto& p, unsigned int& id_t, unsigned int& id_l){
        Mesh::CollegaSenzaIntersezioni(p, id_t, id_l);
    }
    bool accettabile(const Punto& pnew, const Punto& v){return Mesh::accettabile(pnew,v);}
    string Show(){return Mesh::Show();}
};

TEST(TestClassPunto, TestPuntoConstructor)
{
    unsigned int id = 4;
    double x = 0;
    double y = 0;

    string right = to_string(id) + " " + to_string(x) + " " + to_string(y);

    Punto p = Punto(id,x,y);

    EXPECT_EQ(p.Show(),right);
}

TEST(TestClassPunto, TestPuntoCopy)
{
    unsigned int id = 4;
    double x = 0;
    double y = 0;
    Punto p = Punto(id,x,y);
    Punto copy = Punto(p);

    EXPECT_EQ(copy.Show(),p.Show());
}

TEST(TestClassLato, TestLatoConstructor_x)
{
    unsigned int id = 0;
    double c1 = 0;
    double c2 = 1;
    Punto p1 = Punto(id,c1,c1);
    id++;
    Punto p2 = Punto(id,c2,c1);
    double length = 1;

    string right = to_string(id) + " " + to_string(p1._id) + " " + to_string(p2._id) + " " +
            to_string(length) + " " + to_string(id);

    Lato l = Lato(id, p1, p2, id);

    EXPECT_EQ(l.Show(),right);

}

TEST(TestClassLato, TestLatoConstructor_y)
{
    unsigned int id = 0;
    double c1 = 0;
    double c2 = 1;
    Punto p1 = Punto(id,c1,c1);
    id++;
    Punto p2 = Punto(id,c1,c2);
    double length = 1;

    string right = to_string(id) + " " + to_string(p1._id) + " " + to_string(p2._id) + " " +
            to_string(length) + " " + to_string(id);

    Lato l = Lato(id, p1, p2, id);

    EXPECT_EQ(l.Show(),right);

}

TEST(TestClassLato, TestLatoConstructor_pitagora)
{
    unsigned int id = 0;
    double c1 = 0;
    double c2 = 1;
    Punto p1 = Punto(id,c1,c1);
    id++;
    Punto p2 = Punto(id,c2,c2);
    double length = sqrt(2);

    string right = to_string(id) + " " + to_string(p1._id) + " " + to_string(p2._id) + " " +
            to_string(length) + " " + to_string(id);

    Lato l = Lato(id, p1, p2, id);

    EXPECT_EQ(l.Show(),right);
}


TEST(TestClassTriangolo, TestTriangoloConstructor)
{
    unsigned int id = 4;   
    unsigned int idp1 = 0;
    unsigned int idp2 = idp1+1;
    unsigned int idp3 = idp1+2;
    unsigned int idl1 = 0;
    unsigned int idl2 = idl1+1;
    unsigned int idl3 = idl1+2;

    string right = to_string(id) + " vertici:" + to_string(0) + "," + to_string(1) + "," + to_string(2) + " lati:" +
        to_string(0) + "," + to_string(1) + "," + to_string(2);

    Triangolo tr = Triangolo(id, idp1, idp2, idp3, idl1, idl2, idl3);

    EXPECT_EQ(tr.Show(),right);
}

TEST(TestClassTriangolo, TestCalcolangoli)
{
    unsigned int id = 4;
    unsigned int idp1 = 0;
    unsigned int idp2 = idp1+1;
    unsigned int idp3 = idp1+2;
    unsigned int idl1 = 0;
    unsigned int idl2 = idl1+1;
    unsigned int idl3 = idl1+2;
    double c1 = 0;
    double c2 = 1;
    Punto p1 = Punto(idp1,c1,c1);
    Punto p2 = Punto(idp2,c2,c1);
    Punto p3 = Punto(idp3,c1,c2);

    Lato l1 = Lato(idl1, p1, p2, id);
    Lato l2 = Lato(idl2, p2, p3, id);
    Lato l3 = Lato(idl3, p3, p1, id);
    vector<Lato> vlat = {l1, l2, l3};
    double right13 = M_PI/4;
    double right2 = M_PI/2;
    TriangoloTest tr = TriangoloTest(id, idp1, idp2, idp3, idl1, idl2, idl3);
    double tol = Punto::geometricTol;
    EXPECT_LE(abs(tr.CalcolaAngolo(l1,vlat)-right13), tol);
    EXPECT_LE(abs(tr.CalcolaAngolo(l2,vlat)-right2), tol);
    EXPECT_LE(abs(tr.CalcolaAngolo(l3,vlat)-right13), tol);
}

TEST(TestSorting, TestMergeSort)
{
    vector<Punto> SortedV_X;
    vector<Punto> SortedV_Y;
    array<double, 11> coord = {44, 25, 10, 31, 25, 48, 37, 43, 18, 48, 27};
    for(unsigned int i = 0; i<11; i++){
        Punto p = Punto(i, coord[i], coord[i]);
        SortedV_X.push_back(p);
        SortedV_Y.push_back(p);
    }
    array<double, 11> vero = {10, 18, 25, 25, 27, 31, 37, 43, 44, 48, 48};
    MergeSort(SortedV_X, 0, SortedV_X.size()-1, true);
    MergeSort(SortedV_Y, 0, SortedV_Y.size()-1, false);
    array<double, 11> SVY;
    array<double, 11> SVX;
    for (unsigned int k = 0; k<11; k++){
        SVX[k] = SortedV_X[k]._x;
        SVY[k] = SortedV_Y[k]._x;
    }

    EXPECT_EQ(vero, SVX);
    EXPECT_EQ(vero, SVY);
}


TEST(TestProduct, TestCrossProduct)
{
    unsigned int id = 0;
    double c1 = 1.5;
    double c2 = 1;
    double c3 = -1;
    double c4 = 2.5;
    Punto p1 = Punto(id, c1, c4);
    Punto p2 = Punto(id, c3, c3);
    Punto p3 = Punto(id, c4, c2);
    double vero1 = 0;
    double vero2 = -1.5;
    double vero3 = 4.75;

    EXPECT_EQ(vero1, crossProduct(p3,p3));
    EXPECT_EQ(vero2, crossProduct(p3,p2));
    EXPECT_EQ(vero3, crossProduct(p3,p1));
}

TEST(TestClasseMesh, TestControlloDelaunay)
{
    MeshTest m = MeshTest();
    unsigned int id = 0;
    unsigned int idp1 = 0;
    unsigned int idp2 = idp1+1;
    unsigned int idp3 = idp1+2;
    unsigned int idp4 = idp1+3;
    unsigned int idl1 = 0;
    unsigned int idl2 = idl1+1;
    unsigned int idl3 = idl1+2;
    unsigned int idl4 = idl1+3;
    unsigned int idl5 = idl1+4;
    double c1 = 0;
    double c2 = 1;
    double c3 = -1;
    Punto p1 = Punto(idp1,c1,c1);
    Punto p2 = Punto(idp2,c2,c3);
    Punto p3 = Punto(idp3,c1,c2);
    Punto p4 = Punto(idp4,c2,c2);

    LatoTest l1 = LatoTest(idl1, p1, p2, id);
    LatoTest l2 = LatoTest(idl2, p2, p3, id);
    LatoTest l3 = LatoTest(idl3, p3, p1, id);
    Triangolo t1 = Triangolo(id, idp1, idp2, idp3, idl1, idl2, idl3);
    id++;
    LatoTest l4 = LatoTest(idl4, p2, p4, id);
    LatoTest l5 = LatoTest(idl5, p4, p3, id);
    Triangolo t2 = Triangolo(id, idp2, idp4, idp3, idl4, idl5, idl2);
    l2.LIT().push_back(id);
    m.LTR() = {t1, t2};
    m.LP() = {p1, p2, p3, p4};
    m.LL() = {l1, l2, l3, l4, l5};
    m.CD().push_back({l2.ID(), 0});
    m.ControlloDelaunay();
    string vero = "Punti\nId x y\n0 0.000000 0.000000\n1 1.000000 -1.000000\n2 0.000000 1.000000\n3 1.000000 "
                  "1.000000\nLati\nId p1 p2 Length TriangoliAdiacenti\n0 0 1 1.414214 0\n1 0 3 1.414214 0,1\n2"
                  " 2 0 1.000000 1\n3 1 3 2.000000 0\n4 3 2 1.000000 1\nTriangoli\nId p1 p2 p3 l1 l2 l3\n0 1 3"
                  " 0 3 1 0\n1 2 0 3 2 1 4\n";
    EXPECT_EQ(m.Show(), vero);

}

TEST(TestClassMesh, TestDentroMesh)
{
        // id punti
        unsigned int idp1 = 0;
        unsigned int idp2 = idp1+1;
        unsigned int idp3 = idp1+2;
        unsigned int idp4 = idp1+3;
        unsigned int idp5 = idp1+4;
        // id lati
        unsigned int idl1 = 0;
        unsigned int idl2 = idl1+1;
        unsigned int idl3 = idl1+2;
        unsigned int idl4 = idl1+3;
        unsigned int idl5 = idl1+4;
        unsigned int idl6 = idl1+5;
        unsigned int idl7 = idl1+6;
        unsigned int idl8 = idl1+7;
        // id triangoli
        unsigned int idt1 = 1;
        unsigned int idt2 = idt1+1;
        unsigned int idt3 = idt1+2;
        unsigned int idt4 = idt1+3;
        // coordinate p1
        double p1x = 5;
        double p1y = 2;
        // coordinate p2
        double p2x = 2;
        double p2y = 6;
        // coordinate p3
        double p3x = 6;
        double p3y = 9;
        // coordinate p4
        double p4x = 14;
        double p4y = 7;
        // coordinate p5
        double p5x = 7;
        double p5y = 6;
        // Punti
        Punto p1 = Punto(idp1,p1x,p1y);
        Punto p2 = Punto(idp2,p2x,p2y);
        Punto p3 = Punto(idp3,p3x,p3y);
        Punto p4 = Punto(idp4,p4x,p4y);
        Punto p5 = Punto(idp5,p5x,p5y);
        // Lati
        LatoTest l1 = LatoTest(idl1, p2, p1, idt1);
        LatoTest l2 = LatoTest(idl2, p3, p2, idt2);
        LatoTest l3 = LatoTest(idl3, p4, p3, idt3);
        LatoTest l4 = LatoTest(idl4, p1, p4, idt4);
        LatoTest l5 = LatoTest(idl5, p1, p5, idt1);
        LatoTest l6 = LatoTest(idl6, p2, p5, idt2);
        LatoTest l7 = LatoTest(idl7, p3, p5, idt3);
        LatoTest l8 = LatoTest(idl8, p4, p5, idt4);
        // id dilati interni
        l5.LIT().push_back(idt4);
        l6.LIT().push_back(idt1);
        l7.LIT().push_back(idt2);
        l8.LIT().push_back(idt3);
        // id successivi
        l1.Succ() = idl2;
        l2.Succ() = idl3;
        l3.Succ() = idl4;
        l4.Succ() = idl1;
        //id precedenti
        l1.Prec() = idl4;
        l2.Prec() = idl1;
        l3.Prec() = idl2;
        l4.Prec() = idl3;
        // triangoli
        TriangoloTest t1 = TriangoloTest(idt1,idp1,idp5,idp2,idl5,idl6,idl1);
        TriangoloTest t2 = TriangoloTest(idt2,idp2,idp5,idp3,idl6,idl7,idl2);
        TriangoloTest t3 = TriangoloTest(idt3,idp3,idp5,idp4,idl7,idl8,idl3);
        TriangoloTest t4 = TriangoloTest(idt4,idp4,idp5,idp1,idl8,idl5,idl4);
        // inizializzazione mesh (finalmente)
        vector<Punto> vertici = { p1,p2,p3,p4,p5 };
        vector<Lato> lati = { l1,l2,l3,l4,l5,l6,l7,l8 };
        vector<Triangolo> triangoli = { t1,t2,t3,t4 };
        MeshTest mesh;
        mesh.LP() = vertici;
        mesh.LL()= lati;
        mesh.LTR()  = triangoli;
        mesh.HB() = idl1;
        // punto nuovo, dentro primo triangolo
        double pnewx = 5;
        double pnewy = 5;
        unsigned int idpnew = idp1+5;
        Punto pnew = Punto(idpnew,pnewx,pnewy);
        // richiamo dentro mesh
        array<unsigned int, 2> esito = mesh.DentroMesh(pnew);
        string vero = to_string(0) + " " + to_string(1);
        string result = to_string(esito[0]) + " " + to_string(esito[1]);
        EXPECT_EQ(vero, result);

        /*
        // bordo interno
        pnewx = 6;
        pnewy = 4;
        idpnew = idp1+5;
        pnew = Punto(idpnew,pnewx,pnewy);


        // bordo hull
        pnewx = 10;
        pnewy = 8;
        idpnew = idp1+5;
        pnew = Punto(idpnew,pnewx,pnewy);

        // punto esterno
        pnewx = 12;
        pnewy = 3;
        idpnew = idp1+5;
        pnew = Punto(idpnew,pnewx,pnewy);

        // ricerca in un altro triangolo interno
        pnewx = ;
        pnewy = ;
        idpnew = idp1+5;
        pnew = Punto(idpnew,pnewx,pnewy);

        // ricerca in un altro triangolo bordo
        pnewx = ;
        pnewy = ;
        idpnew = idp1+5;
        pnew = Punto(idpnew,pnewx,pnewy);
        */
    }


TEST(TestClasseMesh, TestCollegaSenzaIntersezioni)
{

}

TEST(TestClassMesh, TestAccettabile)
{
    // id punti
    unsigned int idp1 = 0;
    unsigned int idp2 = idp1+1;
    unsigned int idp3 = idp1+2;
    unsigned int idp4 = idp1+3;
    unsigned int idp5 = idp1+4;
    // id lati
    unsigned int idl1 = 0;
    unsigned int idl2 = idl1+1;
    unsigned int idl3 = idl1+2;
    unsigned int idl4 = idl1+3;
    unsigned int idl5 = idl1+4;
    unsigned int idl6 = idl1+5;
    unsigned int idl7 = idl1+6;
    unsigned int idl8 = idl1+7;
    // id triangoli
    unsigned int idt1 = 1;
    unsigned int idt2 = idt1+1;
    unsigned int idt3 = idt1+2;
    unsigned int idt4 = idt1+3;
    // coordinate p1
    double p1x = 5;
    double p1y = 2;
    // coordinate p2
    double p2x = 2;
    double p2y = 6;
    // coordinate p3
    double p3x = 6;
    double p3y = 9;
    // coordinate p4
    double p4x = 14;
    double p4y = 7;
    // coordinate p5
    double p5x = 7;
    double p5y = 6;
    // Punti
    Punto p1 = Punto(idp1,p1x,p1y);
    Punto p2 = Punto(idp2,p2x,p2y);
    Punto p3 = Punto(idp3,p3x,p3y);
    Punto p4 = Punto(idp4,p4x,p4y);
    Punto p5 = Punto(idp5,p5x,p5y);
    // Lati
    LatoTest l1 = LatoTest(idl1, p2, p1, idt1);
    LatoTest l2 = LatoTest(idl2, p3, p2, idt2);
    LatoTest l3 = LatoTest(idl3, p4, p3, idt3);
    LatoTest l4 = LatoTest(idl4, p1, p4, idt4);
    LatoTest l5 = LatoTest(idl5, p1, p5, idt1);
    LatoTest l6 = LatoTest(idl6, p2, p5, idt2);
    LatoTest l7 = LatoTest(idl7, p3, p5, idt3);
    LatoTest l8 = LatoTest(idl8, p4, p5, idt4);
    // id dilati interni
    l5.LIT().push_back(idt4);
    l6.LIT().push_back(idt1);
    l7.LIT().push_back(idt2);
    l8.LIT().push_back(idt3);
    // id successivi
    l1.Succ() = idl2;
    l2.Succ() = idl3;
    l3.Succ() = idl4;
    l4.Succ() = idl1;
    //id precedenti
    l1.Prec() = idl4;
    l2.Prec() = idl1;
    l3.Prec() = idl2;
    l4.Prec() = idl3;
    // triangoli
    TriangoloTest t1 = TriangoloTest(idt1,idp1,idp5,idp2,idl5,idl6,idl1);
    TriangoloTest t2 = TriangoloTest(idt2,idp2,idp5,idp3,idl6,idl7,idl2);
    TriangoloTest t3 = TriangoloTest(idt3,idp3,idp5,idp4,idl7,idl8,idl3);
    TriangoloTest t4 = TriangoloTest(idt4,idp4,idp5,idp1,idl8,idl5,idl4);
    // inizializzazione mesh (finalmente)
    vector<Punto> vertici = { p1,p2,p3,p4,p5 };
    vector<Lato> lati = { l1,l2,l3,l4,l5,l6,l7,l8 };
    vector<Triangolo> triangoli = { t1,t2,t3,t4 };
    MeshTest mesh;
    mesh.LP() = vertici;
    mesh.LL()= lati;
    mesh.LTR()  = triangoli;
    mesh.HB() = idl1;
    // punto nuovo
    double pnewx = 14;
    double pnewy = 9;
    unsigned int idpnew = idp1+5;
    Punto pnew = Punto(idpnew,pnewx,pnewy);
    // test accettabile
    string result ;
    for(unsigned int i = 0; i<4; i++){
        if(mesh.accettabile(pnew, mesh.LP()[i]))
        {
            result = result + "T ";
        }else{
            result = result + "F ";
        }
    }
    string vero = "F F T T ";
    EXPECT_EQ(vero, result);
}

TEST(TestClassMesh, TestCostruttore)
{

}









#endif // __TEST_H
