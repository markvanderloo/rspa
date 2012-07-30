

#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "maxdist.h"

static void update_x_k_eq(double *A, double *b, double *x, int m, int n, double *w, double *wa, double awa, int k, double *fact){

   double ax = 0;

   for ( int j=0; j<n; j++){
      ax += A[k + j*m] * x[j];
      wa[j] = w[j] * A[k + j*m];
   }

   fact[k] = (ax - b[k])/awa;
   for ( int j=0; j<n; x[j++] -= wa[j]*fact[k] );
}

static void update_x_k_in(double *A, double *b, double *x, int m, int n, double *w, double *wa, double *alpha, double awa, int k, double *fact){
   
   double alpha_old = alpha[k];

   double ax = 0;
   for ( int j=0; j<n; j++){
      ax += A[k + j*m] * x[j];
      wa[j] = w[j] * A[k + j*m];
   }

	fact[k] = (ax - b[k])/awa;

   alpha[k] = alpha[k] + fact[k];
   alpha[k] = alpha[k] > 0 ? alpha[k] : 0;

   double alphadiff = alpha_old - alpha[k];
   for ( int j=0; j<n; j++ ){
      x[j] +=   wa[j]*alphadiff;
   }
}



// optimal adjustments with dense constraints.
int dc_solve(double *A, double *b, double *w, int m, int n, int neq, double *tol, int *maxiter, double *x){
   
   int niter;

   double *awa = (double *) calloc(m, sizeof(double)); if (awa==0) return 1;
   double *xt = (double *) calloc(n, sizeof(double));     
   double *xw = (double *) calloc(n, sizeof(double));
   double *alpha = (double *) calloc(m, sizeof(double));
   double *wa = (double *) calloc(n, sizeof(double));
	double *conv = (double *) calloc(m, sizeof(double));

   // in case of emergency: wee haave too get autofhea (Schwartzenegger style)
   if ( awa == 0 || xt == 0 || xw == 0|| alpha == 0|| conv == 0 || wa == 0 ){ 
      free(awa); free(xt); free(xw); free(alpha); free(conv); free(wa);
      return 1;
   }
   double diff = DBL_MAX; 
   int exit_status = 0;
   
   // we only need w's inverse.
   for ( int k=0; k < n; xw[k++] = 1/w[k] );
   
   // determine diag(A'W^(-1)A)
   for ( int k=0; k < m; k++){
      awa[k] = 0;
      for ( int j=0; j<n; j++ ){
         awa[k] += A[k+j*m]*xw[j]*A[k+j*m];
      }
   }

   for ( int i=0; i<n; xt[i++]=x[i] );

	double div=DBL_MAX, div0;
   while ( diff > *tol && niter < *maxiter ){
		div0 = div;

      for (int k=0; k<neq; k++) update_x_k_eq(A, b, x, m, n, xw, wa, awa[k], k, conv);
      for (int k=neq; k<m; k++) update_x_k_in(A, b, x, m, n, xw, wa, alpha, awa[k], k, conv);
      ++niter;

		diff = absmax(conv, awa, neq, m);
 /*     div = maxdist(xt, x, n);
      if (div > div0 + *tol){ // there is no joy in divergence
         exit_status = 2;
         break; 
      }
      for ( int j=0; j<n; xt[j++] = x[j]);
*/
   }
  
   *tol = dc_diffmax(A, b, x, neq, m, n); // actual max abs diff.
   *maxiter = niter;
   free(wa);
   free(awa);
   free(xt);
   free(xw);
   free(alpha);
	free(conv);

   return exit_status;
}




