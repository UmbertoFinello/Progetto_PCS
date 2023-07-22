#ifndef __TEST_H
#define __TEST_H

#include <gtest/gtest.h>
#include "Delaunay_class.hpp"
#include "sorting.hpp"

using namespace testing;
using namespace ProjectLibrary;
using namespace SortLibrary;
using namespace std;

class LatoTest : public Lato
{
public:
    LatoTest(unsigned int& id, const Punto& p1, const Punto& p2, unsigned int& idtr):Lato(id, p1, p2, idtr){}
    LatoTest(const LatoTest& lat):Lato(lat){}
    LatoTest(const Lato& lat):Lato(lat){}
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
    unsigned int DentroMesh(const Punto& p, Posizione& DM) {return Mesh::DentroMesh(p, DM);}
    void CollegaSenzaIntersezioni(const Punto& p, unsigned int& id_t, unsigned int& id_l){
        Mesh::CollegaSenzaIntersezioni(p, id_t, id_l);
    }
    bool accettabile(const Punto& pnew, const Punto& v){return Mesh::accettabile(pnew,v);}
    void PuntoInterno(const Punto& po, unsigned int& itr, unsigned int& id_tr, unsigned int& id_lt){
        Mesh::PuntoInterno(po, itr, id_tr, id_lt);}
    void PuntoBordoTriangolo(const Punto& po, unsigned int& ilt, unsigned int& id_tr, unsigned int& id_lt){
        Mesh::PuntoBordoTriangolo(po, ilt, id_tr, id_lt);}
    void PuntoBordoHull(const Punto& po, unsigned int& ilt, unsigned int& id_tr, unsigned int& id_lt){
        Mesh::PuntoBordoHull(po, ilt, id_tr, id_lt);}
    string Show(){return Mesh::Show();}
};

TEST(TestImportPunti, TestImportPunti)
{
    vector<Punto> lst_pt;
    string NomeFile = "../Delaunay/Dataset/TestImportPunti.csv";
    ImportPunti(lst_pt, NomeFile);
    string punti;
    for(unsigned int k=0; k<lst_pt.size(); k++){
        punti = punti + to_string(lst_pt[k]._id)+ " " + to_string(lst_pt[k]._x) + " " + to_string(lst_pt[k]._y)+ "\n";
    }
    string right = "0 0.386290 0.604883\n1 0.513084 0.628267\n2 0.597176 0.756016\n3 0.420698 0.773757\n";
    EXPECT_EQ(right, punti);
}

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

TEST(TestClasseMesh, TestControlloDelaunay_scambio)
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
    string vero = "Punti\nId x y\n0 0.000000 0.000000\n1 1.000000 -1.000000\n2 0.000000 1.000000\n"
                  "3 1.000000 1.000000\nLati\nId p1 p2 Length TriangoliAdiacenti\n0 0 1 1.414214 0\n"
                  "1 0 3 1.414214 0,1\n2 2 0 1.000000 1\n3 1 3 2.000000 0\n4 3 2 1.000000 1\nTriangoli\n"
                  "Id p1 p2 p3 l1 l2 l3\n0 1 3 0 3 1 0\n1 2 0 3 2 1 4\n";
    EXPECT_EQ(m.Show(), vero);
}

TEST(TestClasseMesh, TestControlloDelaunay_noscambio)
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

    Punto p1 = Punto(idp1,c1,c1);
    Punto p2 = Punto(idp2,c2,c1);
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
    string vero = "Punti\nId x y\n0 0.000000 0.000000\n1 1.000000 0.000000\n2 0.000000 1.000000\n"
                  "3 1.000000 1.000000\nLati\nId p1 p2 Length TriangoliAdiacenti\n0 0 1 1.000000 0\n"
                  "1 1 2 1.414214 0,1\n2 2 0 1.000000 0\n3 1 3 1.000000 1\n4 3 2 1.000000 1\nTriangoli\n"
                  "Id p1 p2 p3 l1 l2 l3\n0 0 1 2 0 1 2\n1 1 3 2 3 4 1\n";
    EXPECT_EQ(m.Show(), vero);
}

TEST(TestClasseMesh, TestPuntoInterno)
{
    MeshTest m = MeshTest();
    unsigned int id_t = 0;
    unsigned int idt1 = 0;
    unsigned int idt2 = 1;
    unsigned int idp1 = 0;
    unsigned int idp2 = 1;
    unsigned int idp3 = 2;
    unsigned int idp4 = 3;
    unsigned int idp5 = 4;
    unsigned int idl1 = 0;
    unsigned int idl2 = 1;
    unsigned int idl3 = 2;
    unsigned int idl4 = 3;
    unsigned int idl5 = 4;
    unsigned int idl = 1 + idl5;
    double c1 = 0;
    double c2 = 1;
    double c3 = -1;
    double c4 = 3;
    double c5 = 4;

    Punto p1 = Punto(idp1,c1,c1);
    Punto p2 = Punto(idp2,c5,c3);
    Punto p3 = Punto(idp3,c1,c4);
    Punto p4 = Punto(idp4,c1,c3);
    Punto p5 = Punto(idp5,c2,c2);
    LatoTest l1 = LatoTest(idl1, p1, p2, idt1);
    LatoTest l2 = LatoTest(idl2, p2, p3, idt1);
    LatoTest l3 = LatoTest(idl3, p3, p1, idt1);
    LatoTest l4 = LatoTest(idl4, p1, p4, idt2);
    LatoTest l5 = LatoTest(idl5, p4, p2, idt2);
    Triangolo t1 = Triangolo(idt1, idp1, idp2, idp3, idl1, idl2, idl3);
    id_t++;
    Triangolo t2 =Triangolo(idt2, idp1, idp4, idp2, idl4, idl5, idl1);
    id_t++;
    l1.LIT().push_back(idt2);
    m.LTR() = {t1, t2};
    m.LP() = {p1, p2, p3, p4, p5};
    m.LL() = {l1, l2, l3, l4, l5};
    m.PuntoInterno(p5, idt1, id_t, idl);
    string vero_mesh = "Punti\nId x y\n0 0.000000 0.000000\n1 4.000000 -1.000000\n2 0.000000 3.000000\n"
                       "3 0.000000 -1.000000\n4 1.000000 1.000000\nLati\nId p1 p2 Length TriangoliAdiacenti\n"
                       "0 0 1 4.123106 0,1\n1 1 2 5.656854 2\n2 2 0 3.000000 3\n3 0 3 1.000000 1\n"
                       "4 3 1 4.000000 1\n5 4 0 1.414214 0,3\n6 1 4 3.605551 0,2\n7 2 4 2.236068 2,3\n"
                       "Triangoli\nId p1 p2 p3 l1 l2 l3\n0 4 0 1 5 0 6\n1 0 3 1 3 4 0\n2 4 1 2 6 1 7\n"
                       "3 4 2 0 7 2 5\n";
    string vero_CD = "0,0 ";
    EXPECT_EQ(m.Show(), vero_mesh);
    string sCD = "";
    for(array<unsigned int,2> i : m.CD())
        sCD = sCD + to_string(i[0]) + "," + to_string(i[1]) + " ";
    EXPECT_EQ(sCD, vero_CD);
}

