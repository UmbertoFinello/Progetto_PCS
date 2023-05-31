#ifndef __SORTING_H
#define __SORTING_H

#include "iostream"
#include "list"
#include "Eigen/Eigen"
#include "map"
#include "Delaunay_class.hpp"

using namespace ProjectLibrary;
using namespace std;
using namespace Eigen;

namespace SortLibrary
{
  void Merge(vector<Punto>& v,
             const unsigned int& sx,
             const unsigned int& cx,
             const unsigned int& dx,
             const bool& assex)
  {
      unsigned int i = sx;
      unsigned int j = cx + 1;

      vector<Punto> b;

      while( (i<=cx) && (j <= dx) )
      {
          if(assex)
          {
              if( v[i]._x <= v[j]._x )
              {
                  b.push_back(v[i]);
                  i++;
              }
              else
              {
                  b.push_back(v[j]);
                  j++;
              }
          }
          else
          {
              if( v[i]._y <= v[j]._y )
              {
                  b.push_back(v[i]);
                  i++;
              }
              else
              {
                  b.push_back(v[j]);
                  j++;
              }
          }
      }
      for ( ; i <= cx ; i++)
        b.push_back(v[i]);
      for ( ; j <= dx ; j++)
        b.push_back(v[j]);
      for ( i = sx ; i <= dx ; i++ )
        v[i] = Punto(b[i-sx]);
  }

  void MergeSort(vector<Punto>& v,
                 const unsigned int& sx,
                 const unsigned int& dx,
                 const bool& assex)
  {
    if (sx < dx)
    {
        unsigned int cx = ( sx + dx ) / 2;
        MergeSort(v, sx, cx, assex);
        MergeSort(v, cx + 1 , dx, assex);
        Merge(v,sx,cx,dx,axis);
    }
  }

}

#endif // __SORTING_H
