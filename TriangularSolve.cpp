//
// Created by Leila on 2/4/2021.
//

#include "TriangularSolve.h"
#include "Matrix.h"
#include <iostream>

TriangularSolve::TriangularSolve(int n)
{
    rowNo = n;

}
TriangularSolve::~TriangularSolve()
{

}
Matrix* TriangularSolve::solve(Matrix *L, Matrix *d)
{
    Matrix *y = new Matrix(rowNo, 1, "y(Unknows):");
    double s = 0;
    y->array[0][0] = d->array[0][0] / L->array[0][0];

    for (int i=1; i<rowNo;i++ )
    {
        for (int j = 0; j < i ; j++)
        {
            s = (L->array[i][j] * y->array[j][0]) + s;
        }

       y->array[i][0] = (d->array[i][0] - s) / L->array[i][i];
        s = 0;

    }

    return y;
}
Matrix* TriangularSolve::solve(CSR *L, Matrix *d)
{
    Matrix *y = new Matrix(rowNo, 1, "y(Unknows):");
    double s = 0;

    y->array[0][0] = d->array[0][0] / L->val[0];
    for (int i=1; i<rowNo; i++)
    {
        for (int j=L->ptr[i]; j<L->ptr[i+1]-1; j++ )
        {
            s = (L->val[j] * y->array[L->idx[j]][0]) + s;

        }
        y->array[i][0] = (d->array[i][0] - s) / L->val[L->ptr[i+1]-1];
        s = 0;
    }
    return  y;
}

Matrix* TriangularSolve::solve(CSC *L, Matrix *d)
{
    Matrix *y = new Matrix(rowNo, 1, "y(Unknows):");
    y = d;
    for (int i=0; i<rowNo; i++)
      {
            y->array[i][0] /=  L->val[L->ptr[i]];
            for (int j=L->ptr[i]+1; j<L->ptr[i+1]; j++ )
            {
                y->array[L->idx[j]][0] -=  L->val[j]*y->array[i][0];
            }

      }

    return d;

}