

// R interface functions to SparseConstraints

#include <R.h>
#include <Rdefines.h>
#include "sparseConstraints.h"

void R_sc_del(SEXP p){
    if (!R_ExternalPtrAddr(p)) return;
    sc_del(R_ExternalPtrAddr(p));
    R_ClearExternalPtr(p);
}


void R_print_sc_row(SparseConstraints *x, int i, SEXP names){
    char op;
    int n = x->nrag[i]-1;

    op = i < x->neq ? '=' : '<';

    for (int j=0; j < n; j++){
        Rprintf("%g*%s + ", x->A[i][j], CHAR(STRING_ELT(names,x->index[i][j])) );
    }
    Rprintf("%g*%s %.1s %g\n",x->A[i][n], CHAR(STRING_ELT(names,x->index[i][n])), &op ,x->b[n]);

}

SEXP R_print_sc(SEXP p, SEXP names){
    PROTECT(p);
    PROTECT(names);
    SparseConstraints * xp = R_ExternalPtrAddr(p);
    if (!xp){
        Rprintf("NULL pointer\n");
        return R_NilValue;
    }
    Rprintf("Sparse numerical constraints.\n");
    Rprintf("  Restrictions: %d\n",xp->nedits);
    Rprintf("  Variables   : %d\n",xp->nvar);

    for ( int i =0; i < xp->nedits; i++){
       R_print_sc_row(xp, i, names); 
    }
    UNPROTECT(2);
    return R_NilValue;
}



// Create ragged array (sparse) representation from dense matrix rep Ax=b.
SEXP R_sc_from_matrix(SEXP A, SEXP b, SEXP neq, SEXP tol){

    double *Amat = REAL(A);
    SparseConstraints *E;

    SEXP dim; 
    dim = getAttrib(A, R_DimSymbol);
    int m=INTEGER(dim)[0];
    int n=INTEGER(dim)[1];
    int neqs=INTEGER(neq)[0];

    E = sc_from_matrix(REAL(A), REAL(b), m, n, neqs, REAL(tol)[0]);

    SEXP ptr = R_MakeExternalPtr(E, R_NilValue, R_NilValue);
    PROTECT(ptr);
    R_RegisterCFinalizerEx(ptr, R_sc_del, TRUE);
    UNPROTECT(1);

    return ptr;
}