TEST(TestClasseMesh, TestPuntoBordoTriangolo)
{
    MeshTest m = MeshTest();
    unsigned int id_t = 0;
    unsigned int idp1 = 0;
    unsigned int idp2 = 1;
    unsigned int idp3 = 2;
    unsigned int idp4 = 3;
    unsigned int idp5 = 4;
    unsigned int idp6 = 5;
    unsigned int idl1 = 0;
    unsigned int idl2 = 1;
    unsigned int idl3 = 2;
    unsigned int idl4 = 3;
    unsigned int idl5 = 4;
    unsigned int idl6 = 5;
    unsigned int idl7 = 6;
    unsigned int idl = 1 + idl7;
    double c1 = 0;
    double c2 = 1;
    double c3 = 0.5;
    double c4 = -1;

    Punto p1 = Punto(idp1,c1,c1);
    Punto p2 = Punto(idp2,c2,c1);
    Punto p3 = Punto(idp3,c1,c2);
    Punto p4 = Punto(idp4,c2,c2);
    Punto p5 = Punto(idp5,c4,c4);
    Punto p6 = Punto(idp6,c3,c3);
    LatoTest l1 = LatoTest(idl1, p1, p2, id_t);
    LatoTest l2 = LatoTest(idl2, p2, p3, id_t);
    LatoTest l3 = LatoTest(idl3, p3, p1, id_t);
    Triangolo t1 = Triangolo(id_t, idp1, idp2, idp3, idl1, idl2, idl3);
    id_t++;
    LatoTest l4 = LatoTest(idl4, p2, p4, id_t);
    LatoTest l5 = LatoTest(idl5, p4, p3, id_t);
    Triangolo t2 = Triangolo(id_t, idp2, idp4, idp3, idl4, idl5, idl2);
    l2.LIT().push_back(id_t);
    id_t++;
    LatoTest l6 = LatoTest(idl6, p1, p5, id_t);
    LatoTest l7 = LatoTest(idl7, p5, p2, id_t);
    Triangolo t3 = Triangolo(id_t, idp1, idp5, idp2, idl6, idl7, idl1);
    l1.LIT().push_back(id_t);
    id_t++;
    m.LTR() = {t1, t2, t3};
    m.LP() = {p1, p2, p3, p4, p5, p6};
    m.LL() = {l1, l2, l3, l4, l5, l6, l7};
    m.PuntoBordoTriangolo(p6, idl2, id_t, idl);
    string vero_mesh = "Punti\nId x y\n0 0.000000 0.000000\n1 1.000000 0.000000\n2 0.000000 1.000000\n"
                  "3 1.000000 1.000000\n4 -1.000000 -1.000000\n5 0.500000 0.500000\nLati\n"
                  "Id p1 p2 Length TriangoliAdiacenti\n0 0 1 1.000000 0,2\n1 5 0 0.707107 0,3\n"
                  "2 2 0 1.000000 3\n3 1 3 1.000000 1\n4 3 2 1.000000 4\n5 0 4 1.414214 2\n"
                  "6 4 1 2.236068 2\n7 1 5 0.707107 0,1\n8 5 2 0.707107 3,4\n9 5 3 0.707107 1,4\n"
                  "Triangoli\nId p1 p2 p3 l1 l2 l3\n0 5 0 1 1 0 7\n1 5 1 3 7 3 9\n2 0 4 1 5 6 0\n"
                  "3 5 2 0 8 2 1\n4 5 3 2 9 4 8\n";
    EXPECT_EQ(m.Show(), vero_mesh);
    string vero_CD = "0,0 ";
    string sCD = "";
    for(array<unsigned int,2> i : m.CD())
        sCD = sCD + to_string(i[0]) + "," + to_string(i[1]) + " ";
    EXPECT_EQ(sCD, vero_CD);
}

TEST(TestClasseMesh, TestPuntoBordoHull_NOCambioInizio)
{
    MeshTest m = MeshTest();
    unsigned int id_t = 0;
    unsigned int idt1 = 0;
    unsigned int idt2 = 1;
    unsigned int idp1 = 0;
    unsigned int idp2 = 1;
    unsigned int idp3 = 2;
    unsigned int idp4 = 3;
    unsigned int idp5 = 4;
    unsigned int idl1 = 0;
    unsigned int idl2 = 1;
    unsigned int idl3 = 2;
    unsigned int idl4 = 3;
    unsigned int idl5 = 4;
    unsigned int idl = 1 + idl5;
    double c1 = 0;
    double c2 = 1;
    double c3 = -0.5;
    double c4 = 0.5;

    Punto p1 = Punto(idp1,c1,c1);
    Punto p2 = Punto(idp2,c2,c1);
    Punto p3 = Punto(idp3,c1,c2);
    Punto p4 = Punto(idp4,c3,c4);
    Punto p5 = Punto(idp5,c4,c4);
    LatoTest l1 = LatoTest(idl1, p1, p2, idt1);
    LatoTest l2 = LatoTest(idl2, p2, p3, idt1);
    LatoTest l3 = LatoTest(idl3, p3, p1, idt1);
    LatoTest l4 = LatoTest(idl4, p3, p4, idt2);
    LatoTest l5 = LatoTest(idl5, p4, p1, idt2);
    l1.Prec() = idl5;
    l1.Succ() = idl2;
    l2.Prec() = idl1;
    l2.Succ() = idl4;
    l4.Prec() = idl2;
    l4.Succ() = idl5;
    l5.Prec() = idl4;
    l5.Succ() = idl1;
    Triangolo t1 = Triangolo(idt1, idp1, idp2, idp3, idl1, idl2, idl3);
    id_t++;
    Triangolo t2 =Triangolo(idt2, idp1, idp3, idp4, idl3, idl4, idl5);
    id_t++;
    l3.LIT().push_back(idt2);
    m.LTR() = {t1, t2};
    m.LP() = {p1, p2, p3, p4, p5};
    m.LL() = {l1, l2, l3, l4, l5};
    m.HB() = idl1;
    m.PuntoBordoHull(p5, idl2, id_t, idl);
    string vero_mesh = "Punti\nId x y\n0 0.000000 0.000000\n1 1.000000 0.000000\n2 0.000000 1.000000\n"
                       "3 -0.500000 0.500000\n4 0.500000 0.500000\nLati\nId p1 p2 Length TriangoliAdiacenti\n"
                       "0 0 1 1.000000 0\n1 1 4 0.707107 0\n2 2 0 1.000000 2,1\n3 2 3 0.707107 1\n"
                       "4 3 0 0.707107 1\n5 4 0 0.707107 0,2\n6 4 2 0.707107 2\nTriangoli\n"
                       "Id p1 p2 p3 l1 l2 l3\n0 1 4 0 1 5 0\n1 0 2 3 2 3 4\n2 4 2 0 6 2 5\n";
    EXPECT_EQ(m.Show(), vero_mesh);

    string vero_CD = "2,2 ";
    string sCD = "";
    for(array<unsigned int,2> i : m.CD())
        sCD = sCD + to_string(i[0]) + "," + to_string(i[1]) + " ";
    EXPECT_EQ(sCD, vero_CD);

    string vero_Hull_succ = " 0 1 6 3 4 0";
    unsigned int entra = 0;
    unsigned int i = m.HB();
    string sHull_succ = "";
    while(entra<2){
        if (i == m.HB())
            entra++;
        sHull_succ = sHull_succ + " " + to_string(i);
        LatoTest a = (m.LL())[i];
        i = a.Succ();
    }
    string vero_Hull_prec = " 0 4 3 6 1 0";
    entra = 0;
    i = m.HB();
    string sHull_prec = "";
    while(entra<2){
        if (i == m.HB())
            entra++;
        sHull_prec = sHull_prec + " " + to_string(i);
        LatoTest a = (m.LL())[i];
        i = a.Prec();
    }
    EXPECT_EQ(sHull_succ, vero_Hull_succ);
    EXPECT_EQ(sHull_prec, vero_Hull_prec);
}

