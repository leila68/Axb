//
// Created by Leila on 2/4/2021.
//

#ifndef AXB_CSR_H
#define AXB_CSR_H
#include "Matrix.h"



class CSR {

    int row = 0;
    int col = 0;
    int nonzero = 0;
public:
    double *val;
    int *ptr;
    int *idx;

    CSR(int r, int c, int n);

    void printArray(double *arr);

    void initializeWithMatirx1(Matrix *m);//Private

public:
    Matrix* turnToRegular();

    CSR(Matrix *m);

    CSR(int r, int c, int nz, double *v, int *p, int *ind, double *x);

    void triplet();

    Matrix* csrMult();

    Matrix* csrMult(Matrix *v);

    ~CSR();

    CSR(int r, int c, int nz, int *p, int *ind, double *x);
};

#endif //AXB_CSR_H