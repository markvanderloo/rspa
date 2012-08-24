
#include <R.h>
#include <Rdefines.h>

SEXP all_finite_double(SEXP x){
   PROTECT(x);
   double *xx = REAL(x);

   SEXP y = allocVector(LGLSXP,1);
   PROTECT(y);
 
   int i=1,  b=R_finite(xx[0]);
   while ( i < length(x) && b ) b = R_finite(xx[i++]);

   
   LOGICAL(y)[0] = b;
   UNPROTECT(2);

   return y; 
}