TEST(TestClasseMesh, TestPuntoBordoHull_CambioInizioHull)
{
    MeshTest m = MeshTest();
    unsigned int id_t = 0;
    unsigned int idt1 = 0;
    unsigned int idt2 = 1;
    unsigned int idp1 = 0;
    unsigned int idp2 = 1;
    unsigned int idp3 = 2;
    unsigned int idp4 = 3;
    unsigned int idp5 = 4;
    unsigned int idl1 = 0;
    unsigned int idl2 = 1;
    unsigned int idl3 = 2;
    unsigned int idl4 = 3;
    unsigned int idl5 = 4;
    unsigned int idl = 1 + idl5;
    double c1 = 0;
    double c2 = 1;
    double c3 = -0.5;
    double c4 = 0.5;

    Punto p1 = Punto(idp1,c1,c1);
    Punto p2 = Punto(idp2,c2,c1);
    Punto p3 = Punto(idp3,c1,c2);
    Punto p4 = Punto(idp4,c3,c4);
    Punto p5 = Punto(idp5,c4,c1);
    LatoTest l1 = LatoTest(idl1, p1, p2, idt1);
    LatoTest l2 = LatoTest(idl2, p2, p3, idt1);
    LatoTest l3 = LatoTest(idl3, p3, p1, idt1);
    LatoTest l4 = LatoTest(idl4, p3, p4, idt2);
    LatoTest l5 = LatoTest(idl5, p4, p1, idt2);
    l1.Prec() = idl5;
    l1.Succ() = idl2;
    l2.Prec() = idl1;
    l2.Succ() = idl4;
    l4.Prec() = idl2;
    l4.Succ() = idl5;
    l5.Prec() = idl4;
    l5.Succ() = idl1;
    Triangolo t1 = Triangolo(idt1, idp1, idp2, idp3, idl1, idl2, idl3);
    id_t++;
    Triangolo t2 =Triangolo(idt2, idp1, idp3, idp4, idl3, idl4, idl5);
    id_t++;
    l3.LIT().push_back(idt2);
    m.LTR() = {t1, t2};
    m.LP() = {p1, p2, p3, p4, p5};
    m.LL() = {l1, l2, l3, l4, l5};
    m.HB() = idl1;
    m.PuntoBordoHull(p5, idl1, id_t, idl);
    string vero_mesh = "Punti\nId x y\n0 0.000000 0.000000\n1 1.000000 0.000000\n2 0.000000 1.000000\n"
                       "3 -0.500000 0.500000\n4 0.500000 0.000000\nLati\nId p1 p2 Length TriangoliAdiacenti\n"
                       "0 0 4 0.500000 0\n1 1 2 1.414214 2\n2 2 0 1.000000 0,1\n3 2 3 0.707107 1\n"
                       "4 3 0 0.707107 1\n5 4 2 1.118034 0,2\n6 4 1 0.500000 2\nTriangoli\n"
                       "Id p1 p2 p3 l1 l2 l3\n0 0 4 2 0 5 2\n1 0 2 3 2 3 4\n2 4 1 2 6 1 5\n";
    EXPECT_EQ(m.Show(), vero_mesh);

    string vero_CD = "2,0 ";
    string sCD = "";
    for(array<unsigned int,2> i : m.CD())
        sCD = sCD + to_string(i[0]) + "," + to_string(i[1]) + " ";
    EXPECT_EQ(sCD, vero_CD);

    string vero_Hull_succ = " 0 6 1 3 4 0";
    unsigned int entra = 0;
    unsigned int i = m.HB();
    string sHull_succ = "";
    while(entra<2){
        if (i == m.HB())
            entra++;
        sHull_succ = sHull_succ + " " + to_string(i);
        LatoTest a = (m.LL())[i];
        i = a.Succ();
    }
    string vero_Hull_prec = " 0 4 3 1 6 0";
    entra = 0;
    i = m.HB();
    string sHull_prec = "";
    while(entra<2){
        if (i == m.HB())
            entra++;
        sHull_prec = sHull_prec + " " + to_string(i);
        LatoTest a = (m.LL())[i];
        i = a.Prec();
    }
    EXPECT_EQ(sHull_succ, vero_Hull_succ);
    EXPECT_EQ(sHull_prec, vero_Hull_prec);
}

//Dentro Mesh

TEST(TestClassMesh, TestDentroMesh_primo_interno)
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
    unsigned int idt1 = 0;
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
    // punto nuovo, dentro primo triangolo
    double p6x = 5;
    double p6y = 5;
    unsigned int idpnew = idp1+5;
    Punto p6 = Punto(idpnew,p6x,p6y);
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
    l1.Prec() = idl2;
    l2.Prec() = idl3;
    l3.Prec() = idl4;
    l4.Prec() = idl1;
    //id precedenti
    l1.Succ() = idl4;
    l2.Succ() = idl1;
    l3.Succ() = idl2;
    l4.Succ() = idl3;
    // triangoli
    TriangoloTest t1 = TriangoloTest(idt1,idp1,idp5,idp2,idl5,idl6,idl1);
    TriangoloTest t2 = TriangoloTest(idt2,idp2,idp5,idp3,idl6,idl7,idl2);
    TriangoloTest t3 = TriangoloTest(idt3,idp3,idp5,idp4,idl7,idl8,idl3);
    TriangoloTest t4 = TriangoloTest(idt4,idp4,idp5,idp1,idl8,idl5,idl4);
    idl8++;
    idt4++;
    // inizializzazione mesh
    MeshTest mesh;
    mesh.LP() = { p1,p2,p3,p4,p5 };
    mesh.LL()= { l1,l2,l3,l4,l5,l6,l7,l8 };
    mesh.LTR()  = { t1,t2,t3,t4 };
    mesh.HB() = idl1;

    Posizione DM;
    unsigned int esito = mesh.DentroMesh(p6, DM);
    string vero = to_string(0);
    string result = to_string(esito);
    EXPECT_EQ(vero, result);
    EXPECT_EQ(Posizione::INTERNO, DM);
}

TEST(TestClassMesh, TestDentroMesh_brodo_non_hull)
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
    unsigned int idt1 = 0;
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
    // punto nuovo, dentro primo triangolo
    double p6x = 6;
    double p6y = 4;
    unsigned int idpnew = idp1+5;
    Punto p6 = Punto(idpnew,p6x,p6y);
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
    l1.Prec() = idl2;
    l2.Prec() = idl3;
    l3.Prec() = idl4;
    l4.Prec() = idl1;
    //id precedenti
    l1.Succ() = idl4;
    l2.Succ() = idl1;
    l3.Succ() = idl2;
    l4.Succ() = idl3;
    // triangoli
    TriangoloTest t1 = TriangoloTest(idt1,idp1,idp5,idp2,idl5,idl6,idl1);
    TriangoloTest t2 = TriangoloTest(idt2,idp2,idp5,idp3,idl6,idl7,idl2);
    TriangoloTest t3 = TriangoloTest(idt3,idp3,idp5,idp4,idl7,idl8,idl3);
    TriangoloTest t4 = TriangoloTest(idt4,idp4,idp5,idp1,idl8,idl5,idl4);
    idl8++;
    idt4++;
    // inizializzazione mesh
    MeshTest mesh;
    mesh.LP() = { p1,p2,p3,p4,p5 };
    mesh.LL()= { l1,l2,l3,l4,l5,l6,l7,l8 };
    mesh.LTR()  = { t1,t2,t3,t4 };
    mesh.HB() = idl1;
    //test2: dentro bordo interno
    Posizione DM;
    unsigned int esito = mesh.DentroMesh(p6, DM);
    string vero = to_string(4);
    string result = to_string(esito);
    EXPECT_EQ(vero, result);
    EXPECT_EQ(Posizione::LATO_DENTRO, DM);
}

