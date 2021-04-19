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
    ptr = new int[row+1]();
    idx = new int[nonzero]();
    val = new double[nonzero]();

    for(int i=0; i<row+1; i++)
    {
        ptr[i] = p[i];
    }
    for(int i=0; i<nonzero; i++)
    {
        idx[i] = ind[i];
    }
    for(int i=0; i<nonzero; i++)
    {
        val[i] = x[i];
    }

}
CSR::~CSR()
{
    delete []ptr ;
    delete []idx ;
    delete []val;
    delete []nzRow;
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
    nzRow = new int[row];
    ptr[0] = 0;

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
    delete v;
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
    int *rowNum = new int [row+1]();
    int *ptrR = new int[row+1]();
    int *idxR = new int[nonzero]();
    double *valR = new double [nonzero]();

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
    rowNum[col] = k+1;
    ptrR[0]=0;
    for (int i = 1; i < row+1; i++)
    {
        ptrR[i] = ptrR[i-1]+rowNum[i-1];
    }

    //ptr = ptrR;
   // val = valR;
   // idx = idxR;
    for(int i=0; i<row+1; i++)
    {
        ptr[i] = ptrR[i];
    }
    for(int i=0; i<nonzero; i++)
    {
        val[i] = valR[i];
    }
    for(int i=0; i<nonzero; i++)
    {
        idx[i] = idxR[i];
    }
    delete []colIdx;
    delete []rowNum;
}