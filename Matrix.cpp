//
// Created by Leila on 2/2/2021.
//

#include "Matrix.h"
#include <iostream>
Matrix::Matrix(int row, int col, string str)
{
    rowNo = row;
    colNo = col;
    name = str;
    array = new double*[rowNo]();
    for (int i = 0; i < rowNo; ++i)
    {
        array[i] = new double[colNo]();
    }
}
Matrix::~Matrix()
{
    for(int i = 0; i < rowNo; ++i)
    {
        delete [] array[i];
    }
    delete [] array;
}

double Matrix::getArray(int r, int c)
{
    return array[r][c];
}
void Matrix::setArray(int r, int c, double val)
{
    array[r][c] = val;
}
int Matrix::getNonzero()
{
    int zero = 0;
    for (int i = 0; i < rowNo; i++)
    {
        for (int j = 0; j < colNo; j++)
        {
            if (array[i][j] != 0)
            {
                zero++;
            }
        }
    }
    return zero;
}
int Matrix::getRow()
{
    return rowNo;
}
int Matrix::getCol()
{
    return colNo;
}
void Matrix::Random()
{

    for (int i = 0; i < rowNo; i++)
    {
        for (int j = 0; j < colNo; j++)
        {
            array[i][j] = (rand() % 10) + 1;//+1????

        }
    }
}
void Matrix::One()
{

    for (int i = 0; i < rowNo; i++)
    {
        for (int j = 0; j < colNo; j++)
        {
            array[i][j] = 1;

        }
    }
}

void Matrix::Random(int z)
{
    int count = 0;
    int row;
    int col;

    do
    {
        row = rand()%rowNo;
        col = rand()%colNo;

        if (array[row][col] == 0)
        {
            array[row][col] = rand() % 10+1;
            count++;
        }
    } while (count<z);
}
 /*void Matrix::RandomLow()
{
    Matrix *l = new Matrix(rowNo,colNo,"a lower triangular sparse matrix:");
    int row;
    int col;
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            if(i >= j)
            {
                if(array[i][j] == 0)
                {
                    array[row][col] = rand() % 10+1;
                }
            }
        }
    }

}*/

void Matrix::print()
{

    std::cout << name << "\n";

    for (int i = 0; i < rowNo; i++)
    {
        for (int j = 0; j < colNo; j++)
        {
            std::cout << array[i][j]<< " "<<" " ;
        }
        std::cout << "\n";
    }

}

Matrix* Matrix::add(Matrix *m)
{
    if (m->rowNo == rowNo & m->colNo == colNo)
    {
        Matrix *s = new Matrix (rowNo, colNo, "sum of mtx1 and mtx2:");

        for (int i = 0; i < rowNo; i++)
        {
            for (int j = 0; j < colNo; j++)

            {
                s->array[i][j] = m->array[i][j] + array[i][j];
            }
        }
        return s;
    }

    else
    {
        return m;
    }
}
Matrix* Matrix::averageRow()
{
    Matrix *rowsum = new Matrix(rowNo,1, "RowAve");

    for (int i = 0; i < rowNo; i++)
    {
        for (int j = 0; j < colNo; j++)
        {

            rowsum->array[i][0] =  rowsum->array[i][0] + array[i][j] ;
        }
    }

    for (int i = 0; i<rowNo; i++)
    {
        rowsum->array[i][0] = rowsum->array[i][0] / colNo;
    }
    return rowsum;
}

Matrix* Matrix::averageCol()
{
    Matrix *colsum = new Matrix(1,colNo, "ColAve");

    for (int i = 0; i < colNo; i++)
    {
        for (int j = 0; j < rowNo; j++)
        {

            colsum->array[0][i] = array[j][i] + colsum->array[0][i];
        }
    }

    for (int i = 0; i < colNo; i++)
    {
        colsum->array[0][i] = colsum->array[0][i] / rowNo;
    }
    return colsum;
}

Matrix* Matrix::mult(Matrix *m)
{

    Matrix *n = new Matrix(rowNo, m->colNo, "result of multiply of two matrix:");
    if (colNo == m->rowNo)
    {

        for (int i = 0; i < rowNo; i++)
        {
            for (int j = 0; j < m->colNo; j++)
            {
                for (int k = 0; k < colNo; k++)
                {
                    n->array[i][j]  = array[i][k] * m->array[k][j] + n->array[i][j];
                }
            }
        }

    }

    else
    {
        return 0;

    }
    return n;
}
void Matrix::LowerTriangular()
{
    for (int i = 0; i < rowNo; i++)
    {
        for (int j = 0; j < colNo; j++)
        {
            if (i < j)
            {
                array[i][j] = 0;
            }
        }
    }

}
bool Matrix::isequal(Matrix *a)
{
    if (a->rowNo == rowNo & a->colNo == colNo)
    {
        for (int i = 0; i < rowNo; i++)
        {
            for (int j = 0; j < colNo; j++)
            {
                if (a->array[i][j] - array[i][j] > 0.1)//TODO
                {
                    return false;
                }
            }
        }
    }
    else
    {
        return false;
    }

    return true;
}