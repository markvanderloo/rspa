
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

   SEXP niter, eps, status;
    
   // make copies outside R to prevent writing in userspace.
   double xtol = REAL(tol)[0];
   int xmaxiter = INTEGER(maxiter)[0];
   int s;
   double xx[length(x)];
   for ( int i=0; i<length(x); xx[i++] = REAL(x)[i]);
  
   // solve
   s = solve_sc_spa(xp, REAL(w) , &xtol, &xmaxiter, xx); 
    
   // copy answers back into R
   SEXP out = allocVector(REALSXP, length(x));
   for (int i=0; i<length(x); REAL(out)[i++] = xx[i]); 

   PROTECT(status = allocVector(INTSXP,1));
   PROTECT(niter = allocVector(INTSXP,1));
   PROTECT(eps = allocVector(REALSXP,1));

   INTEGER(status)[1] = s;
   INTEGER(niter)[0] = xmaxiter;
   REAL(eps)[0] = xtol;

   setAttrib(out,install("niter"), niter);
   setAttrib(out,install("accuracy"), eps);
   setAttrib(out,install("status"), status);


   UNPROTECT(7);
   return out;
}



