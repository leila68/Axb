//
// Created by Leila on 2/17/2021.
//

#ifndef AXB_CSC_H
#define AXB_CSC_H
#include "Matrix.h"

class CSC
{
  friend class Matrix;
  int row = 0;
  int col = 0;
  int nonzero = 0;


    CSC(int r, int c, int nnz);

    CSC(Matrix *m);

public:
    CSC(int r, int c, int nnz, int *p, int *idx, double *val);

    void Triplet();

    Matrix* turnToRegular();

    void initializeWithMatirx1(Matrix *m);

    Matrix* cscMult(Matrix *v);

    double *val;
    int *ptr;
    int *idx;
};


#endif //AXB_CSC_H
