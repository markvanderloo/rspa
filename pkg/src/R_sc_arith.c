
#include <R.h>
#include <Rdefines.h>
#include "sparseConstraints.h"
#include "sc_arith.h"


SEXP R_sc_multvec(SEXP p, SEXP x){
   PROTECT(p);
   PROTECT(x);
   SparseConstraints *xp = R_ExternalPtrAddr(p);
   SEXP Ax = allocVector(REALSXP, xp->nconstraints);
   PROTECT(Ax);
   
   sc_multvec(xp, REAL(x), REAL(Ax));

   UNPROTECT(3);
   return Ax;

}


SEXP R_sc_diffvec(SEXP p, SEXP x){
   PROTECT(p);
   PROTECT(x);
   SparseConstraints *xp = R_ExternalPtrAddr(p);

   SEXP dv = allocVector(REALSXP, xp->nconstraints);
   PROTECT(dv);

   sc_diffvec(xp, REAL(x), REAL(dv));

   UNPROTECT(3);
   return dv;

}


SEXP R_sc_diffsum(SEXP p, SEXP x){

   PROTECT(p);
   PROTECT(x);
   SparseConstraints *xp = R_ExternalPtrAddr(p);

   SEXP d = allocVector(REALSXP,1);
   REAL(d)[0] = sc_diffsum( xp, REAL(x) );
   UNPROTECT(2);

   return d;

}



