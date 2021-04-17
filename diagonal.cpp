//
// Created by Leila on 3/5/2021.
//

#include "diagonal.h"
#include <iostream>
#include <iomanip>
#include <chrono>


diagonal::diagonal(int r, int c, int d)
{
    row = r;
    col = c;
    dNum = d;
}
diagonal::diagonal(int d)
{

    dNum = d;
}
diagonal::~diagonal()
{

}

int * diagonal::offsetCompute(int a)
{
    offset = new int[dNum]();

    if(a == -1)
    {
        cout << "offset:";
        for (int i=1; i<=dNum/2; i++) {

            offset[dNum / 2] = 0;
            offset[(dNum / 2) - i] = -i;
            offset[(dNum / 2) + i] = i;
        }
    }
    if(a == 1)
    {
       // cout << "offset:";
        for (int i=0; i<=dNum/2; i++) {

             offset[i] = i;
           // cout<<offset[i]<<","<<"\n";
        }
    }

   /*  for (int j = 0; j < dNum; ++j)
   {
       cout<<offset[j];
       cout<<"\n";
   }*/
   cout<<"\n";
    return offset;
}

Matrix* diagonal::diaStore(Matrix *m, int a)
{
    if (a == -1) {
         offset = new int[dNum]();
        Matrix *dd = new Matrix(dNum, m->colNo, " turn a diagonal matrix to a dense matrix:");

        for (int i = 1; i <= dNum / 2; i++) {
            for (int j = 0; j < m->colNo; j++) {
                dd->array[(dNum / 2)][j] = m->array[j][j];

                if (j + i < m->rowNo) {
                    dd->array[(dNum / 2) - i][j] = m->array[j][j + i];

                    dd->array[(dNum / 2) + i][j] = m->array[j + i][j];
                }

            }
              offset[dNum/2]=0;
              offset[(dNum/2) - i]=-i;
              offset[(dNum/2) + i]=i;
        }
        return dd;
    }
   else if (a == 1) {
        Matrix *dd = new Matrix((dNum/2)+1, m->colNo, " turn a diagonal matrix to a dense matrix:");

        for (int i = 0; i <= dNum / 2; i++) {
            for (int j = 0; j < m->colNo; j++) {
                if(i==0)
                {
                    dd->array[i][j] = m->array[j][j];
                }

                else if (j + i < m->rowNo)
                {
                    dd->array[i][j] = m->array[j+i][j];
                }

            }
        }
        return dd;
    }
    return 0;
}

 Matrix* diagonal::diaMult(Matrix *d,Matrix *v)
 {
     int i, j;

     Matrix *result = new Matrix(row, 1, " vector:");


     for (int l = 0; l < d->rowNo; l++) {
         for (int k = 0; k < d->colNo - abs(offset[l]); k++) {
             if (offset[l] > 0) {
                 i = k + offset[l];
                 j = k;

                 result->array[i][0] += d->array[l][k] * v->array[j][0];
             }
             if (offset[l] == 0) {
                 i = k;
                 j = k;
                 result->array[i][0] += d->array[l][k] * v->array[j][0];
             }
             if (offset[l] < 0) {
                 i = k;
                 j = k - offset[l];

                 result->array[i][0] += d->array[l][k] * v->array[j][0];
             }
         }
     }

     return result;
 }

Matrix* diagonal::diaStore2(Matrix *m, int a)
{

    Matrix *dm = new Matrix(m->rowNo, (dNum/2)+1, " turn a diagonal matrix to a dense matrix:");
    int c=0;
  for (int i=0; i< m->rowNo; i++)
  {
      for(int j=0; j<=i; j++)
      {
          if(m->array[i][j] !=0)
          {
              dm->array[i][c] = m->array[i][j];
              c++;
          }
      }
      c=0;
  }
    return dm;
}

Matrix* diagonal::diaMult2(Matrix *d, Matrix *v)
{
    Matrix *result = new Matrix(row, 1, " vector:");
   // int k=1;
    int d1 = dNum/2;

     for(int i=0; i<row; i++)
     {
         if(i<=d1)
         {
             for(int j=0; j<=i ; j++)
             {
                 result->array[i][0] += (d->array[i][j]*v->array[j][0]);
             }
         }
         if(i>d1)
         {
             for (int j = i; j >= j-d1; j--)
             {
                 result->array[i][0] += (d->array[i][d1]*v->array[j][0]);
                 d1--;
             }
             d1=dNum/2;
         }
     }

    return result;
}
