//
// Created by Leila on 2/17/2021.
//

#include <iostream>
#include "CSC.h"
#include <chrono>

CSC::CSC(int r, int c, int nnz)
{
    row = r;
    col = c;
    nonzero = nnz;
    ptr = new int[col+1]();
    idx = new int[nonzero]();
    val = new double[nonzero]();

}
CSC::CSC(int r, int c, int nnz, int *p, int *d, double *v)
{
    row = r;
    col = c;
    nonzero = nnz;
    ptr = new int[col+1]();
    idx = new int[nonzero]();
    val = new double [nonzero]();

    for (int i = 0; i < col+1; ++i)
    {
      ptr[i] = p[i];
    }
    for (int i = 0; i < nonzero; ++i)
    {
        idx[i] = d[i];
    }
    for (int i = 0; i < nonzero; ++i)
    {
        val[i] = v[i];
    }

 }
CSC::CSC(Matrix *m)
{
  initializeWithMatirx(m);
}
CSC::~CSC()
{
    delete []ptr;
    delete []idx;
    delete []val;

}
void CSC::Triplet()
{
    cout<<"CSC:"<<"\n";
   for(int i=0; i<col; i++)
   {
       for(int j=ptr[i]; j<ptr[i+1]; j++)
       {
           std::cout << idx[j] << "\t"; // row
           std::cout << i << "\t";     //column
           std::cout << val[j] << "\n"; //value
       }
   }
}

Matrix* CSC::turnToRegular()
{
    Matrix *y = new Matrix(row, col, "turn to regular:");

    for (int i = 0; i < col; i++)
    {
        for (int j = ptr[i]; j < ptr[i+1]; j++)
        {
           // y->setArray( idx[j], i, val[j]);
            y->array[idx[j]][i] = val[j];

        }
    }
    return y;
}

void CSC::initializeWithMatirx(Matrix *m)
{
    int c = 0;
    int s;
    row = m->rowNo;
    col = m->colNo;
    nonzero = m->getNonzero();

    ptr = new int[col + 1]();
    idx = new int[nonzero]();
    val = new double[nonzero]();
    nzcol = new int[col];

    ptr[0] = 0;
    for (int i = 0; i < col; i++)
    {
        s = 0;
        for (int j = 0; j < row; j++)
        {
            if (m->array[j][i] != 0)
            {
                val[c] = m->array[j][i];
                idx[c] = j;
                c++;
                s++;
            }
        }
        nzcol[i] = s;
    }
    for (int i = 1; i <= col; i++)
    {
        ptr[i] = ptr[i - 1] + nzcol[i - 1];
    }
    delete []nzcol;
}

Matrix* CSC::cscMult(Matrix *v)
{
    Matrix *result = new Matrix(row, 1, "CSC * vector :");

         for (int i=0; i<col; i++ )
           {
               for (int j=ptr[i]; j<ptr[i+1]; j++)
               {
                    result->array[idx[j]][0] += val[j]*v->array[i][0];

               }
           }

    return result;
}