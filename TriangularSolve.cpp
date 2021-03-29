//
// Created by Leila on 2/4/2021.
//

#include "TriangularSolve.h"
#include "Matrix.h"
#include "diagonal.h"
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
    Matrix *y = new Matrix(rowNo, 1, "y(Unknowns):");
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
    Matrix *y = new Matrix(rowNo, 1, "y(Unknowns):");
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
    Matrix *y = new Matrix(rowNo, 1, "y(Unknowns):");

    for(int i=0; i<rowNo; i++)
    {
        y->array[i][0] = d->array[i][0];
    }

    for (int i=0; i<rowNo; i++)
      {
            y->array[i][0] /=  L->val[L->ptr[i]];
            for (int j=L->ptr[i]+1; j<L->ptr[i+1]; j++ )
            {
                y->array[L->idx[j]][0] -=  L->val[j]*y->array[i][0];
            }
      }

    return y;
}

Matrix* TriangularSolve::solve(Matrix *L, Matrix *d, int *offset, int dia )
{
   // int i,j;
    int k;
    double s = 0;
    int l=0;
    int n = dia/2 + 1;


    Matrix *r = new Matrix(rowNo,rowNo, "row index:");
    Matrix *y = new Matrix(rowNo, 1, "y(Unknowns):");

    for(int i=0; i<rowNo; i++)
    {
        y->array[i][0] = d->array[i][0];
    }

    for(int i=0; i<n; i++)
    {
        k=i;
        for(int j=0; j< rowNo-l; j++)
        {
                r->array[i][j] = k;
                k++;
        }
        l++;
    }

    y->array[0][0] = d->array[0][0] / L->array[0][0];
    for(int i=1; i<rowNo; i++)
    {
        l=1;
        for(int j=1; j< n; j++)
        {
            for(int k=0; k<rowNo-l; k++)
            {
                if (r->array[j][k] == i)
                {
                    s += L->array[j][k] * y->array[k][0];

                }
            }
        l++;
        }
        y->array[i][0] = (d->array[i][0] - s) / L->array[0][i];
        s = 0;
    }

    return y;
}

Matrix* TriangularSolve::solve(Matrix *L, Matrix *d, int dia)
{
    Matrix *y = new Matrix(rowNo, 1, "y(Unknowns):");
    double s = 0;
    int c=0;
    int d1 = dia/2;
//    y->array[0][0] = d->array[0][0] / L->array[0][0];

    for(int i=0; i<rowNo; i++)
    {
        if(i<=d1)
        {
            for(int j=0; j<i ; j++)
            {
               s += L->array[i][j]*y->array[j][0];
            }
          y->array[i][0] = (d->array[i][0] - s) / L->array[i][i];

        }

         if(i>d1)
        {
            for (int j = i-1; j > j-d1; j--)
            {
                d1--;
                s += L->array[i][d1]*y->array[j][0];
                //d1--;
            }
          d1=dia/2;
          y->array[i][0] = (d->array[i][0] - s) / L->array[i][d1];
        }

        s = 0;
    }
    return y;
}