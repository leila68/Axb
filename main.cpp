
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include <vector>
#include "TriangularSolve.h"
#include "CSR.h"
#include "math.h"
#include  "sympiler-io/io.h"
#include "CSC.h"
#include "diagonal.h"
#include <chrono>


//#include "stdafx.h"
using namespace std;

void addMultTest(int dim);
void averageTest(int dim);
void triangularTest(int dim);
void  csrTest(int dim);
void  csrFileTest(string f1);
void  cscFileTest(string f1);
void  RunAll1(string f1);
void  RunAll2(string f1);
void diagonalTest(int dim, int dl);
void diagonalSolveTest(int dim, int dl);
void diagonal2Test(int dim, int dl);
void generalMultTest(int dim, int dl);
void generalSolveTest(int dim, int dl);
void generalMultTest1(int dim, int dl);
void generalSolveTest1(int dim, int dl);
void evaluatingFormatsMatrices(string f1);

int main(int argc, char *argv[])
{
    srand((unsigned)time(NULL));

    if(argc == 2)
     {
         string f1 = argv[1];
         evaluatingFormatsMatrices(f1);
     }
   if(argc > 2)
    {
       int dim =  stoi(argv[1]);
       int dl = stoi(argv[2]);


        // diagonalTest(dim,dl);
        // diagonalSolveTest(dim,dl);
        // diagonal2Testdim,dl);
        //  generalMultTest(dim,dl);
        //  generalSolveTest(dim,dl);
        // generalMultTest1(dim,dl);//this function repeat each multiply 5 times
         generalSolveTest1(dim,dl);//this function repeat each solve 5 times
    }


    //Diagonal should be odd number dim: number of rows  - dl=number of diagonal

    // addMultTest(5);
    // averageTest(10);
   //  triangularTest(7);
   //  csrTest(10);
   // csrFileTest(f1);
  // cscFileTest(f1);

    return 0;
}
void evaluatingFormatsMatrices(string f1)
{
    std::ifstream f (f1);
    std::string name;

    while(std::getline(f, name) )
    {
        if (name.empty())
        {
            break;
        }
        // cout<<name<<"\n";
        RunAll2(name);
    }
    f.close();
}

void addMultTest(int dim)
{
    //Print matrix(dense)1
    Matrix *mtx1 = new Matrix(dim, dim, "Matrix1:");
    mtx1->Random();
    mtx1->print();

   //Print matrix(dense)2
    Matrix *mtx2 = new Matrix(dim, dim, "Matrix2:");
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
    delete mtx3;
    delete ml;
}

void averageTest(int dim)
{
    Matrix *mtx3 = new Matrix(dim,dim,"Matrix3");//???
    mtx3->Random();
    mtx3->print();
    //row average, matrix3
    Matrix *ave1 = mtx3->averageRow();
    ave1->print();
    //col average, matrix3
    Matrix *ave2 = mtx3->averageCol();
    ave2->print();

    delete mtx3;
    delete ave1;
    delete ave2;

}

void triangularTest(int dim)
{
    //triangular solve
    //print lower triangular matrix
    Matrix *mtxL = new Matrix(dim,dim,"Lower Triangular Matrix:");
    mtxL->Random();
    mtxL->LowerTriangular();
    mtxL->print();
    //print right hand side
    Matrix *rhs = new Matrix(dim, 1, "rhs Matrix:");
    rhs->Random();
    rhs->print();

    //solve the equation(mtxl*y=rhs)
    TriangularSolve *ts1 = new TriangularSolve(dim);
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
    delete ts1;
    delete y;
    delete q;
}

