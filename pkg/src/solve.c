

#include <math.h>
#include <float.h>
#include "sparseConstraints.h"
#include "solve.h"

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
   //Rprintf("k = %d\n",k); 
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
     alpha[k] = alpha[k] - (E->b[k] - ax)/awa;
      // eqn from paper but wrong by a minus sign.
    //alpha[k] = alpha[k] + (ax - E->b[k])/awa;
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

// successive projection algorithm
void solve_sc_spa(SparseConstraints *E, double *x, double *w, double *tol, int *maxiter ){
    
    int m = E->nedits;
    int n = E->nvar;
    int neq = E->neq;
    double awa[m];
    double xt[n];    
    int nrag;
    double *alpha = (double *) calloc(m,sizeof(double));
    int niter = 0;

    // we only need w's inverse.
    for ( int k=0; k < m; w[k++] = 1/w[k]);
    // determine inner products A'W^(-1)A
    for ( int k=0; k < m; k++){
        awa[k] = 0;
        nrag = E->nrag[k];
        for ( int j=0; j<nrag; j++){
            awa[k] += E->A[k][j] * w[E->index[k][j]] * E->A[k][j];
        }
    }

    for ( int i=0; i<n; xt[i++]=x[i] );
    double diff = DBL_MAX;
    
    while ( diff > tol[0] && niter < maxiter[0] ){
        for ( int k=0; k<neq; k++ ) update_x_k_eq(E, x, w, awa[k], k);
        for ( int k=neq; k<m; k++ ) update_x_k_in(E, x, w, alpha, awa[k], k);
        diff = maxdist(xt, x, n);
        for (int j=0; j<n; xt[j++] = x[j]);
        ++niter;
    }
    *tol = diff;
    *maxiter = niter;
    free(alpha);
}





