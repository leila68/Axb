# Axb

---
### Sparse Storage Formats
<p style='text-align: justify;'>
There are several compressed formats for storing sparse matrices. Four different format are explained in this section. 
compressed sparse row (CSR), compressed sparse column(CSC), and two different diagonal format. To illustrate dofferent 
formats, an example shown in Figure1 is used.
<br>

![sparse matrix](https://github.com/leila68/Axb/blob/master/doc/mtx.png "mtx")
<br>

Figure 1: a sparse matrix with a rows and column (4*4) and 7 non-zero elements

**CSR:** Storing the non-zero elements of a sparse matrix into a linear array *val* is done by going over each row in
order,and storing the non-zero elements to a linear array in the order they appear in the walk. Location and column
indices of non-zero elements of the *val* array in order stored in *row_ptr* and *col_idx*. The first element of row *i* 
is stored *val[row_ptr[i]]* and the column index of the non-zero element in *val[i]* is stored in *col_idx*. <br>
val:[1,3,5,4,6,2,7]<br>
col_ptr:[0,2,4,6,7]<br>
row_idx:[0,1,1,2,2,3,3]<br>
For the matrix shown in Figure 1, the non-zero element 5 in row 1 and column 1 is stored in *val[row_ptr[1]+1]* and its 
column index is stored in *col_idx[row_ptr[1]+1]* where one is the offset of the element from the first element in row one.

**CSC:** Storing the non-zero elements of a sparse matrix into a linear array is done by going over each column in order,
and writing the non-zero elements to a linear array in the order they appear in the walk. Location and row indices of 
non-zero elements of the *val* array are in order stored in *col_ptr* and *row_idx*. The first element of column *i* is stored
in val[col_ptr[i]] and the row index of the non-zero elements in *val[j]* is stored in *row_idx[j]*.
<br>
val:[1,3,5,4,6,2,7]<br>
col_ptr:[0,2,4,6,7]<br>
row_idx:[0,1,1,2,2,3,3]<br>

For the matrix in Figure 1, the non-zero element 5 in column 1 and row 1 is stored in *val[col_ptr[1]+0]* and its row 
index is stored in *row_idx[col_ptr[1]]+0* where zero is the offset of the element from the first column.<br>

**Diagonal Formats:** Since in a banded matrix, non-zero elements are limited to diagonal bands, we only need to store 
elements based on their diagonals. For this storage format, two implementations are presented. First one stores non-zero 
elements diagonal by diagonal, and second one stores them row by row.<br>

**Diagonal 1:** First format stores diagonals in a two dimensional array that each row contains one diagonal. So, the row
numbers of this matrix is equal to number of non-zero diagonals od banded matrix. Figure 2 shows a banded matrix. *dNum*
 is the number of diagonals. *dd* is the array that stores diagonals. we store non-zero elements (diagonals) in *dd*. 
Size of *Offset* shows the number of diagonals. *offset[i]=0* is main diagonal. *offsset[i]<0* is up-diagonal, and *offset[i]>0*
is sub-diagonal.<br>
dd:
<br>
![sparse matrix](https://github.com/leila68/Axb/blob/master/doc/dd.png "mtx")
<br>

offset:
<br>
![sparse matrix](https://github.com/leila68/Axb/blob/master/doc/offset.png "mtx")
<br>

*Note:* to solve s linear system of the form *Ly = d*, *L* should be a lower triangular matrix. So we just store main 
diagonal and sub-diagonals. *i=0* means main diagonal and for main diagonal we know row and column is the same.
For sub-diagonals *row>column*.<br>

Diagonal2: Second format that is suggested for storing a banded matrix, processes matrix row by row and stored every 
non-zero elements of banded matrix in dm. So, in *dm[0,*]* is stored non-zero elements of the first row of banded matrix.
*dm[1,*]* contains non-zero elements of second row of banded matrix ,…
<br>
dm:
![sparse matrix](https://github.com/leila68/Axb/blob/master/doc/dm.png "mtx")

### Kernel Variants
Sparse kernels provide a different performance depending on the sparse storage format they use. In this section, we use 
two sparse kernels, sparse lower triangular solver (SpTRSV) and sparse matrix vector multiplication (SpMV) and show how 
they should be redesigned for the four different storage formats shown in Section 1.  

#### Sparse Matrix – Vector Multiplication (SpMV)
SpMV kernel computes y = A*x  where *A* is a sparse kernel and *x* and *y* are dense vectors. *A* can be stored in any of the 
four formats introduced here. We will explain how SpMV code differ using different formats. 

**CSR:** According figure1, In CSR storage format, val[0] contains element of row(0), and val[1] and val[2] contains elements
of row(1) and so on. Therefore, we process val one by one and multiply every element with corresponding value of vector 
to compute result.

**CSC:** In CSC storage format, we process column by column. So unlike CSR format we don’t have access to rows by order. 
It causes CSC slower than CSR. As it was explained in section 1, val[0] and val[1] contains elements of first column but
row number of val[0] is zero and val[1] is 1.

**Diagonal:** for diagonal storage format two implementation are introduced. In first one we need to set offset which we 
initialize it when we were storing the sparse matrix. If offset[l]=0, it means dd[l,*] contains main diagonal elements. 
And if offset[l]>0 sub-diagonal values were stored in dd[l,*]. So, according offset values we can calculate row and column.
According to figure2, first we process dd [0,*] that contains value of main diagonal and find row of every value and then
multiply that with corresponding element of vector. In the first row of matrix dd , 1 is the first element. Its row is 0.
so we multiply dd[0,0] with the value of first row of vector to compute y0. We continue until last row of dd.
For Second format we have access to non-zero elements of each row by storing them in dm. So, we process that row by row 
and multiply each element by corresponding value of vector.

####  Sparse Triangular Solve:For Different Formats
sparse linear system -> Ly = d
“L” is lower triangular matrix in CSC, CSC or Diagonal format. y is unknowns vector. d is ….

**CSR:** L is a sparse matrix in CSR storage format and y is unknowns. In this format, we compute all unknowns by order.
It means we first compute y0 then we use that to calculate y1 and we continue until we find the last unknown.
There is a nested loop  that goes row by row and for each row compute corresponding y.

**CSC:** In CSC format we access elements column by column and copy d into y. In order to find yi we should have access to
all elements of row i, so should process all column to find that. So we can compute each y until we finish the process of
all column entirely. 

**diagonal1:** To solve a system of linear equation when the sparse matrix is stored in diagonal format, first we copy d into
unknowns(y) vector. Then we need to find row index. By having row index we can find corresponding y and then compute yi
for every row(i).

**Diagonal2:** But second format that is suggested for storing the matrix in diagonal format is different. In this way,
because we stored non-zero elements of diagonals row by row, we use an implementation like CSR format.
this implementation is more efficient than diagonal1. Because special locality for finding y is better than diagonal1.
This is because we don’t have to process all columns every time that we want to find yi.


<p>