TEST(TestClassMesh, TestDentroMesh_bordo_hull)
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
    unsigned int idt1 = 0;
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
    // punto nuovo, dentro primo triangolo
    double p6x = 10;
    double p6y = 8;
    unsigned int idpnew = idp1+5;
    Punto p6 = Punto(idpnew,p6x,p6y);
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
    l1.Prec() = idl2;
    l2.Prec() = idl3;
    l3.Prec() = idl4;
    l4.Prec() = idl1;
    //id precedenti
    l1.Succ() = idl4;
    l2.Succ() = idl1;
    l3.Succ() = idl2;
    l4.Succ() = idl3;
    // triangoli
    TriangoloTest t1 = TriangoloTest(idt1,idp1,idp5,idp2,idl5,idl6,idl1);
    TriangoloTest t2 = TriangoloTest(idt2,idp2,idp5,idp3,idl6,idl7,idl2);
    TriangoloTest t3 = TriangoloTest(idt3,idp3,idp5,idp4,idl7,idl8,idl3);
    TriangoloTest t4 = TriangoloTest(idt4,idp4,idp5,idp1,idl8,idl5,idl4);
    idl8++;
    idt4++;
    // inizializzazione mesh
    MeshTest mesh;
    mesh.LP() = { p1,p2,p3,p4,p5 };
    mesh.LL()= { l1,l2,l3,l4,l5,l6,l7,l8 };
    mesh.LTR()  = { t1,t2,t3,t4 };
    mesh.HB() = idl1;

    Posizione DM;
    unsigned int esito = mesh.DentroMesh(p6, DM);
    string vero = to_string(2);
    string result = to_string(esito);
    EXPECT_EQ(vero, result);
    EXPECT_EQ(Posizione::BORDO_HULL, DM);
}

TEST(TestClassMesh, TestDentroMesh_esterno)
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
    unsigned int idt1 = 0;
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
    // punto nuovo, dentro primo triangolo
    double p6x = 12;
    double p6y = 3;
    unsigned int idpnew = idp1+5;
    Punto p6 = Punto(idpnew,p6x,p6y);
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
    l1.Prec() = idl2;
    l2.Prec() = idl3;
    l3.Prec() = idl4;
    l4.Prec() = idl1;
    //id precedenti
    l1.Succ() = idl4;
    l2.Succ() = idl1;
    l3.Succ() = idl2;
    l4.Succ() = idl3;
    // triangoli
    TriangoloTest t1 = TriangoloTest(idt1,idp1,idp5,idp2,idl5,idl6,idl1);
    TriangoloTest t2 = TriangoloTest(idt2,idp2,idp5,idp3,idl6,idl7,idl2);
    TriangoloTest t3 = TriangoloTest(idt3,idp3,idp5,idp4,idl7,idl8,idl3);
    TriangoloTest t4 = TriangoloTest(idt4,idp4,idp5,idp1,idl8,idl5,idl4);
    idl8++;
    idt4++;
    // inizializzazione mesh
    MeshTest mesh;
    mesh.LP() = { p1,p2,p3,p4,p5 };
    mesh.LL()= { l1,l2,l3,l4,l5,l6,l7,l8 };
    mesh.LTR()  = { t1,t2,t3,t4 };
    mesh.HB() = idl1;

    Posizione DM;
    unsigned int esito = mesh.DentroMesh(p6, DM);
    string vero = to_string(0);
    string result = to_string(esito);
    EXPECT_EQ(vero, result);
    EXPECT_EQ(Posizione::ESTERNO, DM);
}

TEST(TestClassMesh, TestDentroMesh_interno_generico)
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
    unsigned int idt1 = 0;
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
    // punto nuovo, dentro primo triangolo
    double p6x = 8;
    double p6y = 7;
    unsigned int idpnew = idp1+5;
    Punto p6 = Punto(idpnew,p6x,p6y);
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
    l1.Prec() = idl2;
    l2.Prec() = idl3;
    l3.Prec() = idl4;
    l4.Prec() = idl1;
    //id precedenti
    l1.Succ() = idl4;
    l2.Succ() = idl1;
    l3.Succ() = idl2;
    l4.Succ() = idl3;
    // triangoli
    TriangoloTest t1 = TriangoloTest(idt1,idp1,idp5,idp2,idl5,idl6,idl1);
    TriangoloTest t2 = TriangoloTest(idt2,idp2,idp5,idp3,idl6,idl7,idl2);
    TriangoloTest t3 = TriangoloTest(idt3,idp3,idp5,idp4,idl7,idl8,idl3);
    TriangoloTest t4 = TriangoloTest(idt4,idp4,idp5,idp1,idl8,idl5,idl4);
    idl8++;
    idt4++;
    // inizializzazione mesh
    MeshTest mesh;
    mesh.LP() = { p1,p2,p3,p4,p5 };
    mesh.LL()= { l1,l2,l3,l4,l5,l6,l7,l8 };
    mesh.LTR()  = { t1,t2,t3,t4 };
    mesh.HB() = idl1;

    Posizione DM;
    unsigned int esito = mesh.DentroMesh(p6, DM);
    string vero = to_string(2);
    string result = to_string(esito);
    EXPECT_EQ(vero, result);
    EXPECT_EQ(Posizione::INTERNO, DM);
}

// Collega Senza Intersezioni

TEST(TestClasseMesh, TestCollegaSenzaIntersezioni_basic)
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
    unsigned int idt1 = 0;
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
    // punto nuovo, dentro primo triangolo
    double pnewx = 12;
    double pnewy = 3;
    unsigned int idpnew = idp1+5;
    Punto pnew = Punto(idpnew,pnewx,pnewy);
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
    l1.Prec() = idl2;
    l2.Prec() = idl3;
    l3.Prec() = idl4;
    l4.Prec() = idl1;
    //id precedenti
    l1.Succ() = idl4;
    l2.Succ() = idl1;
    l3.Succ() = idl2;
    l4.Succ() = idl3;
    // triangoli
    TriangoloTest t1 = TriangoloTest(idt1,idp1,idp5,idp2,idl5,idl6,idl1);
    TriangoloTest t2 = TriangoloTest(idt2,idp2,idp5,idp3,idl6,idl7,idl2);
    TriangoloTest t3 = TriangoloTest(idt3,idp3,idp5,idp4,idl7,idl8,idl3);
    TriangoloTest t4 = TriangoloTest(idt4,idp4,idp5,idp1,idl8,idl5,idl4);
    idl8++;
    idt4++;
    // inizializzazione mesh
    MeshTest mesh;
    mesh.LP() = { p1,p2,p3,p4,p5,pnew };
    mesh.LL()= { l1,l2,l3,l4,l5,l6,l7,l8 };
    mesh.LTR()  = { t1,t2,t3,t4 };
    mesh.HB() = idl1;
    mesh.CollegaSenzaIntersezioni(pnew,idt4,idl8);
    string vero = "Punti\nId x y\n0 5.000000 2.000000\n1 2.000000 6.000000\n2 6.000000 9.000000\n3 14.000000 7.000000\n4 7.000000 "
                  "6.000000\n5 12.000000 3.000000\nLati\nId p1 p2 Length TriangoliAdiacenti\n0 1 0 5.000000 0\n1 2 1 5.000000 1\n2 "
                  "3 2 8.246211 2\n3 0 3 10.295630 3,4\n4 0 4 4.472136 0,3\n5 1 4 5.000000 1,0\n6 2 4 3.162278 2,1\n7 3 4 7.071068 "
                  "3,2\n8 0 5 7.071068 4\n9 5 3 4.472136 4\nTriangoli\nId p1 p2 p3 l1 l2 l3\n0 0 4 1 4 5 0\n1 1 4 2 5 6 1\n2 2 4 3 6 "
                  "7 2\n3 3 4 0 7 4 3\n4 0 5 3 8 9 3\n";
    string result = mesh.Show();
    EXPECT_EQ(vero, result);
    string vero_Hull_succ = " 0 8 9 2 1 0";
    unsigned int entra = 0;
    unsigned int i = mesh.HB();
    string sHull_succ = "";
    while(entra<2){
        if (i == mesh.HB())
            entra++;
        sHull_succ = sHull_succ + " " + to_string(i);
        LatoTest a = (mesh.LL())[i];
        i = a.Succ();
    }
    string vero_Hull_prec = " 0 1 2 9 8 0";
    entra = 0;
    i = mesh.HB();
    string sHull_prec = "";
    while(entra<2){
        if (i == mesh.HB())
            entra++;
        sHull_prec = sHull_prec + " " + to_string(i);
        LatoTest a = (mesh.LL())[i];
        i = a.Prec();
    }
    EXPECT_EQ(sHull_succ, vero_Hull_succ);
    EXPECT_EQ(sHull_prec, vero_Hull_prec);
    string vero_CD = "3,4 ";
    string sCD = "";
    for(array<unsigned int,2> i : mesh.CD())
        sCD = sCD + to_string(i[0]) + "," + to_string(i[1]) + " ";
    EXPECT_EQ(sCD, vero_CD);
    mesh.Show();
}

