

// R interface functions to SparseNumEdits

#include <R.h>
#include <Rdefines.h>
#include "sparseNumEdits.h"

static void R_sne_del(SEXP p){
    if (!R_ExternalPtrAddr(p)) return;
    sne_del(R_ExternalPtrAddr(p));
    R_ClearExternalPtr(p);
}


SEXP R_print_sne(SEXP p){
    SparseNumEdits * xp = R_ExternalPtrAddr(p);
    Rprintf("Sparse numerical edits object.\nRestrictions: %d\n",xp->nedits);

    for ( int i =0; i < xp->nedits; i++){
        for( int j=0; j < xp->nrag[i]; j++){
            Rprintf("%4f*x%d + ", xp->A[i][j], xp->index[i][j]);
        }
        Rprintf(" >= %4f\n",xp->b[i]);
    }
    return R_NilValue;
}


// Create ragged array (sparse) representation from dense matrix rep Ax=b.
SEXP R_sne_from_matrix(SEXP A, SEXP b, SEXP tol){

    double *Amat = REAL(A);
    SparseNumEdits *E;

    SEXP dim; 
    dim = getAttrib(A, R_DimSymbol);
    int m=INTEGER(dim)[0];
    int n=INTEGER(dim)[1];

    E = sne_from_matrix(REAL(A), REAL(b), m, n, REAL(tol)[0]);

    SEXP ptr = R_MakeExternalPtr(E, R_NilValue, R_NilValue);
    PROTECT(ptr);
    R_RegisterCFinalizerEx(ptr, R_sne_del, TRUE);
    UNPROTECT(1);

    R_print_sne(ptr);

    return ptr;
}




