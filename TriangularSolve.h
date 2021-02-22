//
// Created by Leila on 2/4/2021.
//

#ifndef AXB_TRIANGULARSOLVE_H
#define AXB_TRIANGULARSOLVE_H
#include "Matrix.h"
#include "CSR.h"
#include "CSC.h"

class TriangularSolve
{
    friend class Matrix;
    friend class CSR;
    int rowNo;

public:
    TriangularSolve(int n);

    Matrix* solve(Matrix *L, Matrix *d);
    Matrix* solve(CSR *L, Matrix *d);
    Matrix* solve(CSC *L, Matrix *d);

    ~TriangularSolve();
};


#endif //AXB_TRIANGULARSOLVE_H