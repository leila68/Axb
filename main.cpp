#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include "cmake-build-debug/TriangularSolve.h"
#include "cmake-build-debug/CSR.h"


//#include "stdafx.h"
using namespace std;

/*struct Matrix
{
    int rowNo;
    int colNo;
    string name;
    double **array;

     Matrix(int row, int col, string str)
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
    ~Matrix()
    {
        for(int i = 0; i < rowNo; ++i)
        {
            delete [] array[i];
        }
        delete [] array;
    }
    double getArray(int r, int c)
    {
        return array[r][c];
    }
    void setArray(int r, int c, double val)
    {
        array[r][c] = val;
    }
    int getNonzero()
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
    int getRow()
    {
        return rowNo;
    }
    int getCol()
    {
        return colNo;
    }
    void Random()
    {

        for (int i = 0; i < rowNo; i++)
        {
            for (int j = 0; j < colNo; j++)
            {
                array[i][j] = (rand() % 10) + 1;//+1????

            }
        }
    }
    void Random(int z)
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

    void print()
    {

        cout << name << "\n";

        for (int i = 0; i < rowNo; i++)
        {
            for (int j = 0; j < colNo; j++)
            {
                cout << array[i][j]<< " " ;
            }
            cout << "\n";
        }

    }

    Matrix* add(Matrix *m)
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
    Matrix* averageRow()
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

    Matrix* averageCol()
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

    Matrix* mult(Matrix *m)
    {

        Matrix *n = new Matrix(rowNo, m->colNo, "mtx1 * mtx2");
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

    void LowerTriangular()
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

};*/

/*struct TriangularSolve
{

    int rowNo;
    TriangularSolve(int n)
    {
        rowNo = n;

    }
    ~TriangularSolve()
    {

    }
    Matrix* solve(Matrix *L, Matrix *d)
    {
        Matrix *y = new Matrix(rowNo, 1, "y");
        double s = 0;
        y->setArray(0, 0, d->getArray(0, 0)/L->getArray(0,0))  ;

        for (int i=1; i<rowNo;i++ )
        {
            for (int j = 0; j < i ; j++)
            {
                s = L->getArray(i, j) * y->getArray(j, 0) + s;
            }
            y->setArray(i, 0, (d->getArray(i, 0) - s));
            y->setArray(i, 0, y->getArray(i, 0) / L->getArray(i, i));
            s = 0;

        }

        return y;
    }
};*/

/*struct CSR
{
    int row = 0;
    int col = 0;
    double *val;
    int *ptr;
    int *idx;
    int nonzero;
    CSR(int r, int c, int n)
    {
        row = r;
        col = c;
        nonzero = n;
        ptr = new int[r+1]();
        idx = new int[n]();
        val = new double[n]();

    }
    CSR(Matrix *m)
    {
        //initializeWithMatirx1(m);
    }
    ~CSR()
    {

    }
    void triplet()
    {
        for(int i=0; i<row; i++)
        {
            for (int j = ptr[i]; j < ptr[i + 1]; j++)
            {
                cout << i;
                cout << idx[j];
                cout << val[j];
            }
        }
    }
    void printArray(double *arr)
    {
        for (int i = 0; i < sizeof (arr); i++)
        {
            cout <<  arr[i] << "\t";
            cout << "\n";
        }
    }
    void initializeWithMatirx1(Matrix *m)//Private
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
    Matrix* turnToRegular()
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
    Matrix* csrMult()
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
};*/

int main()
{
    srand((unsigned)time(NULL));
    Matrix *mtx1 = new Matrix(4, 4, "Matrix1");
    mtx1->Random();
    mtx1->print();
    Matrix *mtx2 = new Matrix(4, 4, "Matrix2");
    mtx2->Random();
    mtx2->print();

    Matrix *mtx3 = mtx2->add(mtx1);
    mtx3->print();
    Matrix *ave1 = mtx3->averageRow();
    ave1->print();
    Matrix *ave2 = mtx3->averageCol();
    ave2->print();

    Matrix *ml = mtx1->mult(mtx2);
    ml->print();

    //triangular solve
    Matrix *mtxL = new Matrix(5,5,"Lower Triangular");
    mtxL->Random();
    mtxL->LowerTriangular();
    mtxL->print();
    Matrix *rhs = new Matrix(5, 1, "rhs");
    rhs->Random();
    rhs->print();
    TriangularSolve *ts1 = new TriangularSolve(5);
    Matrix *y = ts1->solve(mtxL,rhs);
    y->print();

    //CSR
    Matrix *spr = new Matrix(3,3, "Sparse Matrix");
    spr->Random(3);
    spr->print();
    CSR *c1 = new CSR(spr);
    c1->triplet();

    Matrix *reg =  c1->turnToRegular();
    reg->print();

    Matrix *vector = c1->csrMult();
    vector->print();

    delete mtx1;
    delete mtx2;
    delete mtx3;
    delete ave1;
    delete ave2;
    delete ml;
    delete mtxL;
    delete rhs;
    delete spr;
    delete c1;
    delete reg;
    delete vector;
    return 0;
}
