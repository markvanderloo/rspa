
#include <stdlib.h>
#include <math.h>
#include "sparseConstraints.h"


SparseConstraints * sc_new( int m){
    
    SparseConstraints *E;
    E = (SparseConstraints *) calloc(1, sizeof(SparseConstraints));
    E->nedits = m;
    E->A = (double **) calloc(E->nedits, sizeof(double *));
    E->index = (int **) calloc(E->nedits, sizeof(int *));
    E->nrag = (int *) calloc(E->nedits, sizeof(int));
    E->b = (double *) calloc(E->nedits, sizeof(double));
    return E;
}

void sc_del(SparseConstraints *E){
    free(E->b);
    free(E->nrag);
    for ( int i = 0; i < E->nedits; i++){
        free(E->A[i]);
        free(E->index[i]);
    } 
    free(E);
}


SparseConstraints * sc_from_matrix(double *A, double *b, int m, int n, int neq, double tol){
    int nrag, jrag;

    SparseConstraints *E = sc_new(m);
    E->neq = neq;
    E->nvar = n;
    for ( int i=0; i < E->nedits; i++){
        E->b[i] = b[i];
        nrag=0;
        jrag=0;      
        for ( int j=0; j < n; j++ ){
            if ( fabs(A[i + m*j]) > tol ) ++nrag;
        }
        E->nrag[i] = nrag;
        E->A[i] = (double *) calloc(nrag, sizeof(double));
        E->index[i] = (unsigned int *) calloc(nrag, sizeof(unsigned int));
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





