//
// Created by Leila on 3/5/2021.
//

#include "diagonal.h"
#include <iostream>

diagonal::diagonal(int r, int c, int d)
{
    row = r;
    col = c;
    dNum = d;
}

Matrix* diagonal::turnTo(Matrix *d)
{

    offset = new int[dNum]();
    Matrix *dd = new Matrix(dNum,d->colNo," turn a diagonal matrix to a dense matrix:");
    //int j=1;
    /*for (int i=0; i<d->rowNo; i++)
    {
            dd->array[1][i] = d->array[i][i];
            if(i+1<d->rowNo)
            {
                dd->array[0][i] = d->array[i][i+1];
                dd->array[2][i] = d->array[i+1][i];

            }

    }*/

    for (int i=1; i<=dNum/2; i++)
    {
        for (int j = 0; j < d->colNo; j++) {
            dd->array[(dNum/2)][j] = d->array[j][j];

            if (j + 1 < d->rowNo) {
                dd->array[(dNum/2) - i][j] = d->array[j][j + 1];

                dd->array[(dNum/2) + i][j] = d->array[j + 1][j];
            }

        }
        offset[dNum/2]=0;
        offset[(dNum/2) - i]=-i;
        offset[(dNum/2) + i]=i;
        /* for (int j = 0; j < 3; ++j)
         {
            cout<<offset[j];

         }*/
    }
    return dd;
}

Matrix* diagonal::multiply(Matrix *dd)
{
    int  i,j;

    //  int offset[] = {-1,0,1};
    Matrix *v = new Matrix(row,1," vector:");
    v->One();
    v->print();
    Matrix *result = new Matrix(row,1," vector:");


    for(int l=0; l<dd->rowNo; l++)
    {

        for(int k=0; k < dd->colNo - abs(offset[l]) ; k++)
        {
            if(offset[l]>0)
            {
                i = k + offset[l];
                j = k;

                result->array[i][0] += dd->array[l][k] * v->array[j][0];
            }
            if(offset[l]==0)
            {
                i = k ;
                j = k ;
                result->array[i][0] += dd->array[l][k] * v->array[j][0];
            }
            if(offset[l]<0)
            {
                i = k;
                j = k-offset[l];

                result->array[i][0] += dd->array[l][k] * v->array[j][0];
            }
        }
    }
    return  result;
}