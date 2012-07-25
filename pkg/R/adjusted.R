#' Adjusted object
#' @name adjusted
#' @format The \code{adjusted} object contains the results
#' for adjusting a single vector.
#'
{}


#' @method print adjusted
#' @param x an object of class \code{adjusted}
#' @param maxprint max number of output values to print
#' @param ... parameters to pass to other methds
#' 
#' @rdname adjusted
#' @export
print.adjusted <- function(x, maxprint = 10, ...){
    cat("Object of class 'adjusted'\n")
    cat(sprintf("  Status    : %s\n", x$status))
    cat(sprintf("  Accuracy  : %g\n", x$accuracy))
    cat(sprintf("  Iterations: %d\n", x$niter))
    cat(sprintf("  Timing (s): %g\n", x$duration['elapsed']))
    tr = ":";
    if (length(x$x) > 10) tr = sprintf(" (truncated at %d):",maxprint)
    cat(paste("Solution",tr,"\n",sep=""))
    print(x$x[1:min(length(x$x),maxprint)])
}

# create 'adjusted' object. Input is a solution vector 
# with attributes, returned by "R_sc_solve_spa" or "R_dc_solve_spa"
new_adjusted <- function(x, duration, varnames=NULL){
   statusLabels = c(
      "success",
      "aborted: could not allocate enough memory",
      "aborted: divergence detected"
   )
   acc = attr(x,"accuracy")
   nit = attr(x,"niter")
   status = statusLabels[attr(x,"status")+1]
   attr(x,"accuracy") <- NULL
   attr(x,"niter")    <- NULL
   attr(x,"status")   <- NULL
   names(x) <- varnames
   structure(
      list(x = x, accuracy = acc, niter = nit, duration=duration, status=status ),
      class = "adjusted"
   )   
}

