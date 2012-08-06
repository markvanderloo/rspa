#' Adjusted records
#' @name adjustedRecords
#' @seealso \code{\link{adjustRecords}}
#' @section Details:
#' todo
#' 
#' 
#' @seealso \code{\link{adjustRecords}}
{}


#' @method print adjustedRecords
#' @param x object of class \code{adjustedRecords}
#' @param ... additional parameters to pass to other methods
#' @rdname adjustedRecords
#' @export
print.adjustedRecords <- function(x,...){
   I <- 1:min(10,nrow(x))
   cat("Object of class 'adjustedRecords'\n")
   print(
      cbind(x$adjusted[I,],x$status[I,])
   )
   if ( nrow(x$adjusted)>10 ) cat("print truncated...\n")
}



# internal method for statusblock of adjustedRecords
`%++%` <- function(x=NULL,y){
   if ( is.null(x) ) return(y)

   x$accuracy <- pmax(x$accuracy, y$accuracy)
   x$objective <- sqrt(x$objective*x$objective + y$objective*y$objective)
   x$status <- pmax(x$status,y$status,na.rm=TRUE)
   x$niter    <- x$niter + y$niter
   x[5:7]     <- x[5:7] + y[5:7] # durations
   x

}




