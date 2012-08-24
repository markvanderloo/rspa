

// R interface functions to SparseConstraints

#include <R.h>
#include <Rdefines.h>

#include "sparseConstraints.h"

void R_sc_del(SEXP p){
    if (!R_ExternalPtrAddr(p)) return;
    sc_del(R_ExternalPtrAddr(p));
    R_ClearExternalPtr(p);
}


static void R_print_sc_row(SparseConstraints *x, int i, SEXP names){
   char *op;
   int n = x->nrag[i]-1;
   double b = x-> b[i];
   op = i < x->neq ? "= " : "<=";

   char varname[252];

   int hasnames = length(names) == 0 ? 0 : 1;

   Rprintf("%3d : ",i+1);
   for (int j=0; j < n; j++){
      if ( hasnames ){ // get varname from 'names'
         sprintf( varname, "%s",CHAR(STRING_ELT(names,x->index[i][j])) );
      } else {  // make surrogate varnames
         sprintf( varname, "X%d",x->index[i][j] );
      }
         
      Rprintf("%g*%s + ", x->A[i][j], varname );
   }
   // prevent -0 printing
   b = b == 0.0 ? 0.0 : b;    
  if ( hasnames ){ // get varname from 'names'
     sprintf( varname, "%s",CHAR(STRING_ELT(names,x->index[i][n])) );
  } else {  // make surrogate varnames
     sprintf( varname, "X%d",x->index[i][n] );
  }
   Rprintf("%g*%s %.1s %g\n",x->A[i][n], varname, op , b);

}

SEXP R_print_sc(SEXP p, SEXP names, SEXP printrange){
    PROTECT(p);
    PROTECT(names);
    PROTECT(printrange);
    int *pr = INTEGER(printrange);
    int nn=0, npr = length(printrange);
    SparseConstraints * xp = R_ExternalPtrAddr(p);
    if (!xp){
        Rprintf("NULL pointer\n");
        return R_NilValue;
    }

     
    for ( int i=0; i<npr; nn += pr[i++] >= xp->nconstraints ? 0 : 1);

    
    Rprintf("Sparse numerical constraints.\n");
    Rprintf("  Variables   : %d\n",xp->nvar);
    Rprintf("  Restrictions: %d (printing %d)\n",xp->nconstraints, nn);

    
    for ( int i =0; i < npr; i++){
       if ( pr[i] >= xp->nconstraints ) continue;
       R_print_sc_row(xp, pr[i], names); 
    }
    UNPROTECT(3);
    return R_NilValue;
}


SEXP R_get_nvar(SEXP p){
   PROTECT(p);
   SparseConstraints *xp = R_ExternalPtrAddr(p);
   SEXP out = allocVector(VECSXP,1);
   PROTECT(out = allocVector(INTSXP,1));
   INTEGER(out)[0] = xp->nvar;
   UNPROTECT(2);
   return out;
}

SEXP R_get_nconstraints(SEXP p){
   PROTECT(p);
   SparseConstraints *xp = R_ExternalPtrAddr(p);
   SEXP out = allocVector(VECSXP,1);
   PROTECT(out = allocVector(INTSXP,1));
   INTEGER(out)[0] = xp->nconstraints;
   UNPROTECT(2);
   return out;
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

    if (E == 0) error("%s\n","Could not allocate enough memory");

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








