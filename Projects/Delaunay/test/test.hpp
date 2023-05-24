#ifndef __TEST_H
#define __TEST_H

#include <gtest/gtest.h>

//#include "Delaunay_class.hpp"
#include "Delaunay_class.cpp"

using namespace testing;
using namespace ProjectLibrary;

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
    Punto p = Punto(id,x,y);
    Punto copy = Punto(p);

    EXPECT_EQ(copy.Show(),p.show);
}

TEST(ClassLato, LatoConstructor_x)
{
    unsigned int id = 4;
    Punto p1 = Punto(0,0,0);
    Punto p2 = Punto(1,1,0);
    double length = 1;

    string right = to_string(id) + " " + p1.Show() + " " + p2.Show() + " " + to_string(length);

    Lato l = Lato(id,p1,p2);

    EXPECT_EQ(l.Show(),right);

}

TEST(ClassLato, LatoConstructor_y)
{
    unsigned int id = 4;
    Punto p1 = Punto(0,0,0);
    Punto p2 = Punto(1,0,1);
    double length = 1;

    string right = to_string(id) + " " + p1.Show() + " " + p2.Show() + " " + to_string(length);

    Lato l = Lato(id,p1,p2);

    EXPECT_EQ(l.Show(),right);

}

TEST(ClassLato, LatoConstructor_pitagora)
{
    unsigned int id = 4;
    Punto p1 = Punto(0,0,0);
    Punto p2 = Punto(1,1,1);
    double length = sqrt(2);

    string right = to_string(id) + " " + p1.Show() + " " + p2.Show() + " " + to_string(length);

    Lato l = Lato(id,p1,p2);

    EXPECT_EQ(l.Show(),right);

}

/*
TEST(ClassTriangolo, TriangoloConstructor)
{
    unsigned int id = 4;
    Punto p1 = Punto(0,0,0);
    Punto p2 = Punto(1,1,0);
    Punto p3 = Punto(2,0,1);

    Lato l12 = Lato(0,p1,p2);
    Lato l23 = Lato(0,p2,p3);;
    Lato l31 = Lato(0,p3,p1);;

    string right = to_string(id) + " " + p1.Show() + " " + p2.Show() + " " + to_string(length);

    Lato l = Lato(id,p1,p2);

    EXPECT_EQ(l.Show(),right);

}
*/







#endif // __TEST_H
