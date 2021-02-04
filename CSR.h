//
// Created by Leila on 2/4/2021.
//

#ifndef AXB_CSR_H
#define AXB_CSR_H
#include "../Matrix.h"



class CSR {
    int row = 0;
    int col = 0;
    double *val;
    int *ptr;
    int *idx;
    int nonzero;
    CSR(int r, int c, int n);

    void printArray(double *arr);

    void initializeWithMatirx1(Matrix *m);//Private

public:
    Matrix* turnToRegular();

    CSR(Matrix *m);

    void triplet();

    Matrix* csrMult();

    ~CSR();
};


#endif //AXB_CSR_H
