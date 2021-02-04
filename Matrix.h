//
// Created by Leila on 2/2/2021.
//
#include <string>

using namespace std;
#ifndef AXB_MATRIX_H
#define AXB_MATRIX_H


class Matrix
{
    int rowNo;
    int colNo;
    std::string name;
    double **array;

public:
    double getArray(int r, int c);

    Matrix(int row, int col, string str);

    ~Matrix();

public:
    void setArray(int r, int c, double val);
public:
    int getNonzero();
public:
    int getRow();
public:
    int getCol();

    void Random();

    void Random(int z);

    void print();

    Matrix* add(Matrix *m);

    Matrix* averageRow();

    Matrix* averageCol();

    Matrix* mult(Matrix *m);

    void LowerTriangular();

};


#endif //AXB_MATRIX_H
