
## Axb: Sparsity-Aware Storage Format Selection

This repository provides a prototype for the [https://github.com/leila68/Axb/blob/master/Axb_paper.pdf]() and explores the effect of 
sparse storage formats on different sparse kernels. 

### Prerequisites:
* CMake
* C++ compiler (tested for GCC)

### Installation
To install Axb, you should first clone the repository and then build it 
as shown below:
```
git clone https://github.com/leila68/Axb.git
cd Axb
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release .. 
make 
```

### Running Axb
Axb provides two different demo that each tests sparse matrix-vector multiplication
 (SpMV) and sparse lower triangular matrix solver (SpTRSV) for general
 sparse matrices in matrix market format and random banded matrices. 
 
 To test the two different kernels for matrix market formats, you 
 should put the list of matrix paths, line by line, in a separate file
  and then emit the following:
  ```
  Axb list_file_path.txt > out1.csv
  ```
  An example input file containing matrix file paths is in [https://github.com/leila68/Axb/blob/master/doc/list_file_path](). 
  
  To test different formats on a banded matrix, you should provide two 
  integer values, the matrix dimension `dim` and the number of bands 
  `d`. The following command shows Axb evaluates banded matrices:
  ```
  Axb dim d > out2.csv
  ```
 
  In either demo, Axb generates CSV files. Some example graphs based on a set of 
  matrices are shown in section 3 of [https://github.com/leila68/Axb/blob/master/Readme.md]().
  
  
 
