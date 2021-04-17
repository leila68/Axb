//
// Created by Leila on 2/4/2021.
//

#include "CSR.h"
#include <iostream>
#include <chrono>

CSR::CSR(int r, int c, int n)
{
    row = r;
    col = c;
    nonzero = n;
    ptr = new int[row+1]();
    idx = new int[nonzero]();
    val = new double[nonzero]();

}
CSR::CSR(Matrix *m)
{
    initializeWithMatirx1(m);
}
CSR::CSR(int r, int c, int nz, int *p, int *ind, double *x)
{
    row = r;
    col = c;
    nonzero = nz;
    ptr = new int[r+1]();
    idx = new int[nz]();
    val = new double[nz]();
    ptr = p;
    idx = ind;
    val=x;

}
CSR::~CSR()
{

}
void CSR::triplet()
{
    for(int i=0; i<row; i++)
    {
        for (int j = ptr[i]; j < ptr[i + 1]; j++)
        {
            std::cout << i<<"\t";
            std::cout << idx[j]<<"\t";
            std::cout << val[j]<<"\n";
        }
    }
}

void CSR::printArray(double *arr)
{
    for (int i = 0; i < sizeof (arr); i++)
    {
        std::cout <<  arr[i] << "\t";
        std::cout << "\n";
    }
}

void CSR::initializeWithMatirx1(Matrix *m)//Private
{
    int c = 0;
    int s;
    row = m->rowNo;
    col = m->colNo;
    nonzero = m->getNonzero();

    ptr = new int[row + 1]();
    idx = new int[nonzero]();
    val = new double[nonzero]();
    int *nzRow = new int[row];
    ptr[0] = 0;
   // int *first = new int[row];

    for (int i = 0; i < row; i++)
    {
        s = 0;

        for (int j = 0; j < col; j++)
        {
            if (m->array[i][j] != 0)
            {
                val[c] = m->array[i][j];
                idx[c] = j;
                c++;
                s++;
            }
        }
        nzRow[i] = s;
    }
    for (int i = 1; i <= row; i++)
    {
        ptr[i] = ptr[i - 1] + nzRow[i - 1];
    }
}
Matrix* CSR::turnToRegular()
{

    Matrix *y = new Matrix(row, col, "turn to regular:");

    for (int i = 0; i < row; i++)
    {
        for (int j = ptr[i]; j < ptr[i + 1]; j++)
        {

            y->array[i][idx[j]] = val[j];

        }
    }
    return y;
}

Matrix* CSR::csrMult()
{
    Matrix *result = new Matrix(row, 1, "result of sparse matrix(csr) * Vector:");
    Matrix *v = new Matrix(row, 1, "vector:");
    v->Random();
    v->print();
    double s = 0;

    for (int i=0; i<row; i++)
    {
        for (int j=ptr[i]; j<ptr[i+1]; j++ )
        {

            s = val[j] * v->array[idx[j]][0] + s;
        }

        result->array[i][0] = s;
        s = 0;
    }

    return result;
}

Matrix* CSR::csrMult(Matrix *v)
{
    Matrix *result = new Matrix(row, 1, "result for testing csrSolve:");
    double s = 0;


    for (int i=0; i<row; i++)
    {
        for (int j=ptr[i]; j<ptr[i+1]; j++ )
        {
            s = val[j] * v->array[idx[j]][0] + s;
        }

       result->array[i][0] = s;
        s = 0;
    }


    return result;
}

void CSR::turntoCSR()
{

    int *colIdx = new int[nonzero];
    int *idxR = new int[nonzero]();
    double *valR = new double [nonzero]();
    int *ptrR = new int[row+1]();
    int *rowNum = new int [row+1]();
    int k = 0;
    int count = 0;
     for(int i=0; i<col; i++)
     {
         for(int j=ptr[i]; j<ptr[i+1]; j++)
         {
           colIdx[j] = i;
         }
     }

    for(int i=0; i<col; i++)
    {
        for(int j=0; j<nonzero; j++)
        {
            if(idx[j] == i)
            {
                idxR[k] = colIdx[j];
                valR[k] = val[j];
                k++;
                count++;
            }
        }
        rowNum[i]=count;
        count = 0;
    }
    rowNum[col+1] = k+1;
    ptrR[0]=0;
    for (int i = 1; i < row+1; i++)
    {
        ptrR[i] = ptrR[i-1]+rowNum[i-1];
    }

    ptr = ptrR;
    val = valR;
    idx = idxR;
}