# Axb

---
### Sparse Storage Formats
<p style='text-align: justify;'>
There are several compressed formats for storing sparse matrices. Four different format are explained in this section. 
compressed sparse row (CSR), compressed sparse column(CSC), and two different diagonal format. To illustrate dofferent 
formats, an example shown in Figure1 is used.

![sparse matrix](doc/mtx.png "mtx")
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


<p>