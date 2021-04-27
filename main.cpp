
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
#include "testUtils.h"


//#include "stdafx.h"
using namespace std;



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
    //Diagonal should be odd number, dim: number of rows  - dl=number of diagonal

    return 0;
}