TEST(TestClasseMesh, TestCollegaSenzaIntersezioni_basic_HullBegin)
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
    unsigned int idt1 = 0;
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
    // punto nuovo, dentro primo triangolo
    double pnewx = 12;
    double pnewy = 3;
    unsigned int idpnew = idp1+5;
    Punto pnew = Punto(idpnew,pnewx,pnewy);
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
    l1.Prec() = idl2;
    l2.Prec() = idl3;
    l3.Prec() = idl4;
    l4.Prec() = idl1;
    //id precedenti
    l1.Succ() = idl4;
    l2.Succ() = idl1;
    l3.Succ() = idl2;
    l4.Succ() = idl3;
    // triangoli
    TriangoloTest t1 = TriangoloTest(idt1,idp1,idp5,idp2,idl5,idl6,idl1);
    TriangoloTest t2 = TriangoloTest(idt2,idp2,idp5,idp3,idl6,idl7,idl2);
    TriangoloTest t3 = TriangoloTest(idt3,idp3,idp5,idp4,idl7,idl8,idl3);
    TriangoloTest t4 = TriangoloTest(idt4,idp4,idp5,idp1,idl8,idl5,idl4);
    idl8++;
    idt4++;
    // inizializzazione mesh
    MeshTest mesh;
    mesh.LP() = { p1,p2,p3,p4,p5,pnew };
    mesh.LL()= { l1,l2,l3,l4,l5,l6,l7,l8 };
    mesh.LTR()  = { t1,t2,t3,t4 };
    mesh.HB() = idl3;
    mesh.CollegaSenzaIntersezioni(pnew,idt4,idl8);
    string vero = "Punti\nId x y\n0 5.000000 2.000000\n1 2.000000 6.000000\n2 6.000000 9.000000\n3 14.000000 7.000000\n4 7.000000 "
                  "6.000000\n5 12.000000 3.000000\nLati\nId p1 p2 Length TriangoliAdiacenti\n0 1 0 5.000000 0\n1 2 1 5.000000 1\n2 "
                  "3 2 8.246211 2\n3 0 3 10.295630 3,4\n4 0 4 4.472136 0,3\n5 1 4 5.000000 1,0\n6 2 4 3.162278 2,1\n7 3 4 7.071068 "
                  "3,2\n8 0 5 7.071068 4\n9 5 3 4.472136 4\nTriangoli\nId p1 p2 p3 l1 l2 l3\n0 0 4 1 4 5 0\n1 1 4 2 5 6 1\n2 2 4 3 6 "
                  "7 2\n3 3 4 0 7 4 3\n4 0 5 3 8 9 3\n";
    string result = mesh.Show();
    EXPECT_EQ(vero, result);
    string vero_Hull_succ = " 2 1 0 8 9 2";
    unsigned int entra = 0;
    unsigned int i = mesh.HB();
    string sHull_succ = "";
    while(entra<2){
        if (i == mesh.HB())
            entra++;
        sHull_succ = sHull_succ + " " + to_string(i);
        LatoTest a = (mesh.LL())[i];
        i = a.Succ();
    }
    string vero_Hull_prec = " 2 9 8 0 1 2";
    entra = 0;
    i = mesh.HB();
    string sHull_prec = "";
    while(entra<2){
        if (i == mesh.HB())
            entra++;
        sHull_prec = sHull_prec + " " + to_string(i);
        LatoTest a = (mesh.LL())[i];
        i = a.Prec();
    }
    EXPECT_EQ(sHull_succ, vero_Hull_succ);
    EXPECT_EQ(sHull_prec, vero_Hull_prec);
    string vero_CD = "3,4 ";
    string sCD = "";
    for(array<unsigned int,2> i : mesh.CD())
        sCD = sCD + to_string(i[0]) + "," + to_string(i[1]) + " ";
    EXPECT_EQ(sCD, vero_CD);
    EXPECT_EQ(mesh.HB(), idl3);
}

TEST(TestClasseMesh, TestCollegaSenzaIntersezioni_latoParallelo)
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
    unsigned int idt1 = 0;
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
    double p4x = 11;
    double p4y = 4;
    // coordinate p5
    double p5x = 7;
    double p5y = 6;
    // Punti
    Punto p1 = Punto(idp1,p1x,p1y);
    Punto p2 = Punto(idp2,p2x,p2y);
    Punto p3 = Punto(idp3,p3x,p3y);
    Punto p4 = Punto(idp4,p4x,p4y);
    Punto p5 = Punto(idp5,p5x,p5y);
    // punto nuovo, dentro primo triangolo
    double pnewx = 13;
    double pnewy = 2;
    unsigned int idpnew = idp1+5;
    Punto pnew = Punto(idpnew,pnewx,pnewy);
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
    l1.Prec() = idl2;
    l2.Prec() = idl3;
    l3.Prec() = idl4;
    l4.Prec() = idl1;
    //id precedenti
    l1.Succ() = idl4;
    l2.Succ() = idl1;
    l3.Succ() = idl2;
    l4.Succ() = idl3;
    // triangoli
    TriangoloTest t1 = TriangoloTest(idt1,idp1,idp5,idp2,idl5,idl6,idl1);
    TriangoloTest t2 = TriangoloTest(idt2,idp2,idp5,idp3,idl6,idl7,idl2);
    TriangoloTest t3 = TriangoloTest(idt3,idp3,idp5,idp4,idl7,idl8,idl3);
    TriangoloTest t4 = TriangoloTest(idt4,idp4,idp5,idp1,idl8,idl5,idl4);
    idl8++;
    idt4++;
    // inizializzazione mesh
    MeshTest mesh;
    mesh.LP() = { p1,p2,p3,p4,p5,pnew };
    mesh.LL()= { l1,l2,l3,l4,l5,l6,l7,l8 };
    mesh.LTR()  = { t1,t2,t3,t4 };
    mesh.HB() = idl1;
    mesh.CollegaSenzaIntersezioni(pnew,idt4,idl8);
    string vero = "Punti\nId x y\n0 5.000000 2.000000\n1 2.000000 6.000000\n2 6.000000 9.000000\n3 11.000000 4.000000\n4 7.000000 6.000000\n5 13.000000 "
                  "2.000000\nLati\nId p1 p2 Length TriangoliAdiacenti\n0 1 0 5.000000 0\n1 2 1 5.000000 1\n2 3 2 7.071068 2\n3 0 3 6.324555 3,4\n4 0 4 "
                  "4.472136 0,3\n5 1 4 5.000000 1,0\n6 2 4 3.162278 2,1\n7 3 4 4.472136 3,2\n8 0 5 8.000000 4\n9 5 3 2.828427 4\nTriangoli\nId p1 p2 p3 "
                  "l1 l2 l3\n0 0 4 1 4 5 0\n1 1 4 2 5 6 1\n2 2 4 3 6 7 2\n3 3 4 0 7 4 3\n4 0 5 3 8 9 3\n";
    string result = mesh.Show();
    EXPECT_EQ(vero, result);
    string vero_Hull_succ = " 0 8 9 2 1 0";
    unsigned int entra = 0;
    unsigned int i = mesh.HB();
    string sHull_succ = "";
    while(entra<2){
        if (i == mesh.HB())
            entra++;
        sHull_succ = sHull_succ + " " + to_string(i);
        LatoTest a = (mesh.LL())[i];
        i = a.Succ();
    }
    string vero_Hull_prec = " 0 1 2 9 8 0";
    entra = 0;
    i = mesh.HB();
    string sHull_prec = "";
    while(entra<2){
        if (i == mesh.HB())
            entra++;
        sHull_prec = sHull_prec + " " + to_string(i);
        LatoTest a = (mesh.LL())[i];
        i = a.Prec();
    }
    EXPECT_EQ(sHull_succ, vero_Hull_succ);
    EXPECT_EQ(sHull_prec, vero_Hull_prec);
    string vero_CD = "3,4 ";
    string sCD = "";
    for(array<unsigned int,2> i : mesh.CD())
        sCD = sCD + to_string(i[0]) + "," + to_string(i[1]) + " ";
    EXPECT_EQ(sCD, vero_CD);
}

