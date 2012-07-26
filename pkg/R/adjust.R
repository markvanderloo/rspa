
#' Adjust a data to meet linear (in)equality constraints
#'
#' Adjust a (possibly large) vector \eqn{x} to meet (possibly many) constraints \eqn{Ax \leq b}. 
#' 
#' 
#'
#' @param object an \code{R} object describing constraints (see details)
#' @param ... Arguments to be passed to other methods
#'
#'
#' @section Details:
#' \code{adjust} is a generic function allowing several definitions of the constraints to
#' which the input vector have to be adjusted.
#' \itemize{
#'  \item[editmatrix]{If \code{object} is an \code{editmatrix}, the function will 
#'    try to match the names of \code{x} to the variable names in \code{object}. Before further
#'    processing.
#'    Depending on the choice of \code{method}, \code{object} is converted to \code{matrix} or 
#'    \code{sparseConstraints} format for further processing.
#'  }
#'  \item[matrix]{If \code{object} is a \code{matrix}, you also need to provide the constant vector
#'   \code{b}, the number of equations \code{neq} to define the problem. It is assumed that the first
#'   \code{neq} rows of \code{object} correspond to equalities. No names are matched, so \code{x}
#'    must be in the right order and must be of the right dimension. If \code{object} is a \code{matrix},
#'    the problem is solved with the dense method. See \code{\link{sparseConstraints}} on how to translate
#'    a \code{matrix} problem to the sparse version.
#'  } 
#'
#' \item[sparseConstraints] {If \code{object} is of class \code{\link{sparseConstraints}}, 
#'   the sparse method is used to adjust \code{x}. Some basic checks on \code{x} are performed, but
#'   no attempt is made to match names of \code{x} to those of \code{object}.
#' }
#'
#'
#'}
#'
#' @export
adjust <- function(object, ...){
   UseMethod('adjust')
}


#' 
#' @param b Constant vector of the constraint system \eqn{Ax\leq b}
#' @param x The vector to be adjusted
#' @param neq the first \code{neq} linear relations are equalities.
#' @param w A positive weight vector
#' @param tol The convergence criterium \bold{TODO: DESCTRIBE BETTER}
#' @param maxiter maximum number of iterations
#'
#' @method adjust matrix
#' @export
#' @rdname adjust
adjust.matrix <- function(object, b, x, neq=length(b), w=rep(1.0,length(x)), tol=1e-2, maxiter=100L, ...){
   stopifnot(  
      tol > 0, 
      all_finite(w),
      all(w > 0), 
      ncol(object) == length(x), 
      nrow(b) != nrow(object),
      maxiter > 0 
   ) 


   if ( !all_finite(x) ) stop("One or more variables of x occuring in a constraint are NA, NaN or Inf")

   storage.mode(object) <- "double"
   
   
   t0 <- proc.time()
   y <- .Call("R_dc_solve", 
      object, 
      as.double(b), 
      as.double(w),
      as.integer(neq),
      as.double(tol),
      as.integer(maxiter),
      as.double(x)
   )
   t1 <- proc.time()

   new_adjusted(y, t1-t0, colnames(object))
} 

#'
#' @method adjust editmatrix
#' @param method use dense or sparse matrix method.
#' @export
#' @rdname adjust
adjust.editmatrix <- function(object, x, method=c('dense','sparse'), ...){
	method <- match.arg(method)
   if (!isNormalized(object)) object <- normalize(object)

   # match names 
   if ( !is.null(names(x)) ){
      J <- match(getVars(object), names(x))
   } else {
      stopifnot(length(x) == length(getVars(object)))
      J <- 1:length(x)
   }
   u <- x[J]

   ops <- getOps(object)
   I <- order(ops,decreasing=TRUE)
   neq <- sum(ops == "==")


	if ( method == 'sparse' ){
		y <- adjust.sparseConstraints(sparseConstraints(object),u)
	} else {
		y <- adjust.matrix(
			object = getA(object)[I,,drop=FALSE], 
			b      = getb(object)[I], 
			x      = u,
			neq    = neq,
			... 
		)
	} 
		
   
   x[J] <- y$x
   y$x <- x
   y
}

#'
#' @method adjust sparseConstraints
#' @export
#' @rdname adjust
adjust.sparseConstraints <- function(object, x, ...){

   stopifnot(
		is.numeric(x),
      all_finite(x)
   )

   object$adjust(x)

}








