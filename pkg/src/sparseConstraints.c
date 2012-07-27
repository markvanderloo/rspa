
#include <stdlib.h>
#include <math.h>
#include "sparseConstraints.h"


SparseConstraints * sc_new( int m ){
    
   SparseConstraints *E;
   E = (SparseConstraints *) calloc(1, sizeof(SparseConstraints));
   E->nconstraints = m;
   E->A = (double **) calloc(E->nconstraints, sizeof(double *));
   E->index = (int **) calloc(E->nconstraints, sizeof(int *));
   E->nrag = (int *) calloc(E->nconstraints, sizeof(int));
   E->b = (double *) calloc(E->nconstraints, sizeof(double));

   if ( E == 0 || E->A == 0 || E->index == 0 || E->nrag == 0 || E->b == 0 ){
      return (SparseConstraints *) 0;
   } else {
      return E;
   }
}

void sc_del(SparseConstraints *E){
   for ( int i = 0; i < E->nconstraints; i++){
      free(E->A[i]);
      free(E->index[i]);
   }
   free(E->b);
   free(E->nrag);
   free(E->A);
   free(E->index);
   free(E);
}

// generate SparseConstraints object from dense matrix
SparseConstraints * sc_from_matrix(double *A, double *b, int m, int n, int neq, double tol){
   int nrag, jrag;

   SparseConstraints *E = sc_new(m);
   if ( E == 0 ) return E;
 
   E->neq = neq;
   E->nvar = n;
   for ( int i=0; i < E->nconstraints; i++){
      E->b[i] = b[i];
      nrag=0;
      jrag=0;      
      for ( int j=0; j < n; j++ ){
          if ( fabs(A[i + m*j]) > tol ) ++nrag;
      }
      E->nrag[i] = nrag;
      E->A[i] = (double *) calloc(nrag, sizeof(double));
      E->index[i] = (int *) calloc(nrag, sizeof(int));
      if ( E->A[i] == 0 || E->index[i] == 0 ){ // no joy
         E->nconstraints = i-1; 
         sc_del(E);
         return (SparseConstraints *)0;
      }
      for ( int j=0; j<n; j++ ){
         if ( fabs(A[i+m*j]) > tol ){
            E->index[i][jrag] = j;
            E->A[i][jrag] = A[i + m*j];
            ++jrag;
         }
      }
   }
   return E;
}




static int get_row_end(int *rows, int nrows, int row_start){
    int row_nr = rows[row_start];
    int row_end = row_start-1;
    while (row_end < (nrows-1) && rows[row_end+1] == row_nr) row_end++;
    return row_end;
}



// generate SparseConstraints from row-column-coefficient specifications
// It is assumed that  int *rows is sorted in increasing order!
SparseConstraints * sc_from_sparse_matrix(int *rows, int *cols, double *coef, int ncoef, double *b, int m, int neq ){

   SparseConstraints *E = sc_new(m);
   if ( E == 0 ) return E;

   int maxcol=0, k;
   int row_start = 0, row_nr; 
   int row_end = get_row_end(rows, ncoef, row_start);
   for ( int irow=0; irow < m; irow++){
      E->b[irow] = b[irow];
      row_nr = rows[row_start];
      row_end = get_row_end(rows, ncoef, row_start);
     
      E->nrag[irow]    = 1 + row_end - row_start;
      E->index[irow]   = (int *) calloc( E->nrag[irow], sizeof(int));
      E->A[irow]       = (double *) calloc( E->nrag[irow], sizeof(double));
      if ( E->A[irow] == 0 || E->index[irow] == 0 ){ // no memory = no joy 
         E->nconstraints = irow-1;
         sc_del(E);
         return (SparseConstraints *)0;
      }            

      k = 0;
      for ( int j=row_start; j <= row_end; j++){
         E->A[irow][k] = coef[j];
         E->index[irow][k] = cols[j];
         ++k;
         maxcol = cols[j] > maxcol ? cols[j] : maxcol;
      }
      row_start = row_end+1;
   } 
   
   E->neq = neq;
   E->nvar = maxcol+1;

   return E;

}

int get_max_nrag(SparseConstraints *E){
   int nmax = 0;
   for ( int i=0; i < E->nconstraints; ++i ){
      nmax = nmax < E->nrag[i] ? E->nrag[i] : nmax;
   }
}