TEST(TestClasseMesh, TestCollegaSenzaIntersezioni_lati3Punti)
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
    unsigned int idt1 = 0;
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
    double p4y = 6;
    // coordinate p5
    double p5x = 7;
    double p5y = 6;
    // Punti
    Punto p1 = Punto(idp1,p1x,p1y);
    Punto p2 = Punto(idp2,p2x,p2y);
    Punto p3 = Punto(idp3,p3x,p3y);
    Punto p4 = Punto(idp4,p4x,p4y);
    Punto p5 = Punto(idp5,p5x,p5y);
    // punto nuovo, dentro primo triangolo
    double pnewx = 21;
    double pnewy = 6;
    unsigned int idpnew = idp1+5;
    Punto pnew = Punto(idpnew,pnewx,pnewy);
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
    l1.Prec() = idl2;
    l2.Prec() = idl3;
    l3.Prec() = idl4;
    l4.Prec() = idl1;
    //id precedenti
    l1.Succ() = idl4;
    l2.Succ() = idl1;
    l3.Succ() = idl2;
    l4.Succ() = idl3;
    // triangoli
    TriangoloTest t1 = TriangoloTest(idt1,idp1,idp5,idp2,idl5,idl6,idl1);
    TriangoloTest t2 = TriangoloTest(idt2,idp2,idp5,idp3,idl6,idl7,idl2);
    TriangoloTest t3 = TriangoloTest(idt3,idp3,idp5,idp4,idl7,idl8,idl3);
    TriangoloTest t4 = TriangoloTest(idt4,idp4,idp5,idp1,idl8,idl5,idl4);
    idl8++;
    idt4++;
    // inizializzazione mesh
    MeshTest mesh;
    mesh.LP() = { p1,p2,p3,p4,p5,pnew };
    mesh.LL()= { l1,l2,l3,l4,l5,l6,l7,l8 };
    mesh.LTR()  = { t1,t2,t3,t4 };
    mesh.HB() = idl1;
    mesh.CollegaSenzaIntersezioni(pnew,idt4,idl8);
    string vero = "Punti\nId x y\n0 5.000000 2.000000\n1 2.000000 6.000000\n2 6.000000 9.000000\n3 14.000000 6.000000\n4 "
                  "7.000000 6.000000\n5 21.000000 6.000000\nLati\nId p1 p2 Length TriangoliAdiacenti\n0 1 0 5.000000 0\n1"
                  " 2 1 5.000000 1\n2 3 2 8.544004 2,5\n3 0 3 9.848858 3,4\n4 0 4 4.472136 0,3\n5 1 4 5.000000 1,0\n6 2 4 "
                  "3.162278 2,1\n7 3 4 7.000000 3,2\n8 0 5 16.492423 4\n9 3 5 7.000000 5,4\n10 5 2 15.297059 5\nTriangoli\nId "
                  "p1 p2 p3 l1 l2 l3\n0 0 4 1 4 5 0\n1 1 4 2 5 6 1\n2 2 4 3 6 7 2\n3 3 4 0 7 4 3\n4 0 5 3 8 9 3\n5 3 5 2 9 10 "
                  "2\n";
    string result = mesh.Show();
    EXPECT_EQ(vero, result);
    string vero_Hull_succ = " 0 8 10 1 0";
    unsigned int entra = 0;
    unsigned int i = mesh.HB();
    string sHull_succ = "";
    while(entra<2){
        if (i == mesh.HB())
            entra++;
        sHull_succ = sHull_succ + " " + to_string(i);
        LatoTest a = (mesh.LL())[i];
        i = a.Succ();
    }
    string vero_Hull_prec = " 0 1 10 8 0";
    entra = 0;
    i = mesh.HB();
    string sHull_prec = "";
    while(entra<2){
        if (i == mesh.HB())
            entra++;
        sHull_prec = sHull_prec + " " + to_string(i);
        LatoTest a = (mesh.LL())[i];
        i = a.Prec();
    }
    EXPECT_EQ(sHull_succ, vero_Hull_succ);
    EXPECT_EQ(sHull_prec, vero_Hull_prec);
    string vero_CD = "3,4 2,5 ";
    string sCD = "";
    for(array<unsigned int,2> i : mesh.CD())
        sCD = sCD + to_string(i[0]) + "," + to_string(i[1]) + " ";
    EXPECT_EQ(sCD, vero_CD);
}

