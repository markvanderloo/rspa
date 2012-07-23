
#
# @param x an R object describing constraints
# @param ... Arguments to be passed to other methods
#
#  @export
adjust <- function(object, ...){
   UseMethod('adjust')
}


# 
#
#
#
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

