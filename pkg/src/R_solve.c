
#include <R.h>
#include <Rdefines.h>
#include "sparseConstraints.h"
#include "solve.h"


SEXP R_solve_sc_spa(SEXP p, SEXP x, SEXP w, SEXP tol, SEXP maxiter){

    SparseConstraints *xp = R_ExternalPtrAddr(p);

    PROTECT(x);
    PROTECT(w);
    PROTECT(tol);
    PROTECT(maxiter);

    SEXP niter, eps;
    Rprintf("neqs: %d\n", xp->neq);

    solve_sc_spa(xp, REAL(x), REAL(w) , REAL(tol), INTEGER(maxiter)); 
    Rprintf("\niterations: %d\n", INTEGER(maxiter)[0]); 
    Rprintf("accuracy  : %g\n", REAL(tol)[0] );
    
    niter = allocVector(VECSXP,1);
    INTEGER(niter)[0] = INTEGER(maxiter);
    REAL(eps)[0] = REAL(tol)[0]; 

    UNPROTECT(4);

    setAttrib(x,install("tol"),eps);
    setAttrib(x,install("niter"),niter);    
    return x;
}