TEST(TestClasseMesh, TestCollegaSenzaIntersezioni_Rettangolo)
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
    unsigned int idl9 = idl1+8;
    // id triangoli
    unsigned int idt0 = 0;
    unsigned int idt1 = 1;
    unsigned int idt2 = idt0+2;
    unsigned int idt3 = idt0+3;
    // coordinate p1
    double p1x = 1;
    double p1y = 1;
    // coordinate p2
    double p2x = 4;
    double p2y = 1;
    // coordinate p3
    double p3x = 4;
    double p3y = 3;
    // coordinate p4
    double p4x = 4;
    double p4y = 4;
    // coordinate p5
    double p5x = 4;
    double p5y = 5;
    // coordinate e punto p6
    double p6x = 1;
    double p6y = 5;
    unsigned int idp6 = idp1+5;
    // Punti
    Punto p1 = Punto(idp1,p1x,p1y);
    Punto p2 = Punto(idp2,p2x,p2y);
    Punto p3 = Punto(idp3,p3x,p3y);
    Punto p4 = Punto(idp4,p4x,p4y);
    Punto p5 = Punto(idp5,p5x,p5y);
    Punto p6 = Punto(idp6,p6x,p6y);
    // punto controllo
    double pnewx = 4;
    double pnewy = 0;
    unsigned int idpnew = idp1+6;
    Punto pnew = Punto(idpnew,pnewx,pnewy);
    // Lati
    LatoTest l1 = LatoTest(idl1, p1, p2, idt0);
    LatoTest l2 = LatoTest(idl2, p2, p3, idt0);
    LatoTest l3 = LatoTest(idl3, p3, p4, idt2);
    LatoTest l4 = LatoTest(idl4, p4, p5, idt3);
    LatoTest l5 = LatoTest(idl5, p5, p6, idt3);
    LatoTest l6 = LatoTest(idl6, p6, p1, idt1);
    LatoTest l7 = LatoTest(idl7, p1, p3, idt0);
    LatoTest l8 = LatoTest(idl8, p3, p6, idt1);
    LatoTest l9 = LatoTest(idl9, p6, p4, idt2);
    // id dilati interni
    l7.LIT().push_back(idt1);
    l8.LIT().push_back(idt2);
    l9.LIT().push_back(idt3);
    // id successivi
    l1.Prec() = idl6;
    l2.Prec() = idl1;
    l3.Prec() = idl2;
    l4.Prec() = idl3;
    l5.Prec() = idl4;
    l6.Prec() = idl5;
    //id precedenti
    l1.Succ() = idl2;
    l2.Succ() = idl3;
    l3.Succ() = idl4;
    l4.Succ() = idl5;
    l5.Succ() = idl6;
    l6.Succ() = idl1;
    // triangoli
    TriangoloTest t1 = TriangoloTest(idt0,idp1,idp2,idp3,idl1,idl2,idl7);
    TriangoloTest t2 = TriangoloTest(idt1,idp1,idp3,idp6,idl7,idl8,idl6);
    TriangoloTest t3 = TriangoloTest(idt2,idp3,idp4,idp6,idl3,idl9,idl8);
    TriangoloTest t4 = TriangoloTest(idt3,idp4,idp5,idp6,idl4,idl5,idl9);
    idl9++;
    idt3++;
    // inizializzazione mesh
    MeshTest mesh;
    mesh.LP() = { p1,p2,p3,p4,p5,p6 };
    mesh.LL()= { l1,l2,l3,l4,l5,l6,l7,l8,l9 };
    mesh.LTR()  = { t1,t2,t3,t4 };
    mesh.HB() = idl3;
    mesh.CollegaSenzaIntersezioni(pnew,idt3,idl9);
    string vero = "Punti\nId x y\n0 1.000000 1.000000\n1 4.000000 1.000000\n2 4.000000 3.000000\n3 4.000000 4.000000\n4 "
                  "4.000000 5.000000\n5 1.000000 5.000000\nLati\nId p1 p2 Length TriangoliAdiacenti\n0 0 1 3.000000 0,4\n1 "
                  "1 2 2.000000 0\n2 2 3 1.000000 2\n3 3 4 1.000000 3\n4 4 5 3.000000 3\n5 5 0 4.000000 1\n6 0 2 3.605551 0,1\n7"
                  " 2 5 3.605551 1,2\n8 5 3 3.162278 2,3\n9 0 6 3.162278 4\n10 6 1 1.000000 4\nTriangoli\nId p1 p2 p3 l1 l2 l3\n0"
                  " 0 1 2 0 1 6\n1 0 2 5 6 7 5\n2 2 3 5 2 8 7\n3 3 4 5 3 4 8\n4 0 6 1 9 10 0\n";
    string result = mesh.Show();
    EXPECT_EQ(vero, result);
    string vero_Hull_succ = " 2 3 4 5 9 10 1 2";
    unsigned int entra = 0;
    unsigned int i = mesh.HB();
    string sHull_succ = "";
    while(entra<2){
        if (i == mesh.HB())
            entra++;
        sHull_succ = sHull_succ + " " + to_string(i);
        LatoTest a = (mesh.LL())[i];
        i = a.Succ();
    }
    string vero_Hull_prec = " 2 1 10 9 5 4 3 2";
    entra = 0;
    i = mesh.HB();
    string sHull_prec = "";
    while(entra<2){
        if (i == mesh.HB())
            entra++;
        sHull_prec = sHull_prec + " " + to_string(i);
        LatoTest a = (mesh.LL())[i];
        i = a.Prec();
    }
    EXPECT_EQ(sHull_succ, vero_Hull_succ);
    EXPECT_EQ(sHull_prec, vero_Hull_prec);
    string vero_CD = "0,4 ";
    string sCD = "";
    for(array<unsigned int,2> i : mesh.CD())
        sCD = sCD + to_string(i[0]) + "," + to_string(i[1]) + " ";
    EXPECT_EQ(sCD, vero_CD);
}

// Accettabile

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
    unsigned int idt1 = 0;
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
    // punto nuovo, dentro primo triangolo
    double p6x = 12;
    double p6y = 3;
    unsigned int idpnew = idp1+5;
    Punto p6 = Punto(idpnew,p6x,p6y);
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
    l1.Prec() = idl2;
    l2.Prec() = idl3;
    l3.Prec() = idl4;
    l4.Prec() = idl1;
    //id precedenti
    l1.Succ() = idl4;
    l2.Succ() = idl1;
    l3.Succ() = idl2;
    l4.Succ() = idl3;
    // triangoli
    TriangoloTest t1 = TriangoloTest(idt1,idp1,idp5,idp2,idl5,idl6,idl1);
    TriangoloTest t2 = TriangoloTest(idt2,idp2,idp5,idp3,idl6,idl7,idl2);
    TriangoloTest t3 = TriangoloTest(idt3,idp3,idp5,idp4,idl7,idl8,idl3);
    TriangoloTest t4 = TriangoloTest(idt4,idp4,idp5,idp1,idl8,idl5,idl4);
    idl8++;
    idt4++;
    // inizializzazione mesh
    MeshTest mesh;
    mesh.LP() = { p1,p2,p3,p4,p5,p6 };
    mesh.LL()= { l1,l2,l3,l4,l5,l6,l7,l8 };
    mesh.LTR()  = { t1,t2,t3,t4 };
    mesh.HB() = idl1;
    // inizializzazione mesh (finalmente)
    vector<Punto> vertici = { p1,p2,p3,p4,p5 };
    vector<Lato> lati = { l1,l2,l3,l4,l5,l6,l7,l8 };
    vector<Triangolo> triangoli = { t1,t2,t3,t4 };
    mesh.LP() = vertici;
    mesh.LL()= lati;
    mesh.LTR()  = triangoli;
    mesh.HB() = idl1;
    // punto nuovo
    p6x = 14;
    p6y = 9;
    idpnew = idp1+5;
    p6 = Punto(idpnew,p6x,p6y);
    // test accettabile
    string result = " ";
    for(unsigned int i = 0; i<4; i++){
        if(mesh.accettabile(p6, mesh.LP()[i]))
        {
            result = result + "T ";
        }else{
            result = result + "F ";
        }
    }
    string vero = " F F T T ";
    EXPECT_EQ(vero, result);
    // mesh 3 punti collineari
    p4x = 14;
    p4y = 6;
    p4 = Punto(idp4,p4x,p4y);
    p6x = 21;
    p6y = 9;
    p6 = Punto(idpnew,p6x,p6y);
    LatoTest l3new = LatoTest(idl3, p4, p3, idt3);
    LatoTest l4new = LatoTest(idl4, p1, p4, idt4);
    l3new.Prec() = idl4;
    l4new.Prec() = idl1;
    l3new.Succ() = idl2;
    l4new.Succ() = idl3;
    //mesh.LP()[5]=Punto(idpnew,p6x,p6y);
    mesh.LP()[3]=Punto(idp4,p4x,p4y);
    mesh.LL()= { l1,l2,l3new,l4new,l5,l6,l7,l8 };
    //pezzo del test
    result = " ";
    for(unsigned int i = 0; i<4; i++){
        if(mesh.accettabile(p6, mesh.LP()[i]))
        {
            result = result + "T ";
        }else{
            result = result + "F ";
        }
    }
    vero = " T F T T ";
    EXPECT_EQ(vero, result);
}

