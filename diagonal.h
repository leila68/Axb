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

        Matrix* turnTo(Matrix *d);
        Matrix* multiply(Matrix *d);

        diagonal(int r, int c, int d);
    };


#endif //AXB_DIAGONAL_H





