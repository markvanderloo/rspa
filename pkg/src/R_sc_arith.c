
#include <R.h>
#include <Rdefines.h>
#include "sparseConstraints.h"
#include "sc_arith.h"


SEXP R_sc_diffsum(SEXP p, SEXP x){

   PROTECT(p);
   PROTECT(x);
   SparseConstraints *xp = R_ExternalPtrAddr(p);

   SEXP d = allocVector(REALSXP,1);
   REAL(d)[0] = sc_diffsum( xp, REAL(x) );
   UNPROTECT(2);

   return d;

}



