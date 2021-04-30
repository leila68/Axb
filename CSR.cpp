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

    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> elapsed_seconds;
    start = std::chrono::system_clock::now();
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
        ptr[i+1] = ptr[i]+nzRow[i];
    }

    end = std::chrono::system_clock::now();
    elapsed_seconds = end - start;
    double durationSym = elapsed_seconds.count();
    cout << "execution time (initialize CSR):" << durationSym << "\n";

    delete []nzRow;
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

void  CSR::turntoCSR()
{
    int *rowCnt= new int [row]();
    int *ptrR = new int[row+1]();
    int counter = 0;
    int *colIdx = new int[nonzero]();
    double *valR = new double [nonzero]();

    //
  //  int *idxR = new int[nonzero]();
  //
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> elapsed_seconds;
    start = std::chrono::system_clock::now();
    for(int i=0; i<nonzero; i++)
    {
        rowCnt[idx[i]]++;
    }
    for(int i=0; i<row; i++)
    {
        ptrR[i] = counter;
        counter += rowCnt[i];
    }
    ptrR[row] = nonzero;
    memset(rowCnt, 0, sizeof(int) * row);//makes zero all elements of rowCtn
    for(int i=0; i<col; i++)
    {
        for(int j= ptr[i]; j<ptr[i+1]; j++)
        {
           int r = idx[j];
           int index = rowCnt[r]+ptrR[r];
           colIdx[index] = i;
           valR[index] = val[j];
           rowCnt[r]++;
        }
    }
    end = std::chrono::system_clock::now();
    elapsed_seconds = end - start;
    double durationSym1 = elapsed_seconds.count();
   // cout << "execution time (turn to CSR2):" << durationSym1 << "\n";

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
        idx[i] = colIdx[i];
    }

    delete []rowCnt;
    delete []colIdx;
    delete []ptrR;
    delete []valR;
}

int  CSR::turntoCSC()
{
     turntoCSR();
    return nonzero;
}