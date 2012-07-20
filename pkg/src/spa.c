

#include <math.h>
#include <float.h>
#include "sparseConstraints.h"
#include "spa.h"

#include <R.h>
// update equalities
static void update_x_k_eq(SparseConstraints *E, double *x, double *w, double awa, int k){
    
    double *ak = E->A[k];
    int *I = E->index[k];
    int nrag = E->nrag[k];
    double wa[nrag];
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
static void update_x_k_in(SparseConstraints *E, double *x, double *w, double *alpha, double awa, int k){

    double *ak = E->A[k];
    int *I = E->index[k];
    int nrag = E->nrag[k];
    double wa[nrag];
    
    double alpha_old = alpha[k];
    double ax=0;

    for ( int j=0; j<nrag; j++){
        ax += ak[j] * x[I[j]];
        wa[j] = w[I[j]] * ak[j];
    }


    alpha[k] = alpha[k] + (ax - E->b[k])/awa;
    alpha[k] = alpha[k] > 0 ? alpha[k] : 0;
    
    for ( int j=0; j<nrag; j++ ){
        x[I[j]] +=   wa[j]*(alpha_old - alpha[k]);
    }

}

// computes ||x-y||_Inf
static double maxdist(double *x, double *y, int n){
    double t, m = fabs(x[0]-y[0]);
    
    for ( int j=1; j<n; j++ ){ 
        t = fabs(x[j]-y[j]);
        m = t > m ? t : m;
    }
    return m;
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
    double awa[m];
    double xt[n];    
    double xw[n];
    double alpha[m]; 

    if ( alpha == 0 || awa == 0 || xt == 0 || xw == 0 ) return(1);

    double diff = DBL_MAX, diff0 = 0;
    int exit_status=0;

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
        for ( int k=0; k<neq; k++ ) update_x_k_eq(E, x, xw, awa[k], k);
        for ( int k=neq; k<m; k++ ) update_x_k_in(E, x, xw, alpha, awa[k], k);
        diff = maxdist(xt, x, n);
        for (int j=0; j<n; xt[j++] = x[j]);
        ++niter;
        if (diff > diff0){ 
           exit_status = 2;
           break;
        }
    }
    *tol = diff;
    *maxiter = niter;
    
    return exit_status;
}





