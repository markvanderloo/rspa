#' adapt
#' 
#' @param constraints linear (in)equality constraints on \code{x}
#' @param x numeric vector
#' @export
adapt <- function(constraints, x,...){
   UseMethod('adapt')

}

#' Solve quadratic programming problem
#'
#' @param sc constraints in \link{\code{sparseConstraints}} format
#' @param x original values
#; @param w weights
#' @param tol accuracy: max(solution - x)
#' 
#'
#' @export
adapt.sparseConstraints <- function(constraints, x, w=rep(1,length(x)), tol=1e-2, maxiter=1e5, ...){

   y = .Call('R_solve_sc_spa',e$sc, x, w, tol, as.integer(maxiter))

}



