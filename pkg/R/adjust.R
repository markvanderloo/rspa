
#' Adjust a data to meet linear (in)equality constraints
#'
#' This is a user-friendly wrapper acessing both dense and sparse matrix methods 
#' for variable adjutment. 
#'
#' @param object an R object describing constraints
#' @param ... Arguments to be passed to other methods
#'
#' @export
adjust <- function(object, ...){
   UseMethod('adjust')
}


#' 
#' @param b Constant vector of the constraint system \eqn{Ax\leq b}
#' @param x The vector to be adjusted
#' @param w A positive weight vector
#' @param neq the first \code{neq} linear relations are equalities.
#' @param tol The convergence criterium \bold{TODO: DESCTRIBE BETTER}
#' @param maxiter maximum number of iterations
#'
#' @method adjust matrix
#' @export
#' @rdname adjust
adjust.matrix <- function(object, b, x, w=rep(1,length(x)), neq=length(b), tol=1e-2, maxiter=100L, ...){
   storage.mode(object) <- "double"
 
   .Call("R_dc_solve", 
      object, 
      as.double(b), 
      as.double(w),
      as.integer(neq),
      as.double(tol),
      as.integer(maxiter),
      as.double(x),
      PACKAGE="rspa"
   )
} 

#'
#' @method adjust editmatrix
#' @export
#' @rdname adjust
adjust.editmatrix <- function(object, x,...){
   if (!isNormalized(object)) object <- normalize(object)


   if ( !is.null(names(x)) ){
      J <- match(getVars(object),names(x))
      u <- x[J]
   } else {
      stopifnot(length(x) == length(getVars(object)))
      J <- 1:length(x)
   }

   I <- order(getOps(object),decreasing=TRUE)

   neq <- sum(getOps(object) == "==")

   y <- adjust.matrix(
      object = getA(object)[I,,drop=FALSE], 
      b      = getb(object)[I], 
      x      = u,
      neq    = neq,
      ... 
   )
   
   y
}

