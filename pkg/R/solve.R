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
adapt.sparseConstraints <- function(constraints, x, w=rep(1,length(x)), tol=1e-2, maxiter=1e5L, ...){
   x0 <- x;
   y = .Call('R_solve_sc_spa',constraints$sc, x0, w, tol, as.integer(maxiter))
   yy <- as.vector(y)
   names(yy) <- names(constraints)
   structure(list(
        sol = yy,
        accuracy = attr(y,"accuracy"),
        niter = attr(y,"niter")
        ),
        class = "adapt"
  )
}

#' print method for adapt object
#'
#' @keywords internal
#' @export
print.adapt <- function(x,...){
    cat(sprintf("Accuracy  : %g\n",x$accuracy))
    cat(sprintf("Iterations: %d\n", x$niter))
    cat("Solution:\n")
    print(x$sol)
}

