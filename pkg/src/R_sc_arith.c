
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

SEXP R_sc_diffmax(SEXP p, SEXP x){

   PROTECT(p);
   PROTECT(x);
   SparseConstraints *xp = R_ExternalPtrAddr(p);

   SEXP d = allocVector(REALSXP,1);
   PROTECT(d);
   REAL(d)[0] = sc_diffmax( xp, REAL(x) );
   UNPROTECT(3);

   Rprintf("I'm returning %g\n",REAL(d)[0]);
   return d;

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

/* this is currently not exported
SEXP R_sc_substvalue(SEXP p, SEXP col, SEXP val){
   PROTECT(p);
   PROTECT(col);
   PROTECT(val);

   SparseConstraints *xp = R_ExternalPtrAddr(p);
   SEXP nsub = allocVector(INTSXP,1);
   PROTECT(nsub);

   INTEGER(nsub)[0] = sc_substvalue(xp, INTEGER(col)[0], REAL(val)[0]);

   UNPROTECT(4);

   return nsub;
}*/

