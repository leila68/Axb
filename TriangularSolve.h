//
// Created by Leila on 2/4/2021.
//

#ifndef AXB_TRIANGULARSOLVE_H
#define AXB_TRIANGULARSOLVE_H
#include "../Matrix.h"

class TriangularSolve
{
    int rowNo;

    ~TriangularSolve();

public:
    TriangularSolve(int n);

    Matrix* solve(Matrix *L, Matrix *d);
};


#endif //AXB_TRIANGULARSOLVE_H
