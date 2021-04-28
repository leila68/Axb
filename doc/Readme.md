# Exploring the Effect of Sparse Storage Formats

---
Sparse matrix codes show different performances when a different
storage format is selected. In this report, we explore this using
four different storage format and for two different sparse
kernels. This report is based on [Axb]() published in 2018.


### 1- Sparse Storage Formats

There are several compressed formats for storing sparse matrices.
In all sparse storage formats, the nonzero elements are compressed
and stored in different arrays. Four different formats are explained in this section.
compressed sparse row (CSR), compressed sparse column(CSC), and two different diagonal formats. 
To illustrate different formats, an example sparse matrix shown in Figure1 is used.
<br>

![sparse matrix](https://github.com/leila68/Axb/blob/master/doc/mtx.png "mtx")
Figure 1: a sparse matrix with 4 rows, 4 columns, and 7 nonzero elements

**1-1- CSR:** Storing nonzero elements of a sparse matrix into a linear array *val* is done by going over each row in
order, and storing the nonzero elements to a linear array in the order they appear in the walk. Location and column
indices of nonzero elements of the *val* array in order stored in *row_ptr* and *col_idx*. The first element of row *i*
is stored *val[row_ptr[i]]* and the column index of the non-zero
element in *val[i]* is stored in *col_idx[i]*.<br>
val:[1,3,5,4,6,2,7]<br>
row_ptr:[0,1,3,5,7]<br>
col_idx:[0,0,1,1,2,2,3]<br>

The CSR storage of the sparse matrix in Figure 1 is shown above.
As shown, the nonzero element 5 in row 1 and column 1 is stored
in *val[row_ptr[1]+1]* and its column index is stored in *col_idx[row_ptr[1]+1]* where one is
the offset of the element from the first element in row one.

**1-2- CSC:** Storing the nonzero elements of a sparse matrix into a linear array is done by going over each column in order,
and writing the nonzero elements to a linear array in the order they appear in the walk. Location and row indices of
nonzero elements of the *val* array are in order stored in *col_ptr* and *row_idx*. The first element of column *j* is stored
in val[col_ptr[j]] and the row index of the nonzero elements in *val[j]* is stored in *row_idx[j]*.

val:[1,3,5,4,6,2,7]<br>
col_ptr:[0,2,4,6,7]<br>
row_idx:[0,1,1,2,2,3,3]<br>

The CSC storage of the sparse matrix in Figure 1 is shown above.
As shown, the nonzero element 5 in column 1 and row 1 is stored in *val[col_ptr[1]+0]* and its row
index is stored in *row_idx[col_ptr[1]]+0]* where zero is the offset of the element from the first column.<br>

**1-3- Diagonal Formats:** Since in a banded matrix, nonzero elements are
limited to diagonal bands, we only need to store
elements based on their diagonals. For this storage format, two
implementations are presented. The first one stores nonzero
elements diagonal by diagonal, and the second one stores them row by row.<br>

