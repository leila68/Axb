//
// Created by Leila on 2/17/2021.
//

#include <iostream>
#include "CSC.h"
CSC::CSC(int r, int c, int nnz)
{
    row = r;
    col = c;
    nonzero = nnz;
    ptr = new int[col+1]();
    idx = new int[nnz]();
    val = new double[nnz]();

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
            y->setArray( idx[j], i, val[j]);

        }
    }
    return y;
}