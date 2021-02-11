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
    Matrix *y = new Matrix(rowNo, 1, "y");
    double s = 0;
    y->setArray(0, 0, d->getArray(0, 0)/L->getArray(0,0))  ;

    for (int i=1; i<rowNo;i++ )
    {
        for (int j = 0; j < i ; j++)
        {
            s = L->getArray(i, j) * y->getArray(j, 0) + s;
        }
        y->setArray(i, 0, (d->getArray(i, 0) - s));
        y->setArray(i, 0, y->getArray(i, 0) / L->getArray(i, i));
        s = 0;

    }

    return y;
}
Matrix* TriangularSolve::solve(CSR *L, Matrix *d)
{
    Matrix *y = new Matrix(rowNo, 1, "y");
    double s = 0;


    y->setArray(0, 0, d->getArray(0, 0)/ L-> val[0])  ;
    for (int i=1; i<rowNo; i++)
    {
        for (int j=L->ptr[i]; j<L->ptr[i+1]-1; j++ )
        {
            s = L->val[j]* y->getArray(L->idx[j],0) + s;

        }
        y->setArray(i, 0, (d->getArray(i, 0) - s));
        y->setArray(i, 0, y->getArray(i, 0) / L->val[L->ptr[i]+i]);//is diagonal
        s = 0;
    }
    return  y;
}