**Diagonal 1:** The first diagonal format stores diagonals in
a two-dimensional array that each row contains one diagonal. So, the row
numbers of this matrix is equal to number of nonzero diagonals of
the banded matrix. Figure 1 can be considered as a banded matrix and
its corresponding diagonal format is shown below. *dd* is
the array that stores diagonals. we store nonzero elements (diagonals)
in *dd*. Size of *Offset* shows the number of
diagonals. *offset[i]=0* is the main diagonal.
*offsset[i]<0* is up-diagonal, and *offset[i]>0* is sub-diagonal.<br>
dd:
<br>
![sparse matrix](https://github.com/leila68/Axb/blob/master/doc/dd.png "mtx")
<br>

offset:
<br>
![sparse matrix](https://github.com/leila68/Axb/blob/master/doc/offset.png "mtx")
<br>

*Note:* to solve a linear system of the form *Ly = d*, *L* should be a
lower triangular matrix. So we only store main diagonal, and sub-diagonals for a lower triangular banded matrix.
*i=0* means main diagonal and for main diagonal we know row and column is the same, and for sub-diagonals *row>column*.<br>

**Diagonal 2:** Second format that is suggested for storing a banded matrix,
processes matrix row by row and stored every nonzero elements of banded matrix in *dm*. 
So, nonzero elements of the first row are stored in *dm[0,*]*, and *dm[1,*]* contains nonzero elements of second row.

dm:
<br>
![sparse matrix](https://github.com/leila68/Axb/blob/master/doc/dm.png "mtx")

### 2- Kernel Variants
Sparse kernels provide a different performance depending on
the sparse storage format they use. In this section, we use
two sparse kernels, sparse matrix-vector multiplication (SpMV)
and sparse lower triangular solver (SpTRSV) and show how
they should be redesigned for the four different storage formats
shown in Section 1.

#### 2-1- Sparse Matrix – Vector Multiplication (SpMV)
SpMV kernel computes *y = A\* x* where *A* is a sparse kernel and *x* and *y*
are dense vectors. *A* can be stored in any of the four formats are introduced here.
We will explain how SpMV code differs using different formats.

**CSR:** the code shown down in *Listing 1*, is the CSR variant of SpMV.
The SpMV CSR code iterates over rows and computes each row of the *result*
vector one by one in order. Since all elements of a row are stored
contiguously, each iteration *i* of SpMV CSR computes *result[i,0]*.
```
  for (int i=0; i<row; i++)
    {
        for (int j=ptr[i]; j<ptr[i+1]; j++ )
        {
            s = val[j] * v->array[idx[j]][0] + s;
        }
       result->array[i][0] = s;
        s = 0;
    }
 ``` 
<div align="center"> Listing 1: CSR variant of SpMV </div>
<br>
**CSC:** The code shown in *Listing 2* is the CSC variant of SpMV.
The SpMV CSC code iterates over columns and computes the partial multiplication
of each element in *result*. For this sequential implementation, the computed solution in *result* is
final when all iterations are finished. This is the opposite of the SpMV CSR code where each iteration *i* computes
*result[i,0]*.

```
 for (int i=0; i<col; i++ ) 
 {
  for (int j=ptr[i]; j<ptr[i+1]; j++)
    {
     result->array[idx[j]][0] += val[j]*v->array[i][0] ;

    }
  } 
 ```        
<div align="center"> Listing 2: the CSC variant of SpMV </div>
<br>
**Diagonal:** for diagonal storage format two implementations are introduced.
The code in *Listing 3* shows the implementation of SpMV for the first diagonal
format. As shown, the code iterates over each diagonal that is stored
in *d* and computes the partial result of each element in *result*.
To compute what element in *result* should be updated, the diagonal
information in *offset* is used.

```
   for (int l = 0; l < d->rowNo; l++) {
         for (int k = 0; k < d->colNo - abs(offset[l]); k++) {
             if (offset[l] > 0) {
                 i = k + offset[l];
                 j = k;

                 result->array[i][0] += d->array[l][k] * v->array[j][0];
             }
             if (offset[l] == 0) {
                 i = k;
                 j = k;
                 result->array[i][0] += d->array[l][k] * v->array[j][0];
             }
             if (offset[l] < 0) {
                 i = k;
                 j = k - offset[l];

                 result->array[i][0] += d->array[l][k] * v->array[j][0];
             }
         }
     }
 ``` 
<div align="center"> Listing 3: the diagonal1 variant of SpMV </div>
<br>
For the second diagonal format, the code for SpMV is shown in *Listing 4*.
As shown, the code computes one element of *result* in each iteration
because we stored nonzero elements row by row in *d*, i.e. *d[i,*]*
contains the elements of row *i* . But to find the column number we need
to check whether *i* is bigger or smaller than
the number of diagonals (d1). Depending on whether the nonzero element is
on the main diagonal, sub-diagonal, or up-diagonal, three different
column index calculation is used as shown in *Listing 4*.

*Note:* d1 = dNum/2 because in this case, we just considered main
and sub-diagonals.

```
  int d1 = dNum/2; // dNum stores the number of diagonals

     for(int i=0; i<row; i++)
     {
         if(i<=d1)
         {
             for(int j=0; j<=i ; j++)
             {
                 result->array[i][0] += (d->array[i][j]*v->array[j][0]);
             }
         }
         if(i>d1)
         {
             for (int j = i; j >= j-d1; j--)
             {
                 result->array[i][0] += (d->array[i][d1]*v->array[j][0]);
                 d1--;
             }
             d1=dNum/2;
         }
     }
 ```
<div align="center"> Listing 4: the diagonal2 variant of SpMV </div>

#### 2-2- Sparse Triangular Solve:For Different Formats
We have the equation *Ly = d* in which *L* is lower triangular matrix that is
stored in CSC, CSC, or a diagonal format, *y* is the unknown
vector and *d* is the known right hand side vector.
In the following, we explain how we solve this equation
in different sparse storage formats for *L*.

**CSR:** *L* is a sparse matrix in CSR storage format. In this format,
shown in *Listing 5*, we compute all unknowns in order.
It means we first compute *y[0,0]* then we use that to compute *y[1,0]*
and we continue until we find the last unknown *y[rowNo-1,0]*.
Each iteration of *i* computes *y[i,0]*.

```
   y->array[0][0] = d->array[0][0] / L->val[0];
    for (int i=1; i<rowNo; i++)
    {
        for (int j=L->ptr[i]; j<L->ptr[i+1]-1; j++ )
        {
            s = (L->val[j] * y->array[L->idx[j]][0]) + s;

        }
        y->array[i][0] = (d->array[i][0] - s) / L->val[L->ptr[i+1]-1];
        s = 0;
    }
 ```
<div align="center"> Listing 5: solving equation in CSR format </div>
<br>
**CSC:** In CSC format, shown in *Listing 6*, we access elements of *L*
column by column. For simplicity, we first copy the right-hand-side *d* into *y*.
In order to find *y[i,0]*, all elements of row *i* should be accessed,
so we should process all columns that have a nonzero element in row *i*.
In this sequential implementation, the final solution *y* is achieved after
all iterations are done.

```

    for (int i=0; i<rowNo; i++)
      {
            y->array[i][0] /=  L->val[L->ptr[i]];
            for (int j=L->ptr[i]+1; j<L->ptr[i+1]; j++ )
            {
                y->array[L->idx[j]][0] -=  L->val[j]*y->array[i][0];
            }
      }
 ```
<div align="center"> Listing 6: solving equation in CSC format </div>
<br>
**Diagonal1:** The first diagonal format is very inefficient for SpTRSV
because the code should iterate over columns or rows and thus row or col
indices should be computed separately at the beginning from the diagonal
elements. Therefore, it is excluded when comparing with other variants
of SpTRSV.

**Diagonal2:** second format that is suggested for storing the matrix in
diagonal format, shown in *Listing 7*, is different.
In this format, because we stored nonzero elements of banded matrix
row by row, we use an implementation like CSR format.
this implementation is more efficient than diagonal1.
Because spatial locality in accessing nonzero elements is better than diagonal1.
This is because for each iteration *row i* we can compute *y[i,0]*.
Of course, Before the execution of inner for, we check whether
*d1* is bigger or smaller than *i*, because we have two different
implementations to compute column number and  *y[i,0]*
in each condition.

```
    int d1 = dia/2; // dia is the number of diagonals

    for(int i=0; i<rowNo; i++)
    {
        if(i<=d1)
        {
            for(int j=0; j<i ; j++)
            {
               s += L->array[i][j]*y->array[j][0];
            }
          y->array[i][0] = (d->array[i][0] - s) / L->array[i][i];

        }
         if(i>d1)
        {
            for (int j = i-1; j > j-d1; j--)
            {
                d1--;
                s += L->array[i][d1]*y->array[j][0];                
            }
          d1=dia/2;
          y->array[i][0] = (d->array[i][0] - s) / L->array[i][d1];
        }
        s = 0;
    }
 ```
<div align="center"> Listing 7: solving equation in diagonal2 format </div>

### 3- Experimental Results
In this section, we evaluate the two kernels SpMV and SpTRSV for four
different sparse storage formats.
We select a set of sparse matrices from the Suitesparse matrix repository
as shown in *Table 1* to compare the performance of CSC and CSR variants
of SpMV and SpTRSV.
To compare the efficiency of diagonal formats with CSC and CSR, we generated
random banded matrices with dimensions of 100-50000 with 5 nonzero diagonals.

ID | Name | Row Number | Column Number | Non-Zero Number
  :--- | --- | :---: | :---: | :---: 
 1 |cbuckle |13681 |13681 |676515
 2 |Dubcova2 |65025|65025 |1030225 
 3 |Dubcova3 |146689 |146689 |3636643
 4 |ecology2 |999999 |999999 |4995991
 5 |gyro |17361 |17361 |1021159
 6 |gyro_k |17361 |17361 |1021159
 7 |LFAT5.mtx |14 |14 |46
 8 |msc23052 |23052 |23052 |1142686
 9 |olafu |16146 |16146|1015156
 10 |parabolic_fem |525825 |525825 |3674625
 11 |Pres_Poisson |14822|14822 |715804
 12 |raefsky4 |19779 |19779 |1316789
 13 |thermomech_dM|204316 |204316|1423116
 14 |tmt_sym |726713|726713 |5080961

<div align="left"> Table 1: List of matrices </div>


#### 3-1- SpMV Performance
Figure 2 shows the performance of SpMV for both CSC and CSR storage formats.
As shown, because the CSR variant has fewer write operations than the
CSC variant, consistently shows a better performance. The CSR variant
is on average 1.08 times faster than the CSC variant.
<br>
![graph1](https://github.com/leila68/Axb/blob/master/doc/graphM2.png "graph1")

<div align="left"> Figure 2: The performance CSR, CSC variants of SpMV  </div>
<br>

Figure 3 shows the performance of the diagonal variants of SpMV compared with
CSC and CSR variants for the randomly generated banded matrices.
As shown, the diagonal2 variant has the best performance because of
its spatial locality and due to its compact storage format.
<br>

![graph1](https://github.com/leila68/Axb/blob/master/doc/graphM1.png "graph1")

<div align="left"> Figure 3: The performance CSR, CSC, diagonal1 and diagonal2 variants of SpMV  </div>
<br>

#### 3-2- SpTRSV performance
Figure 4 shows the performance of SpTRSV for both CSC and CSR. As shown,
the performance of SpTRSV follow a similar trend to SpMV where the
CSR variant outperforms the CSC one in most matrices.<br>

![graph1](https://github.com/leila68/Axb/blob/master/doc/graph1.png "graph1")

<div align="left"> Figure 4: The performance of triangular solve using CSR and CSC formats  </div>
<br>
Figure 5 compares the performance of the diagonal2 variant comparing to
the CSC and CSR code for diagonal matrices. Diagonal 1 is excluded due
to its inefficiency for SpTRSV. While CSR and diagonal2 show a competitive
performance, diagonal2 is on average 1.2 times better than the CSR
variant due to its smaller number of memory accesses (because of its
compact storage format). <br>


![graph1](https://github.com/leila68/Axb/blob/master/doc/graph2.png "graph1")

<div align="left"> Figure 5: The performance of triangular solve using CSR and CSC and Diagonal2 formats </div>
<br>