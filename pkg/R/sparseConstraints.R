
#' generate sparse restriction set
#'
#' @param x R object to be translated to sparseConstraints format.
#'
sparseConstraints = function(x, ...){
    UseMethod("sparseConstraints")
}


#' Sparse numeric edits from {\sf editmatrix} object
#'
#' @param tol Tolerance for testing where coefficients are zero
#'
sparseConstraints.editmatrix = function(x, tol=1e-8, ...){
    require(editrules)

    ieq <- getOps(E) == '=='
    I <- c(which(ieq),which(!ieq))
    x <- x[I,];
    e <- new.env();
    e$.sc <- .Call("R_sc_from_matrix", getA(x), getb(x), sum(ieq), tol)
    e$.vars <- getVars(E)
    make_sc(e)
}

#'
#'
#' @param base do the row and column indices start at 1 (R-style) or at 0?
#' @param b constant vector
#' @param names optional vector with variable names.
#' @rdname sparseConstraints
#' @export
sparseConstraints.matrix = function(x, b, neq, base=min(x[,2]), names, ...){
    if (length(b) != length(unique(x[,1]))){ 
        stop("length of b unequal to number of unique rows")
    }
    if (base > 1){ 
        stop("base should be 1 or 0 (does your matrix have empty columns?)")
    }
    e <- new.env()
    e$.sc <- .Call("R_sc_from_sparse_matrix", 
        as.integer(rowcol[,1]), 
        as.integer(rowcol[,2]-base),
        as.double(coef), 
        as.double(b),
        as.integer(3)
    )
    
    make_sc(e) 
}

print.sparseConstraints <- function(x, range=1L:10L, ...){
   x$print()
}

# e: environment containing an R_ExternalPtr
make_sc <- function(e){
   
   e$pointer <- function(){
      e$.sc
   }
   
   e$nvar <- function(){
      .Call("R_get_nvar",e$.sc)
   }

   e$nconstr <- function(){
      .Call("R_get_nconstraints",e$.sc)
   }
   
   e$getVars <- function(){
     e$.vars
   }  

   e$print <- function(range=1L:10L){
      range = range-1;
      stopifnot(all(range >= 0))
      if (is.null(e$.vars)) {
         vars = " ";
         range = 0L
      } else {
         vars = e$.vars
      }
      dump <- .Call("R_print_sc",e$.sc, vars, as.integer(range))
   }
 
   # adapt input vector minimally to meet restrictions.
   e$adapt <- function(x, w=rep(1,length(x)), tol=1e-2, maxiter=1e5L, ...){
      d <- system.time( 
         y <- .Call('R_solve_sc_spa',
            e$.sc, as.double(x), 
            as.double(w), 
            as.double(tol), 
            as.integer(maxiter)
         )
      )
      acc = attr(y,"accuracy")
      nit = attr(y,"nit")
      attr(y,c("accuracy","nit")) <- NULL
      names(y) <- e$.vars; 
      structure(
         list(x = y, accuracy = acc, niter = nit, duration=d ),
         class = "adapt"
      )   
   }
   structure(e,class="sparseConstraints")
}






