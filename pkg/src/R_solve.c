
#include <R.h>
#include <Rdefines.h>
#include "sparseConstraints.h"
#include "solve.h"


SEXP R_solve_sc_spa(SEXP p, SEXP x, SEXP w, SEXP tol){

    SparseConstraints *xp = R_ExternalPtrAddr(p);

    PROTECT(x);
    PROTECT(w);
    PROTECT(tol);

    solve_sc_spa(xp, REAL(x), REAL(w) , REAL(tol)); 
    Rprintf("Achieved accuracy: %g\n", REAL(tol)[0]);
    UNPROTECT(3);
    return x;
}