void  csrTest(int dim)
{
    Matrix *mtxL = new Matrix(dim,dim,"Lower Triangular");
    mtxL->Random();
    mtxL->LowerTriangular();

    Matrix *rhs = new Matrix(dim, 1, "rhs");
    rhs->Random();

    //CSR print & mult
    Matrix *spr = new Matrix(dim,dim, "Sparse Matrix:");
    spr->Random(dim);
 //   spr->print();
    //save matrix in csr format
    CSR *c1 = new CSR(spr);
    c1->triplet();
    Matrix *reg =  c1->turnToRegular();
  //  reg->print();

    Matrix *vector = c1->csrMult();
   // vector->print();

    //csrSolve
  //  mtxL->print();
  //  rhs->print();
    CSR *c2 = new CSR(mtxL);
    TriangularSolve *rs = new TriangularSolve(dim);
    Matrix *result = rs->solve(c2,rhs);
    result->print();

    //csr Solve,Test the equation(c2*y=rhs)
    Matrix *rs3 = c2->csrMult(result);
 //   rs3->print();
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

void  csrFileTest(string f1)
{

    //sparse matrix file(csc)
   // std::string in_path_ = "D:\\github\\Axb\\sympiler\\LFAT5.mtx";
  // std::string in_path_ = "D:\\github\\Axb\\sympiler\\bcsstk01.mtx";
    std::string in_path_ = f1;
    std::ifstream fin(in_path_);
    format::CSC *H;
    read_mtx_csc_real(fin, H, true);

    //print of csc file
    CSR *ccf = new CSR(H->n, H->m, H->nnz, H->p, H->i, H->x);
    //ccf->triplet();

    //turn to csr
   // cout<<"CSR:"<<"\n";
    ccf->turntoCSR();
   // ccf->triplet();

    //csc file Solve
    Matrix *rhcr = new Matrix(H->n,1,"rhcr:");
    rhcr->Random();
    // rhcr->print();
    TriangularSolve *rs2 = new TriangularSolve(H->n);
    //time
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> elapsed_seconds;
    start = std::chrono::system_clock::now();
       Matrix *result2 = rs2->solve( ccf, rhcr);
    end = std::chrono::system_clock::now();
    elapsed_seconds = end - start;
    double durationSym = elapsed_seconds.count();
    cout << "exacution time (CSR Solve):" << durationSym << "\n";
    //result2->print();

    //csr Solve,Test the equation(ccf*y=rhcr)
    Matrix *rs3 = ccf->csrMult(result2);
  //  rs3->print();
    bool r2 = rs3->isequal(rhcr);
   // cout<<"the result of Triangular solve is(CSR): "<<r2 <<"\n";

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

}

void  cscFileTest(string f1)
{
    //sparse matrix file(csc)
  //std::string in_path_ = "D:\\github\\Axb\\sympiler\\LFAT5.mtx";
  //  std::string in_path_ ="D:\\github\\Axb\\sympiler\\bcsstk01.mtx" ;
    std::string in_path_ = f1 ;
    std::ifstream fin(in_path_);
    format::CSC *H;
    read_mtx_csc_real(fin, H, true);

    //CSC
  //  csc print file

    CSC *cc = new CSC(H->n, H->m, H->nnz, H->p, H->i, H->x);
    Matrix *mcc = new Matrix(H->n,1," vector: ");
    mcc->Random();

  //  mcc->print();
   // cc->Triplet();
    Matrix *ccr = cc->cscMult(mcc);

   // ccr->print();

    //csc Solve
    TriangularSolve *ccs = new TriangularSolve(H->n);
    //time
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> elapsed_seconds;
    start = std::chrono::system_clock::now();
         Matrix *result3 = ccs->solve(cc,mcc);
   end = std::chrono::system_clock::now();
    elapsed_seconds = end - start;
    double durationSym = elapsed_seconds.count();
    cout << "exacution time (CSC Solve):" << durationSym << "\n";
    //cout<<"csc (solve):"<<"\n";
   // result3->print();
   // cout<<"print mcc again";


    //csc Solve,Test
    Matrix *mcc2 = cc->cscMult(result3);
  //  mcc2->print();
    bool r2 = mcc->isequal(mcc2);
   // cout<<"the result of testing Triangular solve  (CSC) is: "<<r2 <<"\n";

    delete H;
    delete cc;
    delete mcc;
    delete ccr;
    delete ccs;
    delete result3;
    delete mcc2;


}

void  RunAll1(string f1)
{
    //CSR
    std::string in_path_ = f1;
    std::ifstream fin(in_path_);
    format::CSC *H;
    read_mtx_csc_real(fin, H, true);

    //print of csc file
    CSR *ccf = new CSR(H->n, H->m, H->nnz, H->p, H->i, H->x);
    //ccf->triplet();

    //turn to csr
    // cout<<"CSR:"<<"\n";
    ccf->turntoCSR();
    // ccf->triplet();

    //csc file Solve
    Matrix *v = new Matrix(H->n,1,"vector:");
    v->Random();
    // v->print();
    TriangularSolve *rs2 = new TriangularSolve(H->n);
    //time
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> elapsed_seconds;
        start = std::chrono::system_clock::now();
        Matrix *result2 = rs2->solve( ccf, v);
        end = std::chrono::system_clock::now();
        elapsed_seconds = end - start;
        double durationSym1 = elapsed_seconds.count();


   // cout << "exacution time (CSR Solve):" << durationSym1 << "\n";
    //result2->print();

   // csr Solve,Test the equation(ccf*y=rhcr)
     Matrix *rs3 = ccf->csrMult(result2);
    //  rs3->print();
      bool r2 = rs3->isequal(v);
    // cout<<"the result of Triangular solve is(CSR): "<<r2 <<"\n";

    //multiply of csc(file) and vector(file)
    //Matrix *mf = cf->turnToRegular();
    // mf->print();
    //Matrix *vf = cf->csrMult();
    //  vf->print();


    //CSC

   // std::string in_path_ = f1 ;
  //  std::ifstream fin(in_path_);
  //  format::CSC *H;
  //  read_mtx_csc_real(fin, H, true);

    //  csc print file

    CSC *cc = new CSC(H->n, H->m, H->nnz, H->p, H->i, H->x);
    // cc->Triplet();
    Matrix *ccr = cc->cscMult(v);

    // ccr->print();

    //csc Solve
    TriangularSolve *ccs = new TriangularSolve(H->n);
    //time

    start = std::chrono::system_clock::now();
    Matrix *result3 = ccs->solve(cc,v);
    end = std::chrono::system_clock::now();
    elapsed_seconds = end - start;
    double durationSym2 = elapsed_seconds.count();

    //PRINT All
    //cout<<"csr"<<","<<"csc"<<","<<"\n";
     cout << durationSym1<<"," << durationSym2 << "\n";
    //cout<<"csc (solve):"<<"\n";
    // result3->print();
    // cout<<"print mcc again";


    //csc Solve,Test
    Matrix *mcc2 = cc->cscMult(result3);
    //  mcc2->print();
    bool r3 = v->isequal(mcc2);
    // cout<<"the result of testing Triangular solve  (CSC) is: "<<r2 <<"\n";

    delete H;
    delete ccf;
    delete v;
    delete rs2;
    delete result2;
    delete rs3;
    //CSC
    delete cc;
    delete ccr;
    delete ccs;
    delete result3;
    delete mcc2;

}

void  RunAll2(string f1)
{
    vector<double> t1;
    vector<double> t2;
    //CSR
    std::string in_path_ = f1;
    std::ifstream fin(in_path_);
    format::CSC *H;
    read_mtx_csc_real(fin, H, true);

    //print of csc file
    CSR *ccf = new CSR(H->n, H->m, H->nnz, H->p, H->i, H->x);
    //ccf->triplet();

    //turn to csr
    // cout<<"CSR:"<<"\n";
    ccf->turntoCSR();
    // ccf->triplet();

    //csc file Solve
    Matrix *v = new Matrix(H->n,1,"vector:");
    v->Random();
    // v->print();
    TriangularSolve *rs2 = new TriangularSolve(H->n);
    //time
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> elapsed_seconds;
    for(int i=0; i<5; i++)
    {
        start = std::chrono::system_clock::now();
        Matrix *result2 = rs2->solve( ccf, v);
        end = std::chrono::system_clock::now();
        elapsed_seconds = end - start;
        double durationSym1 = elapsed_seconds.count();
        t1.push_back(durationSym1);
        // cout << "exacution time (CSR Solve):" << durationSym1 << "\n";
        //result2->print();
       delete result2;
    }
    sort(t1.begin(), t1.end());
    cout << "Sorted(CSR)"<<",";
    for (auto x : t1)
        cout << x << " ,";
     cout<<"\n";



    // csr Solve,Test the equation(ccf*y=rhcr)
  //  Matrix *rs3 = ccf->csrMult(result2);
    //  rs3->print();
    //bool r2 = rs3->isequal(v);
    // cout<<"the result of Triangular solve is(CSR): "<<r2 <<"\n";

    //multiply of csc(file) and vector(file)
    //Matrix *mf = cf->turnToRegular();
    // mf->print();
    //Matrix *vf = cf->csrMult();
    //  vf->print();


    //CSC

    // std::string in_path_ = f1 ;
    //  std::ifstream fin(in_path_);
    //  format::CSC *H;
    //  read_mtx_csc_real(fin, H, true);

    //  csc print file

    CSC *cc = new CSC(H->n, H->m, H->nnz, H->p, H->i, H->x);
    // cc->Triplet();
    Matrix *ccr = cc->cscMult(v);

    // ccr->print();

    //csc Solve
    TriangularSolve *ccs = new TriangularSolve(H->n);
    //time
    for(int i=0; i<5; i++)
    {
        start = std::chrono::system_clock::now();
        Matrix *result3 = ccs->solve(cc,v);
        end = std::chrono::system_clock::now();
        elapsed_seconds = end - start;
        double durationSym2 = elapsed_seconds.count();
        t2.push_back(durationSym2);
        // result3->print();
        delete result3;
    }
    sort(t2.begin(), t2.end());
    cout << "Sorted(CSC)"<<",";
    for (auto x : t2)
        cout << x << " ,";
    cout<<"\n";

    //PRINT All
    //cout<<"csr"<<","<<"csc"<<","<<"\n";
   // cout << durationSym1<<"," << durationSym2 << "\n";
    //cout<<"csc (solve):"<<"\n";
    // cout<<"print mcc again";


    //csc Solve,Test
   // Matrix *mcc2 = cc->cscMult(result3);
    //  mcc2->print();
  //  bool r3 = v->isequal(mcc2);
    // cout<<"the result of testing Triangular solve  (CSC) is: "<<r2 <<"\n";

    delete H;
    delete ccf;
    delete v;
    delete rs2;
   // delete result2;
   // delete rs3;
    //CSC
    delete cc;
    delete ccr;
    delete ccs;
   // delete result3;
  //  delete mcc2;


}

void  diagonalTest(int dim, int dl)
{
      Matrix *dia = new Matrix(dim,dim," ");
      dia->Random(dl,"3diagonal");
      dia->print();
      diagonal *d1 = new diagonal(dim,dim,dl);
      Matrix *d2 = d1->diaStore(dia,-1);
      d1->offsetCompute(-1);
      Matrix *v1 = new Matrix(dim,1,"vector:");
      v1->Random();
      v1->print();
       Matrix *d3 = d1->diaMult(d2,v1);
      d3->print();


    delete dia;
    delete d1;
    delete d2;
    delete v1;
    delete d3;
}

void diagonalSolveTest(int dim, int dl)
{
    //Solve
    Matrix *dia = new Matrix(dim,dim," ");
    dia->Random(dl,"3diagonal");
    dia->LowerTriangular();
  //  dia->print();
    diagonal *d1 = new diagonal(dim,dim,dl);
    Matrix *d2 = d1->diaStore(dia,1);//1 for Lower Triangular Matrix
   // d2->print();

    Matrix *v1 = new Matrix(dim,1,"vector:");
    v1->Random();

    int *f = d1->offsetCompute(1);//1 for Lower Triangular Matrix
    TriangularSolve *d = new TriangularSolve(dim);//number of rows

    Matrix *result = d->solve(d2,v1,f,dl);
    result->print();
    Matrix *ts = d1->diaMult(d2,result);
    bool a = ts->isequal(v1);
    cout<<"testing solve: "<<a<<"\n";


    delete dia;
    delete d1;
    delete d2;
    delete v1;
    delete []f;
    delete d;
    delete result;
    delete ts;
}

void diagonal2Test(int dim, int dl)
{
    Matrix *m1 = new Matrix(dim,dim,"diagonal");
    m1->Random(dl,"diagonal");//z:number of diagonals
    m1->print();
    Matrix *v1 = new Matrix(dim,1,"vector");
    v1->Random();
   // v1->One();
     // v1->print();

    diagonal *d1 = new diagonal(dim,dim,dl);//d:number of diagonals

    Matrix *m2 = d1->diaStore2(m1,0);//a is for determining lower or regular matrix
    m2->print();

    //multiply
    Matrix *m3 = d1->diaMult2(m2,v1);
  //  m3->print();

    //solve
    TriangularSolve *t1 = new TriangularSolve(dim);
    Matrix *result = t1->solve(m2,v1,dl); //d:number of diagonals
    result->print();
    //solve test
    Matrix *ts = d1->diaMult2(m2,result);
  //  ts->print();
    bool a = ts->isequal(v1);

    cout<<"testing solve: "<<a<<"\n";

    delete d1;
    delete m1;
    delete m2;
    delete v1;
    delete m3;
    delete t1;
    delete result;
    delete ts;
}

void generalMultTest(int dim, int dl)
{
    // 5for rows - 2 for dia
    Matrix *dia1 = new Matrix(dim,dim," ");
    dia1->Random(dl,"diagonal");
    dia1->LowerTriangular();
   // dia1->print();

    //save matrix in csr format
    CSR *cr1 = new CSR(dia1);
   // cr1->triplet();
    Matrix *v1 = new Matrix(dim,1,"vector:");
    //v1->Random();

      v1->One();
      //v1->print();
    //execution time-start
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> elapsed_seconds;
    start = std::chrono::system_clock::now();
          Matrix *result1 = cr1->csrMult(v1);
    end = std::chrono::system_clock::now();
    elapsed_seconds = end-start;
    double durationSym1=elapsed_seconds.count();
    cout<<"exacution time(CSR):"<< durationSym1<<"\n";
    //execution time-end

   //result1->print();

    //save matrix in csc format
    CSC *cc1 = new CSC(dia1);
   // cc1->Triplet();

    //execution time-start
    start = std::chrono::system_clock::now();
       Matrix *result2 = cc1->cscMult(v1);
    end = std::chrono::system_clock::now();
    elapsed_seconds = end-start;
    double durationSym2=elapsed_seconds.count();
    cout<<"exacution time(CSC):"<< durationSym2 <<"\n";
    //execution time-end
    // result2->print();

  //save matrix in diagonal format
    diagonal *d1 = new diagonal(dim,dim,dl);
    Matrix *d2 = d1->diaStore(dia1,1);
    d1->offsetCompute(1);
  //  d2->print();
    //execution time-start
    start = std::chrono::system_clock::now();
        Matrix *result3 = d1->diaMult(d2,v1);
    end = std::chrono::system_clock::now();
    elapsed_seconds = end - start;
    double durationSym3 = elapsed_seconds.count();
    cout << "exacution time (dia):" << durationSym3 << "\n";
    //execution time-end
  //  result3->print();


// diagonal2
    Matrix *m2 = d1->diaStore2(dia1, 1);
   // m2->print();
    start = std::chrono::system_clock::now();
        Matrix *result4 = d1->diaMult2(m2,v1);
    end = std::chrono::system_clock::now();
    elapsed_seconds = end - start;
    double durationSym4 = elapsed_seconds.count();
    cout << "exacution time (dia2):" << durationSym4 << "\n";
    //result4->print();

    //testing the result
    bool t1 = result1->isequal(result2);
    bool  t2 = result2->isequal(result3);
    bool  t3 = result2->isequal(result4);
    bool  t4 = result3->isequal(result4);
    bool  t5 = result1->isequal(result4);
    cout << "the result of multiply of csc & csr & dia & dia2 is:"<< t1 <<","<< t2 << "," << t3<<"," << t4<<"," << t5<<"\n";

    cout<<  "\n";

    delete dia1;
    delete cr1;
    delete v1;
    delete result1;
    delete result2;
    delete cc1;
    delete d1;
    delete d2;
    delete result3;
    delete m2;
    delete result4;
  //  delete d4;

}

void generalSolveTest(int dim, int dl)
{
// 7 for col and row - 4 got diagonal
    Matrix *dia = new Matrix(dim,dim," ");
    dia->Random(dl,"3diagonal");
    dia->LowerTriangular();
   // dia->print();
    Matrix *v1 = dia->addTest(); // for testing vOne. this "v1" cause all rows of the result-vector be just "1"
    //v1->print();
     Matrix *vOne = new Matrix(dim,1,"v for test"); // for testing the result. I used at the end
     vOne->One();
   // Matrix *v1 = new Matrix(dim, 1, "rhs");
    // v1->Random();
   // v1->print();
    TriangularSolve *s = new TriangularSolve(dim);

//csrSolve
    CSR *cr1 = new CSR(dia);
    TriangularSolve *rs = new TriangularSolve(dim);//n:row numbers
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> elapsed_seconds;
    start = std::chrono::system_clock::now();
       Matrix *result1 = s->solve(cr1,v1);
    end = std::chrono::system_clock::now();
    elapsed_seconds = end - start;
    double durationSym1 = elapsed_seconds.count();
    cout << "exacution time (CSR Solve):" << durationSym1 << "\n";
  // result1->print();
    //csr Solve,Test
    Matrix *rs2 = cr1->csrMult(result1);
    //   rs3->print();
    bool r1 = rs2->isequal(v1);
    cout<<"Testing solve (CSR): "<<r1 <<"\n";

//CSC solve
    CSC *cc1 = new CSC(dia);
  //  TriangularSolve *cs = new TriangularSolve(100);
    start = std::chrono::system_clock::now();
       Matrix *result2 = s->solve(cc1,v1);
    end = std::chrono::system_clock::now();
    elapsed_seconds = end - start;
    double durationSym2 = elapsed_seconds.count();
    cout << "exacution time (CSC Solve):" << durationSym2 << "\n";
   //result2->print();
   //csc Solve,Test
    Matrix *mcc2 = cc1->cscMult(result2);
    //  mcc2->print();
    bool r2 = mcc2->isequal(v1);
    cout<<"Testing solve (CSC) is: "<<r2 <<"\n";

 //dia1
    diagonal *d1 = new diagonal(dim,dim,dl);
    Matrix *d2 = d1->diaStore(dia,1);//1 for Lower Triangular Matrix
     // d2->print();
    int *f = d1->offsetCompute(1);//1 for Lower Triangular Matrix

   // TriangularSolve *d = new TriangularSolve(100);//n:row number
    start = std::chrono::system_clock::now();
       Matrix *result3 = s->solve(d2,v1,f,dl);
    end = std::chrono::system_clock::now();
    elapsed_seconds = end - start;
    double durationSym3 = elapsed_seconds.count();
    cout << "exacution time (diagonal1 Solve):" << durationSym3 << "\n";
   // result3->print();
   //test
    Matrix *ts1 = d1->diaMult(d2,result3);
    bool r3 = ts1->isequal(v1);
    cout<<"testing solve(dia1): "<<r3<<"\n";

//dia2

    Matrix *d3 = d1->diaStore2(dia,1);
    //d3->print();
    start = std::chrono::system_clock::now();
       Matrix *result4 = s->solve(d3,v1,dl);//number of diagonals
    end = std::chrono::system_clock::now();
    elapsed_seconds = end - start;
    double durationSym4 = elapsed_seconds.count();
    cout << "exacution time (diagonal2 Solve):" << durationSym4 << "\n";
    // result4->print();
    //test
    Matrix *ts2 = d1->diaMult2(d3,result4);
    bool r4 = ts2->isequal(v1);
    cout<<"testing solve(dia2): "<<r4<<"\n";

    //comparing dia1 and dia2 solve
    bool r5 = result3->isequal(result4);
    bool r6 = result1->isequal(result2);
    bool r7 = result2->isequal(result3);
    cout<<"dia1&dia2&csr&csc: "<<r5<< ","<< r6 << ","<< r7<<"\n";

    cout<<"test with  vector One:"<<"\n";
    bool r10 = result1->isequal(vOne);
    bool r11 = result2->isequal(vOne);
    bool r12 = result3->isequal(vOne);
    bool r13 = result4->isequal(vOne);
    cout<<"dia1&dia2&csr&csc: "<<r10<< ","<< r11 << ","<< r12<<","<< r13<<"\n";

    delete dia;
    delete v1;
    delete vOne;
    delete s;
    delete cr1;
    delete rs;
    delete result1;
    delete rs2;
//    delete cs;
    delete cc1;
    delete result2;
    delete mcc2;
    delete d1;
    delete d2;
    delete []f;
  //  delete d;
    delete result3;
    delete ts1;
    delete d3;
    delete result4;
    delete ts2;
}

void generalMultTest1(int dim, int dl)
{
    // 5for rows - 2 for dia
    vector<double> tm1;
    vector<double> tm2;
    vector<double> tm3;
    vector<double> tm4;
    Matrix *dia1 = new Matrix(dim,dim," ");
    dia1->Random(dl,"diagonal");
    dia1->LowerTriangular();
    // dia1->print();

    //save matrix in csr format
    CSR *cr1 = new CSR(dia1);
    // cr1->triplet();
    Matrix *v1 = new Matrix(dim,1,"vector:");
    //v1->Random();

    v1->One();
    //v1->print();
    //execution time-start
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> elapsed_seconds;
    for(int i=0; i<5; i++)
    {
        start = std::chrono::system_clock::now();
          Matrix *result1 = cr1->csrMult(v1);
        end = std::chrono::system_clock::now();
        elapsed_seconds = end-start;
        double durationSym1=elapsed_seconds.count();
       // cout<<"exacution time(CSR):"<< durationSym1<<"\n";
        //execution time-end
        //result1->print();
        tm1.push_back(durationSym1);
        delete result1;
    }
    sort(tm1.begin(), tm1.end());
    cout << "multiply(CSR)"<<",";
    for (auto x : tm1)
        cout << x << " ,";
    cout<<"\n";

    //save matrix in csc format
    CSC *cc1 = new CSC(dia1);
    // cc1->Triplet();

    //execution time-start
    for(int i=0; i<5; i++)
    {
        start = std::chrono::system_clock::now();
        Matrix *result2 = cc1->cscMult(v1);
        end = std::chrono::system_clock::now();
        elapsed_seconds = end-start;
        double durationSym2=elapsed_seconds.count();
       // cout<<"exacution time(CSC):"<< durationSym2 <<"\n";
        //execution time-end
        // result2->print();
        tm2.push_back(durationSym2);
        delete result2;
    }
    sort(tm2.begin(), tm2.end());
    cout << "multiply(CSC)"<<",";
    for (auto x : tm2)
        cout << x << " ,";
    cout<<"\n";

    //save matrix in diagonal format
    diagonal *d1 = new diagonal(dim,dim,dl);
    Matrix *d2 = d1->diaStore(dia1,1);
    d1->offsetCompute(1);
    //  d2->print();
    //execution time-start
    for(int i=0; i<5; i++)
    {
        start = std::chrono::system_clock::now();
        Matrix *result3 = d1->diaMult(d2,v1);
        end = std::chrono::system_clock::now();
        elapsed_seconds = end - start;
        double durationSym3 = elapsed_seconds.count();
      //  cout << "exacution time (dia):" << durationSym3 << "\n";
        //execution time-end
        //  result3->print();
        tm3.push_back(durationSym3);
        delete result3;
    }
    sort(tm3.begin(), tm3.end());
    cout << "multiply(dia1)"<<",";
    for (auto x : tm3)
        cout << x << " ,";
    cout<<"\n";

// diagonal2
    Matrix *m2 = d1->diaStore2(dia1, 1);
    // m2->print();
    for(int i=0; i<5; i++)
    {
        start = std::chrono::system_clock::now();
        Matrix *result4 = d1->diaMult2(m2,v1);
        end = std::chrono::system_clock::now();
        elapsed_seconds = end - start;
        double durationSym4 = elapsed_seconds.count();
       // cout << "exacution time (dia2):" << durationSym4 << "\n";
        //result4->print();
        tm4.push_back(durationSym4);
        delete result4;
    }
    sort(tm4.begin(), tm4.end());
    cout << "multiply(dia2)"<<",";
    for (auto x : tm4)
        cout << x << " ,";
    cout<<"\n";

    //testing the result
    /*bool  t1 = result1->isequal(result2);
    bool  t2 = result2->isequal(result3);
    bool  t3 = result2->isequal(result4);
    bool  t4 = result3->isequal(result4);
    bool  t5 = result1->isequal(result4);
    cout << "the result of multiply of csc & csr & dia & dia2 is:"<< t1 <<","<< t2 << "," << t3<<"," << t4<<"," << t5<<"\n";

    cout<<  "\n";*/

    delete dia1;
    delete cr1;
    delete v1;
    delete cc1;
    delete d1;
    delete d2;
    delete m2;
    //  delete d4;
}

void generalSolveTest1(int dim, int dl)
{
// 7 for col and row - 4 for diagonal
    vector<double> t1;
    vector<double> t2;
    vector<double> t3;
    vector<double> t4;
    Matrix *dia = new Matrix(dim,dim," ");
    dia->Random(dl,"3diagonal");
    dia->LowerTriangular();
    // dia->print();
    Matrix *v1 = dia->addTest(); // for testing vOne
    //v1->print();
    Matrix *vOne = new Matrix(dim,1,"v for test"); // for testing with vOne
    vOne->One();
    // Matrix *v1 = new Matrix(100, 1, "rhs");
    //  v1->Random();
    // v1->print();
    TriangularSolve *s = new TriangularSolve(dim);

//csrSolve
    CSR *cr1 = new CSR(dia);
    TriangularSolve *rs = new TriangularSolve(dim);//n:row numbers
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> elapsed_seconds;
    for(int i=0; i<5; i++)
    {
        start = std::chrono::system_clock::now();
        Matrix *result1 = s->solve(cr1,v1);
        end = std::chrono::system_clock::now();
        elapsed_seconds = end - start;
        double durationSym1 = elapsed_seconds.count();
       // cout << "exacution time (CSR Solve):" << durationSym1 << "\n";
        // result1->print();
        t1.push_back(durationSym1);
        delete result1;
    }
    sort(t1.begin(), t1.end());
    cout << "solve(CSR)"<<",";
    for (auto x : t1)
        cout << x << ",";
    cout<<"\n";

    //csr Solve,Test
   // Matrix *rs2 = cr1->csrMult(result1);
    //   rs3->print();
   // bool r1 = rs2->isequal(v1);
  //  cout<<"Testing solve (CSR): "<<r1 <<"\n";

//CSC solve
    CSC *cc1 = new CSC(dia);
    //  TriangularSolve *cs = new TriangularSolve(100);
    for(int i=0; i<5; i++)
    {
        start = std::chrono::system_clock::now();
        Matrix *result2 = s->solve(cc1,v1);
        end = std::chrono::system_clock::now();
        elapsed_seconds = end - start;
        double durationSym2 = elapsed_seconds.count();
        //cout << "exacution time (CSC Solve):" << durationSym2 << "\n";
        //result2->print();
        t2.push_back(durationSym2);
        delete result2;
    }
   sort(t2.begin(),t2.end());
    cout << "solve(CSC)"<<",";
    for(auto x:t2)
        cout<<x<<",";
    cout<<"\n";
    //csc Solve,Test
    //Matrix *mcc2 = cc1->cscMult(result2);
    //  mcc2->print();
   // bool r2 = mcc2->isequal(v1);
  //  cout<<"Testing solve (CSC) is: "<<r2 <<"\n";

    //dia1
    diagonal *d1 = new diagonal(dim,dim,dl);
    Matrix *d2 = d1->diaStore(dia,1);//1 for Lower Triangular Matrix
    // d2->print();
    int *f = d1->offsetCompute(1);//1 for Lower Triangular Matrix

    // TriangularSolve *d = new TriangularSolve(100);//n:row number
    for(int i=0; i<5; i++)
    {
        start = std::chrono::system_clock::now();
        Matrix *result3 = s->solve(d2,v1,f,dl);
        end = std::chrono::system_clock::now();
        elapsed_seconds = end - start;
        double durationSym3 = elapsed_seconds.count();
        //cout << "exacution time (diagonal1 Solve):" << durationSym3 << "\n";
        // result3->print();
        t3.push_back(durationSym3);
        delete result3;
    }
    sort(t3.begin(), t3.end());
    cout << "solve(dia1)"<<",";
    for(auto x:t3)
        cout<<x<<",";
    cout<<"\n";
    //test
   //Matrix *ts1 = d1->diaMult(d2,result3);
   // bool r3 = ts1->isequal(v1);
   // cout<<"testing solve(dia1): "<<r3<<"\n";

//dia2

    Matrix *d3 = d1->diaStore2(dia,1);
    //d3->print();
    for(int i=0; i<5; i++)
    {
        start = std::chrono::system_clock::now();
        Matrix *result4 = s->solve(d3,v1,dl);//number of diagonals
        end = std::chrono::system_clock::now();
        elapsed_seconds = end - start;
        double durationSym4 = elapsed_seconds.count();
      // cout << "exacution time (diagonal2 Solve):" << durationSym4 << "\n";
        // result4->print();
        t4.push_back(durationSym4);
        delete result4;
    }
    sort(t4.begin(), t4.end());
    cout << "solve(dia2)"<<",";
    for(auto x:t4)
        cout<<x<<",";
    cout<<"\n";
    //test
    //Matrix *ts2 = d1->diaMult2(d3,result4);
   // bool r4 = ts2->isequal(v1);
   // cout<<"testing solve(dia2): "<<r4<<"\n";

    //comparing dia1 and dia2 solve
   /* bool r5 = result3->isequal(result4);
    bool r6 = result1->isequal(result2);
    bool r7 = result2->isequal(result3);
    cout<<"dia1&dia2&csr&csc: "<<r5<< ","<< r6 << ","<< r7<<"\n";

    cout<<"test with  vector One:"<<"\n";
    bool r10 = result1->isequal(vOne);
    bool r11 = result2->isequal(vOne);
    bool r12 = result3->isequal(vOne);
    bool r13 = result4->isequal(vOne);
    cout<<"dia1&dia2&csr&csc: "<<r10<< ","<< r11 << ","<< r12<<","<< r13<<"\n";*/

    delete dia;
    delete v1;
    delete vOne;
    delete s;
    delete cr1;
    delete rs;
    delete cc1;
    delete d1;
    delete d2;
    delete []f;
    delete d3;


   // delete rs2;
//    delete cs;
  //  delete result1;
   // delete mcc2;
    //  delete d;
   // delete result3;
  //  delete ts1;
  //  delete result4;
   // delete ts2;

}