TEST(TestClasseMesh, TestAccettabileCoincidente)
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
    unsigned int idt1 = 0;
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
    double p4x = 11;
    double p4y = 4;
    // coordinate p5
    double p5x = 7;
    double p5y = 6;
    // Punti
    Punto p1 = Punto(idp1,p1x,p1y);
    Punto p2 = Punto(idp2,p2x,p2y);
    Punto p3 = Punto(idp3,p3x,p3y);
    Punto p4 = Punto(idp4,p4x,p4y);
    Punto p5 = Punto(idp5,p5x,p5y);
    // punto nuovo, dentro primo triangolo
    double pnewx = 13;
    double pnewy = 2;
    unsigned int idpnew = idp1+5;
    Punto pnew = Punto(idpnew,pnewx,pnewy);
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
    l1.Prec() = idl2;
    l2.Prec() = idl3;
    l3.Prec() = idl4;
    l4.Prec() = idl1;
    //id precedenti
    l1.Succ() = idl4;
    l2.Succ() = idl1;
    l3.Succ() = idl2;
    l4.Succ() = idl3;
    // triangoli
    TriangoloTest t1 = TriangoloTest(idt1,idp1,idp5,idp2,idl5,idl6,idl1);
    TriangoloTest t2 = TriangoloTest(idt2,idp2,idp5,idp3,idl6,idl7,idl2);
    TriangoloTest t3 = TriangoloTest(idt3,idp3,idp5,idp4,idl7,idl8,idl3);
    TriangoloTest t4 = TriangoloTest(idt4,idp4,idp5,idp1,idl8,idl5,idl4);
    idl8++;
    idt4++;
    // inizializzazione mesh
    MeshTest mesh;
    mesh.LP() = { p1,p2,p3,p4,p5,pnew };
    mesh.LL()= { l1,l2,l3,l4,l5,l6,l7,l8 };
    mesh.LTR()  = { t1,t2,t3,t4 };
    mesh.HB() = idl1;
    string result = " ";
    for(unsigned int i = 0; i<4; i++){
        if(mesh.accettabile(pnew, mesh.LP()[i]))
        {
            result = result + "T ";
        }else{
            result = result + "F ";
        }
    }
    string vero = " T F F T ";
    EXPECT_EQ(vero, result);
}

TEST(TestClasseMesh, TestAccettabileRettangolo)
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
    unsigned int idl9 = idl1+8;
    // id triangoli
    unsigned int idt0 = 0;
    unsigned int idt1 = 1;
    unsigned int idt2 = idt1+2;
    unsigned int idt3 = idt1+3;
    // coordinate p1
    double p1x = 1;
    double p1y = 1;
    // coordinate p2
    double p2x = 4;
    double p2y = 1;
    // coordinate p3
    double p3x = 4;
    double p3y = 3;
    // coordinate p4
    double p4x = 4;
    double p4y = 4;
    // coordinate p5
    double p5x = 4;
    double p5y = 5;
    // coordinate e punto p6
    double p6x = 1;
    double p6y = 5;
    unsigned int idp6 = idp1+5;
    // Punti
    Punto p1 = Punto(idp1,p1x,p1y);
    Punto p2 = Punto(idp2,p2x,p2y);
    Punto p3 = Punto(idp3,p3x,p3y);
    Punto p4 = Punto(idp4,p4x,p4y);
    Punto p5 = Punto(idp5,p5x,p5y);
    Punto p6 = Punto(idp6,p6x,p6y);
    // punto controllo
    double pnewx = 4;
    double pnewy = 0;
    unsigned int idpnew = idp1+5;
    Punto pnew = Punto(idpnew,pnewx,pnewy);
    // Lati
    LatoTest l1 = LatoTest(idl1, p1, p2, idt0);
    LatoTest l2 = LatoTest(idl2, p2, p3, idt0);
    LatoTest l3 = LatoTest(idl3, p3, p4, idt2);
    LatoTest l4 = LatoTest(idl4, p4, p5, idt3);
    LatoTest l5 = LatoTest(idl5, p5, p6, idt3);
    LatoTest l6 = LatoTest(idl6, p6, p1, idt1);
    LatoTest l7 = LatoTest(idl7, p1, p3, idt0);
    LatoTest l8 = LatoTest(idl8, p3, p6, idt1);
    LatoTest l9 = LatoTest(idl8, p6, p4, idt2);
    // id dilati interni
    l7.LIT().push_back(idt1);
    l8.LIT().push_back(idt2);
    l9.LIT().push_back(idt3);
    // id successivi
    l1.Prec() = idl6;
    l2.Prec() = idl1;
    l3.Prec() = idl2;
    l4.Prec() = idl3;
    l5.Prec() = idl4;
    l6.Prec() = idl5;
    //id precedenti
    l1.Succ() = idl2;
    l2.Succ() = idl3;
    l3.Succ() = idl4;
    l4.Succ() = idl5;
    l5.Succ() = idl6;
    l6.Succ() = idl1;
    // triangoli
    TriangoloTest t1 = TriangoloTest(idt0,idp1,idp2,idp3,idl1,idl2,idl7);
    TriangoloTest t2 = TriangoloTest(idt1,idp1,idp3,idp6,idl7,idl8,idl6);
    TriangoloTest t3 = TriangoloTest(idt2,idp3,idp4,idp6,idl3,idl9,idl8);
    TriangoloTest t4 = TriangoloTest(idt3,idp4,idp5,idp6,idl4,idl5,idl9);
    idl8++;
    idt3++;
    // inizializzazione mesh
    MeshTest mesh;
    mesh.LP() = { p1,p2,p3,p4,p5,p6 };
    mesh.LL()= { l1,l2,l3,l4,l5,l6,l7,l8,l9 };
    mesh.LTR()  = { t1,t2,t3,t4 };
    mesh.HB() = idl3;
    string result = " ";
    for(unsigned int i = 0; i<6; i++){
        if(mesh.accettabile(pnew, mesh.LP()[i]))
        {
            result = result + "T ";
        }else{
            result = result + "F ";
        }
    }
    string vero = " T T F F F F ";
    EXPECT_EQ(vero, result);
}


TEST(TestClassMesh, TestCostruttoreMesh)
{   
    unsigned int idp = 0;
    double c1 = 0;
    double c2 = 1;
    double c3 = 3;
    double c4 = 0.5;
    double c5 = 4;
    double c6 = 2;
    Punto p1 = Punto(idp,c1,c1);
    idp++;
    Punto p2 = Punto(idp,c2,c1);
    idp++;
    Punto p3 = Punto(idp,c4,c1);
    idp++;
    Punto p4 = Punto(idp,c3,c3);
    idp++;
    Punto p5 = Punto(idp,c5,c6);
    idp++;
    Punto p6 = Punto(idp,c2,c6);
    vector<Punto> points = {p1, p2, p3, p4, p5, p6};
    MeshTest mh = MeshTest(points);

    string vero_mesh = "Punti\nId x y\n0 0.000000 0.000000\n1 1.000000 0.000000\n2 0.500000 0.000000\n"
                       "3 3.000000 3.000000\n4 4.000000 2.000000\n5 1.000000 2.000000\nLati\n"
                       "Id p1 p2 Length TriangoliAdiacenti\n0 1 5 2.000000 2,0\n1 4 5 3.000000 0,3\n"
                       "2 5 0 2.236068 1\n3 0 2 0.500000 1\n4 1 4 3.605551 0\n5 2 5 2.061553 1,2\n"
                       "6 2 1 0.500000 2\n7 4 3 1.414214 3\n8 3 5 2.236068 3\nTriangoli\nId p1 p2 p3 l1 l2 l3\n"
                       "0 4 5 1 1 0 4\n1 0 2 5 3 5 2\n2 2 1 5 6 0 5\n3 4 3 5 7 8 1\n";
    EXPECT_EQ(mh.Show(), vero_mesh);

    string vero_Hull_succ = " 2 3 6 4 7 8 2";
    unsigned int entra = 0;
    unsigned int i = mh.HB();
    string sHull_succ = "";
    while(entra<2){
        if (i == mh.HB())
            entra++;
        sHull_succ = sHull_succ + " " + to_string(i);
        LatoTest a = (mh.LL())[i];
        i = a.Succ();
    }
    string vero_Hull_prec = " 2 8 7 4 6 3 2";
    entra = 0;
    i = mh.HB();
    string sHull_prec = "";
    while(entra<2){
        if (i == mh.HB())
            entra++;
        sHull_prec = sHull_prec + " " + to_string(i);
        LatoTest a = (mh.LL())[i];
        i = a.Prec();
    }
    EXPECT_EQ(sHull_succ, vero_Hull_succ);
    EXPECT_EQ(sHull_prec, vero_Hull_prec);
}





#endif // __TEST_H
