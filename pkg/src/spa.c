
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "sparseConstraints.h"
#include "spa.h"
#include "sc_arith.h"
#include "maxdist.h"



static void update_x_k(SparseConstraints *E, double *x, double *w, double *wa, double *alpha, double awa, int k, double *conv){
   
   double *ak = E->A[k];
   int *I = E->index[k];
   int nrag = E->nrag[k];
   
   double ax = 0;
   for ( int j=0; j<nrag; j++ ){
      ax += ak[j] * x[I[j]];
      wa[j] = w[I[j]] * ak[j];
   }

   conv[k] = (ax - E->b[k])/awa;

   double fact = conv[k];
   if ( k >= E->neq ){ // are we at an inequation?
      double alpha_old = alpha[k];
      alpha[k] +=  conv[k];
      if ( alpha[k] < 0 ){
         alpha[k] = 0;
      }
      fact = alpha[k] - alpha_old;
   }
   
   for( int j=0; j < nrag; j++ ){
      x[I[j]] -= wa[j]*fact;
   }
   
}
// successive projection algorithm
int solve_sc_spa(SparseConstraints *E, double *w, double *tol, int *maxiter, double *x  ){

   int m = E->nconstraints;
   int n = E->nvar;
   int neq = E->neq;

   int nrag;
   int niter = 0;
   double *awa = (double *) calloc(m, sizeof(double));
   double *xw = (double *) calloc(n, sizeof(double));
   double *alpha = (double *) calloc(m, sizeof(double));
   double *conv = (double *) calloc(m, sizeof(double));

   int maxrag = get_max_nrag(E);
   double *wa = (double *) calloc(maxrag, sizeof(double));

   // cleanup in case of emergency...
   if ( awa == 0 ||  xw == 0|| alpha == 0 || conv==0 || wa == 0 ){ 
      free(awa); free(xw); free(alpha); free(conv); free(wa);
      return 1;
   }

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


   double diff=DBL_MAX;

   while ( diff > tol[0] && niter < maxiter[0] ){

      for ( int k=0; k<m; k++ ) update_x_k(E, x, xw, wa, alpha, awa[k], k, conv);
      ++niter;

      if ( diverged(x,n) || diverged(alpha,m) ){
         exit_status = 2; 
         break;
      }
		// convergence criterion
		diff = absmax(conv, awa, E->neq, E->nconstraints); 

   }
   // number of iterations exceeded without convergence?
   if (exit_status != 2 && niter == maxiter[0] && diff > tol[0] ) exit_status = 3;

   *tol = sc_diffmax(E,x); // actual difference in current vector
   *maxiter = niter;
   free(wa); free(awa); free(xw); free(alpha); free(conv);
    return exit_status;
}





