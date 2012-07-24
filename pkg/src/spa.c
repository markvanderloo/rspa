
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "sparseConstraints.h"
#include "spa.h"
#include "sc_arith.h"
#include "maxdist.h"

// update equalities
static void update_x_k_eq(SparseConstraints *E, double *x, double *w, double *wa, double awa, int k){
    
    double *ak = E->A[k];
    int *I = E->index[k];
    int nrag = E->nrag[k];
    double fact;

    double ax = 0;
    for ( int j=0; j<nrag; j++ ){
        ax += ak[j] * x[I[j]];
        wa[j] = w[I[j]] * ak[j];
    }
    
    fact = (ax - E->b[k])/awa;

    for( int j=0; j < nrag; j++ ){
        x[I[j]] = x[I[j]] - wa[j]*fact;
    }
}

// update inequalities: alpha and x.
static void update_x_k_in(SparseConstraints *E, double *x, double *w, double *wa, double *alpha, double awa, int k){

   double *ak = E->A[k];
   int *I = E->index[k];
   int nrag = E->nrag[k];
   
   double alpha_old = alpha[k];
   double ax=0;

   for ( int j=0; j<nrag; j++){
      ax += ak[j] * x[I[j]];
      wa[j] = w[I[j]] * ak[j];
   }


   alpha[k] = alpha[k] + (ax - E->b[k])/awa;
   alpha[k] = alpha[k] > 0 ? alpha[k] : 0;
    
   double alphadiff = alpha_old - alpha[k];
   for ( int j=0; j<nrag; j++ ){
      x[I[j]] +=   wa[j]*alphadiff;
   }

}


/* successive projection algorithm
 *
 * inputs:
 *    E: constraints
 *    w: weights (must be positive, this is not checked)
 * in/out
 *    tol     : asked/achieved convergence, measured as inf-norm between successive iterations of x
 *    maxiter : allowed/ran number of iterations
 *    x       : vector to optimize / optimized vector
 *
 * exit codes:
 *  0 all's well
 *  1 memory allocation error; aborted
 *  2 divergence detected; aborted
 */
int solve_sc_spa(SparseConstraints *E, double *w, double *tol, int *maxiter, double *x  ){

   int m = E->nconstraints;
   int n = E->nvar;
   int neq = E->neq;

   int nrag;
   int niter = 0;
   double *awa = (double *) calloc(m, sizeof(double));
   double *xt = (double *) calloc(n, sizeof(double));     
   double *xw = (double *) calloc(n, sizeof(double));
   double *alpha = (double *) calloc(m, sizeof(double));

   int maxrag = get_max_nrag(E);
   double *wa = (double *) calloc(maxrag, sizeof(double));

   // cleanup in case of emergency...
   if ( awa == 0 || xt == 0 || xw == 0|| alpha == 0 || wa == 0 ){ 
      free(awa); free(xt); free(xw); free(alpha); free(wa);
      return 1;
   }

   double diff = DBL_MAX, diff0 = 0;
   int exit_status = 0;
   // we only need w's inverse.
   for ( int k=0; k < n; xw[k++] = 1.0/w[k] );
   // determine inner products A'W^(-1)A
   for ( int k=0; k < m; k++){
      awa[k] = 0;
      nrag = E->nrag[k];
      for ( int j=0; j<nrag; j++){
         awa[k] += E->A[k][j] * xw[E->index[k][j]] * E->A[k][j];
      }
   }

   for ( int i=0; i<n; xt[i++]=x[i] );


   while ( diff > tol[0] && niter < maxiter[0] ){
  
      diff0 = diff;
      for ( int k=0; k<neq; k++ ) update_x_k_eq(E, x, xw, wa, awa[k], k);
      for ( int k=neq; k<m; k++ ) update_x_k_in(E, x, xw, wa, alpha, awa[k], k);
      diff = maxdist(xt, x, n);
      // diff = sc_diffsum(E, x);
      for (int j=0; j<n; xt[j++] = x[j]);
      ++niter;
      if (diff > diff0 + *tol){ // divergence: no joy at all.
         exit_status = 2;
         break;
      }
   }
   *tol = diff;
   *maxiter = niter;
   free(wa);
   free(awa);
   free(xt);
   free(xw);
   free(alpha);
   return exit_status;
}





