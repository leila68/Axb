
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

int main()
{
    srand((unsigned)time(NULL));
    Matrix *mtx1 = new Matrix(4, 4, "Matrix1:");
    mtx1->Random();
    mtx1->print();
    Matrix *mtx2 = new Matrix(4, 4, "Matrix2:");
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

    //triangularSolve,Test
    Matrix *q = mtxL->mult(y);
    bool r = q->isequal(rhs);
    cout<<"the result of Triangular solve is: "<<r <<"\n";


    //CSR print & mult
    Matrix *spr = new Matrix(3,3, "Sparse Matrix:");
    spr->Random(3);
    spr->print();
    CSR *c1 = new CSR(spr);
    c1->triplet();
    Matrix *reg =  c1->turnToRegular();
    reg->print();
    Matrix *vector = c1->csrMult();
    vector->print();

    //csrSolve
    CSR *c2 = new CSR(mtxL);
    TriangularSolve *rs = new TriangularSolve(5);
    Matrix *result = rs->solve(c2,rhs);
    result->print();

    //sparse matrix file(csc)
    std::string in_path_ = "D:\\github\\Axb\\sympiler\\LFAT5.mtx";
    std::ifstream fin(in_path_);
    format::CSC *H;
    read_mtx_csc_real(fin, H, true);

   /* for(int i=0; i<H->m;i++)
    {
        for(int j=H->p[i]; j<H->p[i+1]; j++)
        {
            cout<<H->i[j]<<"\t";
            cout<<i<<"\t";
            cout<<H->x[j]<<"\n";
        }
    }*/

    //print of csc file
    CSR *cf = new CSR(H->n, H->m, H->nnz, H->p, H->i, H->x);
    cf->triplet();

    //multiply of csc(file) and vector(file)
    Matrix *mf = cf->turnToRegular();
    mf->print();
    Matrix *vf = cf->csrMult();
    vf->print();

    //csr file Solve
    Matrix *rhsf = new Matrix(H->n,1,"new rhs:");
    rhsf->Random();
    rhsf->print();
    TriangularSolve *rs2 = new TriangularSolve(H->n);
    Matrix *result2 = rs2->solve( cf, rhsf);
    result2->print();

   //csr Solve,Test
    Matrix *rs3 = cf->csrMult(result2);
    rs3->print();
    bool r2 = rs3->isequal(rhsf);
    cout<<"the result of Triangular solve is: "<<r2 <<"\n";

    //CSC
    //csc print file
    CSC *ccf = new CSC(H->n, H->m, H->nnz, H->p, H->i, H->x);
    ccf->Triplet();
    Matrix *cct = ccf->turnToRegular();
    cct->print();

    delete H;
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
    delete c2;
    delete rs;
    delete result;
    delete cf;
    delete mf;
    delete vf;
    delete rhsf;
    delete rs2;
    delete result2;
    delete rs3;
    delete ccf;
    delete cct;

    return 0;
}