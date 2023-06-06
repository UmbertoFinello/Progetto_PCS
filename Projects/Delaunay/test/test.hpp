#ifndef __TEST_H
#define __TEST_H

#include <gtest/gtest.h>
#include "Delaunay_class.hpp"
#include "sorting.hpp"

using namespace testing;
using namespace ProjectLibrary;
using namespace SortLibrary;


TEST(ClassPunto, PuntoConstructor)
{
    unsigned int id = 4;
    double x = 0;
    double y = 0;

    string right = to_string(id) + " " + to_string(x) + " " + to_string(y);

    Punto p = Punto(id,x,y);

    EXPECT_EQ(p.Show(),right);
}

TEST(ClassPunto, PuntoCopy)
{
    unsigned int id = 4;
    double x = 0;
    double y = 0;
    Punto p = Punto(id,x,y);
    Punto copy = Punto(p);

    EXPECT_EQ(copy.Show(),p.Show());
}

TEST(ClassLato, LatoConstructor_x)
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

TEST(ClassLato, LatoConstructor_y)
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

TEST(ClassLato, LatoConstructor_pitagora)
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

TEST(ClassTriangolo, TriangoloConstructor)
{
    unsigned int id = 4;
    unsigned int idlato = 0;
    unsigned int idl = 0;
    double c1 = 0;
    double c2 = 1;
    Punto p1 = Punto(idl,c1,c1);
    idl++;
    Punto p2 = Punto(idl,c2,c1);
    idl++;
    Punto p3 = Punto(idl,c1,c2);
    vector<Lato> vlat1;
    idl = 0;
    Lato l12 = Lato(idl, p1, p2, id);
    idl++;
    Lato l23 = Lato(idl, p2, p3, id);
    vector<Lato> vlat2 = {l12, l23};
    idl++;
    Lato l31 = Lato(idl, p3, p1, id);
    vector<Lato> vlat3 = {l12, l23, l31};
    string right = to_string(id) + " " + to_string(0) + "," + to_string(1) + "," + to_string(2) + " " +
        to_string(0) + "," + to_string(1) + "," + to_string(2);
    vector<Lato>* pvlat = &vlat1;
    Triangolo tr1 = Triangolo(id, p1, p2, p3, idlato, pvlat);
    idlato = 2;
    pvlat = &vlat2;
    Triangolo tr2 = Triangolo(id, p1, p2, p3, idlato, pvlat);
    pvlat = &vlat3;
    Triangolo tr3 = Triangolo(id, p1, p2, p3, idlato, pvlat);
    EXPECT_EQ(tr1.Show(),right);
    EXPECT_EQ(tr2.Show(),right);
    EXPECT_EQ(tr3.Show(),right);
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

TEST(TestProduct, TestcrossProduct)
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
    double vero2 = -3.5;
    double vero3 = 6.25;

    EXPECT_EQ(vero1, crossProduct(p3,p3));
    EXPECT_EQ(vero2, crossProduct(p3,p2));
    EXPECT_EQ(vero3, crossProduct(p3,p1));
}







#endif // __TEST_H
