//
// Created by Leila on 2/4/2021.
//

#ifndef AXB_TRIANGULARSOLVE_H
#define AXB_TRIANGULARSOLVE_H
#include "Matrix.h"
#include "CSR.h"

class TriangularSolve
{
    int rowNo;

public:
    TriangularSolve(int n);

    Matrix* solve(Matrix *L, Matrix *d);
    Matrix* solve(CSR *L, Matrix *d);

    ~TriangularSolve();
};


#endif //AXB_TRIANGULARSOLVE_H