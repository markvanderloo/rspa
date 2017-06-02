#' Alter numeric data records to match linear (in)equality constraints.
#'
#' @param dat A \code{data.frame}
#' @param restrictions An object of class \code{\link[validate]{validator}}
#' @param adjust A \code{logical} matrix of dimensions \code{dim(dat)} where \code{TRUE} indicates
#'     that a value may be adjusted.
#' @param weight A weight vector of length \code{ncol(dat)} or a matrix of dimensions \code{dim(dat)}.
#' @param ... arguments passed to \code{\link[lintools]{project}}.
#' 
#' @export
match_restrictions <- function(dat, restrictions
      , adjust=array(TRUE, dim=dim(dat))
      , weight=rep(1,ncol(dat))
      , ...){
  stopifnot(inherits(dat,"data.frame"))
  stopifnot(inherits(restrictions,"validator"))
  
  if (is.vector(weight)){
    stopifnot(length(weight)==ncol(dat))
    weight <- matrix(rep(weight,times=nrow(dat)),byrow=TRUE,nrow=nrow(dat))
  } else {
    stopifnot(ncol(weight)==ncol(dat))
  }
  colnames(weight) <- colnames(dat)
  
  stopifnot(all(dim(adjust) == dim(dat)))
  colnames(adjust) <- colnames(dat)
  
  # Get linear restrictions, sort in normalized order.
  L <- restrictions$linear_coefficients(normalize=TRUE)
  i <- order(L$operators, decreasing = TRUE)
  L$A <- L$A[i,,drop=FALSE]
  L$b <- L$b[i,,drop=FALSE]
  L$operators <- L$operators[i]
  
  
  # check if all variables in validator occur in the data.
  not_in_dat <- colnames(L$A)[!colnames(L$A) %in% names(dat)]
  if (length(not_in_dat)>0){
    fstop("Validator object uses variables not present in data: %s"
          ,paste(not_in_dat, collapse=", "))
  }
  
  in_res <- names(dat)[names(dat) %in% colnames(L$A)]
  weight <- weight[ , in_res, drop=FALSE]
  adjust <- adjust[ , in_res, drop=FALSE]
  # working copy.
  M <- t(dat[in_res])
  
  for ( i in seq_along(ncol(M))){
    adj <- adjust[i,]
    x <- M[,i]
    constr <- lintools::subst_value(L$A, L$b, variables=!adj, values=x[!adj])
    
    constr <- lintools::compact(L$A
                , L$b
                , x=x
                , neq=sum(L$operators=="==")
                , nleq=sum(L$operators=="<="))
    
    wt <- weight[i, adj & !constr$cols_removed ]
    
    M[adj, i] <- lintools::project(x = constr$x
                  , A = constr$A
                  , b = constr$b
                  , neq = constr$neq
                  , w = wt, ...)$x
  }
  dat[in_res] <- t(M)
  dat
}


fstop <- function(fmt,...){
 stop(sprintf(fmt,...), call. = FALSE) 
}

fwarn <- function(fmt,...){
  warning(sprintf(fmt,...), call. = FALSE)
}

