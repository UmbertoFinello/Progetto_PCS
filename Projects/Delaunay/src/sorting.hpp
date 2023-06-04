#ifndef __SORTING_H
#define __SORTING_H

#include "iostream"
#include "list"
#include "Eigen/Eigen"
#include "map"

using namespace std;
using namespace Eigen;

namespace SortLibrary
{
    template<typename T>
    void Merge(vector<T>& v,
               const unsigned int& sx,
               const unsigned int& cx,
               const unsigned int& dx,
               const bool& assex)
    {
      unsigned int i = sx;
      unsigned int j = cx+1;
      unsigned int k = 0;
      vector<T> b = v;
      if(assex){
        while((i <= cx) && (j <= dx)) {
          if(v[i]._x <= v[j]._x){
              b[k] = v[i];
              i++;
          } else {
              b[k] = v[j];
              j++;
          }
          k++;
        }
      }else{
        while((i <= cx) && (j <= dx)) {
          if(v[i]._y <= v[j]._y) {
              b[k] = v[i];
              i++;
          } else {
              b[k] = v[j];
              j++;
          }
          k++;
        }
      }
      for ( ; i <= cx ; i++, k++)
        b[k] = v[i];
      for ( ; j <= dx ; j++, k++)
        b[k] = v[j];
      for (i = sx; i <= dx; i++)
        v[i] = b[i-sx];
    }

    template<typename T>
    void MergeSort(vector<T>& v,
                   const unsigned int& sx,
                   const unsigned int& dx,
                   const bool& assex)
    {
      if (sx < dx) {
        unsigned int cx = (sx+dx)/2;
        MergeSort(v, sx, cx, assex);
        MergeSort(v, cx+1, dx, assex);
        Merge(v, sx, cx, dx, assex);
      }
      return;
    }


}

#endif // __SORTING_H
