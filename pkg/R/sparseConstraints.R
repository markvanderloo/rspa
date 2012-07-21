
#' generate sparse restriction set
#'
#' @param x R object to be translated to sparseConstraints format.
#' @param ... options to be passed to other methods
#' @export
#' @example ../examples/sparseConstraints.R
sparseConstraints = function(x, ...){
    UseMethod("sparseConstraints")
}


#'
#' @param tol Tolerance for testing where coefficients are zero
#' @rdname sparseConstraints
#' @export
sparseConstraints.editmatrix = function(x, tol=1e-8, ...){
   if (!isNormalized(x)) normalize(x)

   ieq <- getOps(x) == '=='
   I <- c(which(ieq),which(!ieq))
   x <- x[I,];
   e <- new.env();
   A <- getA(x);
   storage.mode(A) <- "double"
   e$.sc <- .Call("R_sc_from_matrix", A, as.double(getb(x)), as.integer(sum(ieq)), as.double(tol), PACKAGE="rspa")
   e$.vars <- getVars(x)
   make_sc(e)
}


#'
#' @param b Constant vector
#' @param neq The first \code{new} equations are interpreted as equality constraints, the rest as '<='
#' @param base are the indices in \code{x[,1:2]} base 0 or base 1?
#' @export
#' @rdname sparseConstraints
sparseConstraints.data.frame <- function(x, b, neq=length(b), base=min(x[,2]), ...){
   if (length(b) != length(unique(x[,1]))){
      stop("length of b unequal to number of constraints")
   }
   if (base > 1){
      stop("base should be 1 or 0 (does your condition matrix have empty columns?)")
   }
   e <- new.env()
   e$.sc <- .Call("R_sc_from_sparse_matrix", 
      as.integer(x[,1]), 
      as.integer(x[,2]-base),
      as.double(x[,3]), 
      as.double(b),
      as.integer(neq),
      PACKAGE="rspa"
   )
   make_sc(e)

}




#' print method for sparse constraints object
#' 
#' @param range integer vector stating which constraints to print
#'
#' @export
#' @rdname sparseConstraints
print.sparseConstraints <- function(x, range=1L:10L, ...){
   x$print()
}

# e: environment containing an R_ExternalPtr
make_sc <- function(e){
   
   e$pointer <- function(){
      e$.sc
   }
   
   e$nvar <- function(){
      .Call("R_get_nvar", e$.sc, PACKAGE="rspa")
   }

   e$nconstr <- function(){
      .Call("R_get_nconstraints", e$.sc, PACKAGE="rspa")
   }
   
   e$getVars <- function(){
     e$.vars
   }  

   e$print <- function(range){
      if ( missing(range) & e$nvar() > 10 ) range = numeric(0)
      if ( missing(range) & e$nvar() <=10 ) range = 1L:10L
      vars = e$getVars()
      if ( is.null(vars) ) vars = character(0);

      stopifnot(all(range >= 1))
      range = range-1;

      dump <- .Call("R_print_sc",e$.sc, vars, as.integer(range),  PACKAGE="rspa")
   }
 
   # adapt input vector minimally to meet restrictions.
   e$adapt <- function(x, w=rep(1,length(x)), tol=1e-2, maxiter=1e5L, ...){
      d <- system.time( 
         y <- .Call('R_solve_sc_spa',
            e$.sc, 
            as.double(x), 
            as.double(w), 
            as.double(tol), 
            as.integer(maxiter),
            PACKAGE="rspa"
         )
      )
      statusLabels = c(
         "success",
         "aborted: could not allocate enough memory",
         "aborted: divergence detected"
      )
      acc = attr(y,"accuracy")
      nit = attr(y,"niter")
      status = statusLabels[attr(y,"status")+1]
      attr(y,"accuracy") <- NULL
      attr(y,"niter")    <- NULL
      attr(y,"status")   <- NULL
      names(y) <- e$.vars; 
      structure(
         list(x = y, accuracy = acc, niter = nit, duration=d, status=status ),
         class = "adapt"
      )   
   }

   e$diffsum <- function(x){
      stopifnot(length(x)==e$nvar())
      .Call("R_sc_diffsum", e$.sc, as.double(x), PACKAGE="rspa") 
   }

   structure(e,class="sparseConstraints")
}






