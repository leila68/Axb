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
CSC::CSC(int r, int c, int nnz, int *p, int *idx, double *val)
{
    row = r;
    col = c;
    nonzero = nnz;
    ptr = p;
    this->idx = idx;
    this->val = val;

}
CSC::CSC(Matrix *m)
{
  initializeWithMatirx(m);
}
CSC::~CSC()
{
    delete ptr;
    delete idx;
    delete val;
    delete nzcol;
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

    ptr = new int[col + 1](); //TODO des /checked
    idx = new int[nonzero](); // TODO  /checked
    val = new double[nonzero](); // TODO  /checked
    nzcol = new int[col]; //TODO delete this  /checked

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
}

Matrix* CSC::cscMult(Matrix *v)
{
    Matrix *result = new Matrix(row, 1, "CSC * vector :");

         for (int i=0; i<col; i++ )
           {
               for (int j=ptr[i]; j<ptr[i+1]; j++)
               {
                    result->array [idx[j]][0] += val[j]*v->array[i][0] ;

               }
           }

    return result;
}