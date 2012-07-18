

#include <math.h>
#include <float.h>
#include "sparseConstraints.h"
#include "solve.h"

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
    
    double ax=0;

    for ( int j=0; j<nrag; j++){
        ax += ak[j] * x[I[j]];
        wa[j] = w[I[j]] * ak[j];
    }
    int alpha_old = alpha[k];
    alpha[k] = alpha[k] + (ax - E->b[k])/awa;
    alpha[k] = alpha[k] < 0 ? alpha[k] : 0;
    
    for ( int j=0; j<nrag; j++ ){
        x[I[j]] -= wa[j]*(alpha_old - alpha[k]);
    }

}

// computes ||x-y||_Inf
static double maxdist(double *x, double *y, int n){
    double t, m = fabs(x[0]-y[0]);
    
    for ( int j=1; j<n; j++ ){ 
        t = fabs(x[j]-y[j]);
        m = t > m ? t : m;
    }
}


// successive projection algorithm
void solve_sc_spa(SparseConstraints *E, double *x, double *w, double *tol){
    
    int m = E->nedits;
    int n = E->nvar;
    int neq = E->neq;
    double awa[m];
    double xt[n];    
    int nrag;
    double alpha[m];


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

    for ( int i=0; i<n; xt[i]=x[i] );
    double diff = DBL_MAX;
    while ( diff > tol[0] ){
        for ( int k=0; k<neq; k++ ) update_x_k_eq(E, x, w, awa[k], k);
        for ( int k=neq; k<m; k++ ) update_x_k_in(E, x, w, alpha, awa[k], k);
        diff = 0;
        for ( int j=0; j<n; j++ ){
            diff += (xt[j]-x[j])*(xt[j]-x[j]);
        }
        diff = maxdist(xt, x, n);
        for (int j=0; j<n; xt[j++] = x[j]);
    }
    tol[0] = diff;

}





