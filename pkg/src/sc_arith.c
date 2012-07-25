
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
   for ( int i=0; i<E->nconstraints; diff[i++] -= E->b[i] );
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
      d += dt < E->b[i] ? 0 : dt-E->b[i];
   }
   
   return d;
}


double sc_diffmax(SparseConstraints *E, double *x){
   double d, dmax=0;

   for( int i=0; i<E->neq; i++ ){
      d = fabs(sc_row_vec(E, i, x) - E->b[i]);
      dmax = (d > dmax) ? d : dmax;
   }
   for ( int i=E->neq; i<E->nconstraints; i++ ){
      d = fabs(sc_row_vec(E, i, x) - E->b[i]);
      d = d < 0.0 ? 0.0 : d;
      dmax = d > dmax ? d : dmax;

   }
Rprintf("returning %g\n",dmax);
   return dmax;
}


