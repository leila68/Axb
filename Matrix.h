//
// Created by Leila on 2/2/2021.
//



#ifndef AXB_MATRIX_H
#define AXB_MATRIX_H
#include <string>
using namespace std;

class Matrix
{
    friend class CSR;
    friend class  TriangularSolve;
    friend class  CSC;
    friend class diagonal;
    int rowNo;
    int colNo;
    std::string name;
    double **array;

public:
    double getArray(int r, int c);

    Matrix(int row, int col, string str);

    ~Matrix();

    void setArray(int r, int c, double val);

    int getNonzero();

    int getRow();

    int getCol();

    void Random();

    void One();

    void Random(int z);

    void Random(int z, string s);
    void Random1(int z, string s);

   // void Matrix::RandomLow();

    void print();

    Matrix* add(Matrix *m);

    Matrix* addTest();

    Matrix* averageRow();

    Matrix* averageCol();

    Matrix* mult(Matrix *m);

    void LowerTriangular();

    bool Matrix::isequal(Matrix *a);


};


#endif //AXB_MATRIX_H