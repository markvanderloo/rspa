

// R interface functions to SparseConstraints

#include <R.h>
#include <Rdefines.h>
#include "R_sparseConstraints.h"
#include "sparseConstraints.h"

void R_sc_del(SEXP p){
    if (!R_ExternalPtrAddr(p)) return;
    sc_del(R_ExternalPtrAddr(p));
    R_ClearExternalPtr(p);
}


static void R_print_sc_row(SparseConstraints *x, int i, SEXP names){
    char op;
    int n = x->nrag[i]-1;
    double b;
    op = i < x->neq ? '=' : '<';

    for (int j=0; j < n; j++){
        Rprintf("%g*%s + ", x->A[i][j], CHAR(STRING_ELT(names,x->index[i][j])) );
    }
    // prevent -0 printing
    b = b == 0.0 ? 0.0 : b;    
    Rprintf("%g*%s %.1s %g\n",x->A[i][n], CHAR(STRING_ELT(names,x->index[i][n])), &op , b);

}

SEXP R_print_sc(SEXP p, SEXP names, SEXP maxprint){
    PROTECT(p);
    PROTECT(names);
    PROTECT(maxprint);
    int mp = INTEGER(maxprint)[0];
    SparseConstraints * xp = R_ExternalPtrAddr(p);
    if (!xp){
        Rprintf("NULL pointer\n");
        return R_NilValue;
    }

    mp = mp < xp->nedits ? mp : xp->nedits;
    Rprintf("Sparse numerical constraints.\n");
    Rprintf("  Variables   : %d\n",xp->nvar);
    Rprintf("  Restrictions: %d (printing %d)\n",xp->nedits, mp);


    for ( int i =0; i < mp; i++){
       R_print_sc_row(xp, i, names); 
    }
    UNPROTECT(3);
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


// Create ragged array (sparse) representation from row-col-coefficient-b representation.
SEXP R_sc_from_sparse_matrix(SEXP rows, SEXP cols, SEXP coef, SEXP b, SEXP neq ){
    PROTECT(rows);
    PROTECT(cols);
    PROTECT(coef);
    PROTECT(b);
    PROTECT(neq);

    SparseConstraints *E;

    E = sc_from_sparse_matrix(
        INTEGER(rows), 
        INTEGER(cols), 
        REAL(coef),
        length(rows),
        REAL(b),
        length(b),
        INTEGER(neq)[0]
    );

    SEXP ptr = R_MakeExternalPtr(E, R_NilValue, R_NilValue);
    PROTECT(ptr);
    R_RegisterCFinalizerEx(ptr, R_sc_del, TRUE);

    UNPROTECT(6);

    return ptr;
} 








