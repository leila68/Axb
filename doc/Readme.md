# Axb

---
### Sparse Storage Formats
<p style='text-align: justify;'>
There are several compressed formats for storing sparse matrices. Four different format are explained in this section. 
compressed sparse row (CSR), compressed sparse column(CSC), and two different diagonal format. To illustrate dofferent 
formats, an example shown in Figure1 is used.

![Alt text]( D:\github\Axb\doc\mtx.png "mtx")
<br>
Figure 1: a sparse matrix

**CSR:** Storing the non-zero elements of a sparse matrix into a linear array *val* is done by going over each row in
order,and storing the non-zero elements to a linear array in the order they appear in the walk. Location and column
indices of non-zero elements of the *val* arrray in order stored in *row_ptr* and *col_idx*. The first element of row *i* is stored
*val[row_ptr[i]]* and the column index of the non-zero element in *val[i]* is stored in *col_idx*. For the matrix shown
in Figure 1, the non-zero element 5 in row 1 and column 1 is stored in *val[row_ptr[1]+1]* and its column index is stored 
in *col_idx[row_ptr[1]+1]* where one is the offset of the element from the first element in row one.<br>
Val:[1,3,5,4,6,2,7]<br>
col_ptr:[0,2,4,6,7]<br>
row_idx:[0,1,1,2,2,3,3]<br>

**CSC:** Storing the non-zero elements of a sparse matrix into a linear array is done by going over each column in order,
and writing the non-zero elements to a linear array in the order they appear in the walk. According to the Figure 1 we have:
<br>
val:[1,3,5,4,6,2,7]<br>
col_ptr:[0,2,4,6,7]<br>
row_idx:[0,1,1,2,2,3,3]<br>
<p>