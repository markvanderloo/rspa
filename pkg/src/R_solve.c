
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

    double xtol = REAL(tol)[0];
    int xmaxiter = INTEGER(maxiter)[0];

    solve_sc_spa(xp, REAL(x), REAL(w) , &xtol, &xmaxiter); 
    //Rprintf("\niterations: %d\n", xmaxiter); 
    //Rprintf("accuracy  : %g\n", xtol );
    
    niter = allocVector(VECSXP,1);


    PROTECT(niter = allocVector(INTSXP,1));
    PROTECT(eps = allocVector(REALSXP,1));
    INTEGER(niter)[0] = xmaxiter;
    REAL(eps)[0] = xtol;

    setAttrib(x,install("niter"), niter);
    setAttrib(x,install("accuracy"), eps);
    UNPROTECT(6);
    return x;
}



