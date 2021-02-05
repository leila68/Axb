//
// Created by Leila on 2/4/2021.
//

#include "CSR.h"
#include <iostream>

CSR::CSR(int r, int c, int n)
{
    row = r;
    col = c;
    nonzero = n;
    ptr = new int[r+1]();
    idx = new int[n]();
    val = new double[n]();

}
CSR::CSR(Matrix *m)
{
initializeWithMatirx1(m);
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
            std::cout << i;
            std::cout << idx[j];
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
    row = m->getRow();
    col = m->getCol();
    nonzero = m->getNonzero();
    ptr = new int[row + 1]();
    idx = new int[nonzero]();
    val = new double[nonzero]();
    int *nzRow = new int[row];
    ptr[0] = 0;
    int *first = new int[row];

    for (int i = 0; i < row; i++)
    {
        s = 0;

        for (int j = 0; j < col; j++)
        {
            if (m->getArray(i, j) != 0)
            {
                val[c] = m->getArray(i, j);
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

    Matrix *y = new Matrix(row, col, "turn");

    for (int i = 0; i < row; i++)
    {
        for (int j = ptr[i]; j < ptr[i + 1]; j++)
        {
            y->setArray(i, idx[j], val[j]);

        }
    }

    return y;
}
Matrix* CSR::csrMult()
{
    Matrix *result = new Matrix(row, 1, "resultVector");
    Matrix *v = new Matrix(row, 1, "vector");
    v->Random();
    v->print();
    double s = 0;
    for (int i=0; i<row; i++)
    {
        for (int j=ptr[i]; j<ptr[i+1]; j++ )
        {
            s = val[j] * v->getArray(idx[j],0) + s;
            result->setArray(i, 0, s);

        }
        s = 0;
    }
    return result;
}