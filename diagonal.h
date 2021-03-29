//
// Created by Leila on 3/5/2021.
//

#ifndef AXB_DIAGONAL_H
#define AXB_DIAGONAL_H
#include "Matrix.h"

    class diagonal
    {
        int dNum= 0 ;
        int row=0;
        int col=0;
        int *offset;


    public:

        Matrix* diaStore(Matrix *m, int a);
        Matrix* diaMult(Matrix *d, Matrix *v);
        Matrix* diaStore2(Matrix *m, int a);
        Matrix* diaMult2(Matrix *d, Matrix *v);
        int * offsetCompute(int a);
        diagonal(int r, int c, int d);
        diagonal(int d);
        ~diagonal();

    };


#endif //AXB_DIAGONAL_H





