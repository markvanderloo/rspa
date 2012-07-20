#' adapt
#' 
#' @param constraints linear (in)equality constraints on \code{x}
#' @param x numeric vector
#' @export
adapt <- function(constraints, x,...){
   UseMethod('adapt')

}


#' print method for adapt object
#'
#' @keywords internal
#' @export
print.adapt <- function(x, maxprint = 10, ...){
    cat("Object of class 'adapt'\n")
    cat(sprintf("  Accuracy  : %g\n", x$accuracy))
    cat(sprintf("  Iterations: %d\n", x$niter))
    cat(sprintf("  Timing (s): %g\n", x$duration['elapsed']))
    tr = ":";
    if (length(x$x) > 10) tr = sprintf(" (truncated at %d):",maxprint)
    cat(paste("Solution",tr,"\n",sep=""))
    print(x$x[1:min(length(x$x),maxprint)])
}

