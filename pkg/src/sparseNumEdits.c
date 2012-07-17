
#include <stdlib.h>
#include <math.h>
#include "sparseNumEdits.h"


SparseNumEdits * sne_new( int n){
    
    SparseNumEdits *E;
    E = (SparseNumEdits *) calloc(1, sizeof(SparseNumEdits));
    E->nedits = n;
    E->A = (double **) calloc(E->nedits, sizeof(double *));
    E->index = (int **) calloc(E->nedits, sizeof(int *));
    E->nrag = (int *) calloc(E->nedits, sizeof(int));
    E->b = (double *) calloc(E->nedits, sizeof(double));
    return E;
}

void sne_del(SparseNumEdits *E){
    free(E->b);
    free(E->nrag);
    for ( int i = 0; i < E->nedits; i++){
        free(E->A[i]);
        free(E->index[i]);
    } 
    free(E);
}


SparseNumEdits * sne_from_matrix(double *A, double *b, int m, int n, double tol){
    int nrag, jrag;

    SparseNumEdits *E = sne_new(m);

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





