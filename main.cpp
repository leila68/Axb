
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
#include "diagonal.h"
#include <chrono>

//#include "stdafx.h"
using namespace std;

void addMultTest();
void averageTest();
void triangularTest();
void  csrTest();
void  csrFileTest();
void  cscFileTest();
void diagonalTest();
void diagonalSolveTest();
void diagonal2Test();
void generalMultTest();
void generalSolveTest();

int main(int argc, char *argv[])
{

    srand((unsigned)time(NULL));
    std::string f1 = argv[1];

    // addMultTest();
    // averageTest();
    // triangularTest();
   //  csrTest();
 // csrFileTest(); //there is a problem, show a message after while it works
    //cscFileTest();
 // diagonalTest();
  //  diagonalSolveTest();
   // diagonal2Test();
   generalMultTest();
   // generalSolveTest();
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
    TriangularSolve *rs = new TriangularSolve(5);
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

void  csrFileTest()
{
    //sparse matrix file(csc)
  //  std::string in_path_ = "D:\\github\\Axb\\sympiler\\LFAT5.mtx";
    std::string in_path_ ="D:\\github\\Axb\\sympiler\\bcsstk01.mtx" ;
    std::ifstream fin(in_path_);
    format::CSC *H;
    read_mtx_csc_real(fin, H, true);

    //print of csc file
    CSR *ccf = new CSR(H->n, H->m, H->nnz, H->p, H->i, H->x);
    //ccf->triplet();

    //turn to csr
    cout<<"CSR:"<<"\n";
    ccf->turntoCSR();
   // ccf->triplet();

    //csc file Solve
    Matrix *rhcr = new Matrix(H->n,1,"rhcr:");
    rhcr->One();
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
    result2->print();

    //csr Solve,Test the equation(ccf*y=rhcr)
    Matrix *rs3 = ccf->csrMult(result2);
  //  rs3->print();
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
 // std::string in_path_ = "D:\\github\\Axb\\sympiler\\LFAT5.mtx";
    std::string in_path_ ="D:\\github\\Axb\\sympiler\\bcsstk01.mtx" ;
    std::ifstream fin(in_path_);
    format::CSC *H;
    read_mtx_csc_real(fin, H, true);

    //CSC
    //csc print file

    CSC *cc = new CSC(H->n, H->m, H->nnz, H->p, H->i, H->x);
    Matrix *mcc = new Matrix(H->n,1," vector: ");
    mcc->Random();


  //  mcc->print();
 //   cc->Triplet();
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
    cout<<"csc (solve):"<<"\n";
   // result3->print();
   // cout<<"print mcc again";


    //csc Solve,Test
    Matrix *mcc2 = cc->cscMult(result3);
  //  mcc2->print();
    bool r2 = mcc->isequal(mcc2);
    cout<<"the result ofc testing Triangular solve  (CSC) is: "<<r2 <<"\n";

    delete H;
    delete cc;
    delete mcc;
    delete ccr;
    delete ccs;
    delete result3;
    delete mcc2;


}

void  diagonalTest()
{
      Matrix *dia = new Matrix(10,10," ");
      dia->Random(5,"3diagonal");
      dia->print();
      diagonal *d1 = new diagonal(10,10,5);
      Matrix *d2 = d1->diaStore(dia,-1);
      d1->offsetCompute(-1);
      Matrix *v1 = new Matrix(10,1,"vector:");
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

void diagonalSolveTest()
{
    //Solve
    Matrix *dia = new Matrix(10,10," ");
    dia->Random(5,"3diagonal");
    dia->LowerTriangular();
    dia->print();
    diagonal *d1 = new diagonal(10,10,5);
    Matrix *d2 = d1->diaStore(dia,1);//1 for Lower Triangular Matrix
    d2->print();

    Matrix *v1 = new Matrix(10,1,"vector:");
    v1->Random();
   // v1->One();
    int *f = d1->offsetCompute(1);//1 for Lower Triangular Matrix
    TriangularSolve *d = new TriangularSolve(10);//number of rows

    Matrix *result = d->solve(d2,v1,f,5);
    result->print();
    Matrix *ts = d1->diaMult(d2,result);
    bool a = ts->isequal(v1);
    cout<<"testing solve: "<<a<<"\n";


    delete dia;
    delete d1;
    delete d2;
    delete v1;
    delete f;
    delete d;
    delete result;
    delete ts;
}

void diagonal2Test()
{
    Matrix *m1 = new Matrix(10,10,"diagonal");
    m1->Random1(5,"diagonal");//z:number of diagonals
   // m1->print();
    Matrix *v1 = new Matrix(10,1,"vector");
   // v1->Random();
   v1->One();
     // v1->print();

    diagonal *d1 = new diagonal(10,10,5);//d:number of diagonals

    Matrix *m2 = d1->diaStore2(m1,0);//a is for determining lower or regular matrix
   //m2->print();

    //multiply
    Matrix *m3 = d1->diaMult2(m2,v1);
  //  m3->print();

    //solve
    TriangularSolve *t1 = new TriangularSolve(10);
    Matrix *result = t1->solve(m2,v1,5); //d:number of diagonals
    //result->print();
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

void generalMultTest()
{
    // 5for rows - 2 for dia
    Matrix *dia1 = new Matrix(100,100," ");
    dia1->Random(11,"diagonal");
    dia1->LowerTriangular();
   // dia1->print();

    //save matrix in csr format
    CSR *cr1 = new CSR(dia1);
   // cr1->triplet();
    Matrix *v1 = new Matrix(100,1,"vector:");
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
    diagonal *d1 = new diagonal(100,100,11);
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

void generalSolveTest()
{
// 7 for col and row - 4 got diagonal
    Matrix *dia = new Matrix(100,100," ");
    dia->Random(21,"3diagonal");
    dia->LowerTriangular();
   // dia->print();
    Matrix *v1 = dia->addTest(); // for testing vOne
    //v1->print();
    Matrix *vOne = new Matrix(100,1,"v for test"); // for testing with vOne
    vOne->One();
   // Matrix *v1 = new Matrix(100, 1, "rhs");
  //  v1->Random();
   // v1->print();
    TriangularSolve *s = new TriangularSolve(100);

//csrSolve
    CSR *cr1 = new CSR(dia);
    TriangularSolve *rs = new TriangularSolve(100);//n:row numbers
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
    diagonal *d1 = new diagonal(100,100,21);
    Matrix *d2 = d1->diaStore(dia,1);//1 for Lower Triangular Matrix
     // d2->print();
    int *f = d1->offsetCompute(1);//1 for Lower Triangular Matrix

   // TriangularSolve *d = new TriangularSolve(100);//n:row number
    start = std::chrono::system_clock::now();
       Matrix *result3 = s->solve(d2,v1,f,21);
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
       Matrix *result4 = s->solve(d3,v1,21);//number of diagonals
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

    delete v1;
    delete cr1;
    delete rs;
    delete rs2;
//    delete cs;
    delete cc1;
    delete result1;
    delete mcc2;
    delete vOne;

    delete dia;
    delete d1;
    delete d2;
    delete f;
  //  delete d;
    delete result3;
    delete ts1;
    delete d3;

    delete result4;
    delete ts2;
    delete s;



}