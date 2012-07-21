
#include <stdlib.h>
#include <math.h>
#include "sparseConstraints.h"
#include "sc_arith.h"

// multiply row of constraints with vector
double sc_row_vec(SparseConstraints *E, int i, double *x){
   
   double ax=0;

   for ( int j=0; j<E->nrag[i]; j++) 
      ax += E->A[i][j]*x[E->index[i][j]];
  
   return ax;
}

// multiply constraints with vector
void sc_multvec(SparseConstraints *E, double *x, double *Ax){
   for ( int i=0; i<E->nconstraints; i++){
      Ax[i] = sc_row_vec(E, i, x);
   }
}


// compute difference vector Ax - b
void sc_diffvec(SparseConstraints *E, double *x, double *diff){
   sc_multvec(E,x,diff);
   for ( int i=0; i<E->nconstraints; diff[i++] -= E->b[i]);
}

/* compute sum over differences: sum(|Ax-b|).
 * for inequations: if diff_i = a_i.x - b < 0 then diff_i = 0
 */
double sc_diffsum(SparseConstraints *E, double *x){
   double d, dt;


   for ( int i=0; i<E->neq; i++ ) 
      d += fabs(sc_row_vec(E, i, x) - E->b[i]);

   for ( int i=E->neq; i<E->nconstraints; i++){
      dt = sc_row_vec(E, i, x) - E->b[i];
      d += dt < E->b[i] ? 0 : fabs(dt-E->b[i]);
   }
   
   return d;
}

// replace coefficient with zero if it matches a variable.
static int sc_subinrow(SparseConstraints *E,int row, int col, double val){

   for ( int j=0; j<E->nrag[row]; j++){ 
      if ( E->index[row][j] == col ){
         E->b[row] -= E->A[row][j] * val;
         E->A[row][j] = 0;
         return 1;
      }
   }
   return 0;
}

// 
int sc_substvalue(SparseConstraints *E, int col, double val){
   
   int nsub = 0;
   for ( int i=0; i<E->nconstraints; i++){
      nsub += sc_subinrow(E, i, col, val);
   }

   if ( nsub > 0 ){
      E->nvar -= 1;
   }

   return nsub;

}



