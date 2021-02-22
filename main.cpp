
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include "TriangularSolve.h"
#include "CSR.h"
#include "math.h"
#include  "sympiler/smp-format/io.h"
#include "CSC.h"
//#include "stdafx.h"
using namespace std;

void addMultTest();
void averageTest();
void triangularTest();
void  csrTest();
void  csrFileTest();
void  cscFileTest();


int main()
{

    srand((unsigned)time(NULL));

    //addMultTest();
    // averageTest();
    // triangularTest();
    // csrTest();
    // csrFileTest();
     cscFileTest();

    return 0;
}

void addMultTest()
{
    //Print matrix(dense)1
    Matrix *mtx1 = new Matrix(4, 4, "Matrix1:");
    mtx1->Random();
    mtx1->print();

   //Print matrix(dense)2
    Matrix *mtx2 = new Matrix(4, 4, "Matrix2:");
    mtx2->Random();
    mtx2->print();
   //matrix1 + matrix2
    Matrix *mtx3 = mtx2->add(mtx1);
    mtx3->print();
   //matrix1*matrix2
    Matrix *ml = mtx1->mult(mtx2);
    ml->print();

    delete mtx1;
    delete mtx2;
    delete ml;
}

void averageTest()
{
    Matrix *mtx3 = new Matrix(4,4,"Matrix3");//???
    mtx3->Random();
    mtx3->print();
    //row average, matrix3
    Matrix *ave1 = mtx3->averageRow();
    ave1->print();
    //col average, matrix3
    Matrix *ave2 = mtx3->averageCol();
    ave2->print();
    delete ave1;
    delete ave2;
    delete mtx3;
}

void triangularTest()
{
    //triangular solve
    //print lower triangular matrix
    Matrix *mtxL = new Matrix(5,5,"Lower Triangular Matrix:");
    mtxL->Random();
    mtxL->LowerTriangular();
    mtxL->print();
    //print right hand side
    Matrix *rhs = new Matrix(5, 1, "rhs Matrix:");
    rhs->Random();
    rhs->print();

    //solve the equation(mtxl*y=rhs)
    TriangularSolve *ts1 = new TriangularSolve(5);
    Matrix *y = ts1->solve(mtxL,rhs);
    y->print();

    //triangularSolve,Test the answer
    Matrix *q = mtxL->mult(y);
    cout<<"mtxl * y:" <<"\n";
    q->print();
    bool r = q->isequal(rhs);
    cout<<"the result of Triangular solve is: "<<r <<"\n";
    delete mtxL;
    delete rhs;
    delete q;
}

void  csrTest()
{
    Matrix *mtxL = new Matrix(5,5,"Lower Triangular");
    mtxL->Random();
    mtxL->LowerTriangular();

    Matrix *rhs = new Matrix(5, 1, "rhs");
    rhs->Random();

    //CSR print & mult
    Matrix *spr = new Matrix(5,5, "Sparse Matrix:");
    spr->Random(5);
    spr->print();
    //save matrix in csr format
    CSR *c1 = new CSR(spr);
    c1->triplet();
    Matrix *reg =  c1->turnToRegular();
    reg->print();

    Matrix *vector = c1->csrMult();
    vector->print();

    //csrSolve
    mtxL->print();
    rhs->print();
    CSR *c2 = new CSR(mtxL);
    TriangularSolve *rs = new TriangularSolve(5);
    Matrix *result = rs->solve(c2,rhs);
    result->print();

    //csr Solve,Test the equation(c2*y=rhs)
    Matrix *rs3 = c2->csrMult(result);
    rs3->print();
    bool r2 = rs3->isequal(rhs);
    cout<<"the result of Triangular solve is: "<<r2 <<"\n";

    delete mtxL;
    delete rhs;
    delete spr;
    delete c1;
    delete reg;
    delete vector;
    delete c2;
    delete rs;
    delete rs3;
    delete result;
}

void  csrFileTest()
{
    //sparse matrix file(csc)
    std::string in_path_ = "D:\\github\\Axb\\sympiler\\LFAT5.mtx";
    // std::string in_path_ = "D:\\github\\Axb\\sympiler\\Trefethen_20b.mtx";
    std::ifstream fin(in_path_);
    format::CSC *H;
    read_mtx_csc_real(fin, H, true);

    //print of csc file
    CSR *ccf = new CSR(H->n, H->m, H->nnz, H->p, H->i, H->x);
    ccf->triplet();

    //turn to csr
    cout<<"CSR:"<<"\n";
    ccf->turntoCSR();
    ccf->triplet();

    //csc file Solve
    Matrix *rhcr = new Matrix(H->n,1,"rhcr:");
    rhcr->One();
    // rhcr->print();
    TriangularSolve *rs2 = new TriangularSolve(H->n);
    Matrix *result2 = rs2->solve( ccf, rhcr);
    result2->print();

    //csr Solve,Test the equation(ccf*y=rhcr)
    Matrix *rs3 = ccf->csrMult(result2);
    rs3->print();
    bool r2 = rs3->isequal(rhcr);
    cout<<"the result of Triangular solve is: "<<r2 <<"\n";

    //multiply of csc(file) and vector(file)
    //Matrix *mf = cf->turnToRegular();
    // mf->print();
    //Matrix *vf = cf->csrMult();
    //  vf->print();

    delete H;
    delete ccf;
    delete rhcr;
    delete rs2;
    delete result2;
    delete rs3;
    delete ccf;

}

void  cscFileTest()
{
    //sparse matrix file(csc)
    std::string in_path_ = "D:\\github\\Axb\\sympiler\\LFAT5.mtx";
    // std::string in_path_ = "D:\\github\\Axb\\sympiler\\Trefethen_20b.mtx";
    std::ifstream fin(in_path_);
    format::CSC *H;
    read_mtx_csc_real(fin, H, true);

    //CSC
    //csc print file

    CSC *cc = new CSC(H->n, H->m, H->nnz, H->p, H->i, H->x);
    Matrix *mcc = new Matrix(H->n,1," vector: ");
    mcc->One();
    mcc->print();
    cc->Triplet();
    Matrix *ccr = cc->cscMult(mcc);
    ccr->print();

    //csc Solve
    TriangularSolve *ccs = new TriangularSolve(H->n);
    Matrix *result3 = ccs->solve(cc,mcc);
    cout<<"csc (solve):"<<"\n";
    result3->print();



    delete H;
    delete cc;
    delete mcc;
    delete ccr;
    delete ccs;
    delete result3;
}