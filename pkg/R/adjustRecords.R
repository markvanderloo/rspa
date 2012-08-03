#' Adjust records in a \code{data.frame}
#'
#' A convenient wrapper around \code{\link{adjust}} that loops over all records in a
#' \code{data.frame}
#'
#' @param E a \code{\link[editrules]{editmatrix}}
#' @param dat a \code{data.frame}
#' @param adjust a \code{nrow(dat) x ncol(dat)} boolean array, indicating which fields 
#'        must be adjusted.
#' @param w a vector of length \code{ncol(dat)} or array of size \code{adjust} with adjustment weights.
#' @param ... extra options, passed through to \code{\link{adjust}}
#' @seealso \code{\link{adjust}} 
#'
#' @return An object of class \code{adjustedRecords}
#' @export
adjustRecords <- function(E, dat, adjust, w=array(1,dim=c(nrow(dat), ncol(dat))), ... ){
   stopifnot(
      all(dim(adjust) == dim(dat)),
      all(dim(w) == dim(dat)),
      all(getVars(E) %in% names(dat))
   )

   nm <- names(dat)
   if ( is.null(colnames(adjust)) ) colnames(adjust) <- nm
   if ( is.null(colnames(w)) ) colnames(w) <- nm 

   B <- blocks(E)
   status = NULL
   for ( i in 1:length(B) ){
      cat(sprintf("adjusting block %4d of %4d\n",i, length(B)))
      e <- B[[i]]
      vars <- nm[nm %in% getVars(e)]
      adj <- adjustBlock(e, dat[vars], adjust[,vars,drop=FALSE], w[,vars,drop=FALSE]) 
      dat[vars] <- adj$adjusted
      status <- status %++% adj$status 
   }

   structure(list(dat, status), class="adjustedRecords")

}



adjustBlock <- function(E, dat, adjust, w, ...){

	out <- t(dat)
	n <- nrow(dat)
	acc <- numeric(n)
	tpl <- editrules:::getDuration(proc.time())
	dur <- array(0,dim=c(n,length(tpl)))
	colnames(dur) <- names(tpl)
	nit <- numeric(n)
	status <- new_status(rep(NA,n))

	cat("\n")
	for ( i in 1:nrow(dat) ){
		cat( sprintf("\rAdjusting record %4d / %d ",i,n))
		r <- do.call(c,as.list(dat[i,]))
		J <- adjust[i,]
		if (!any(J)) next
	
		e <- reduce(substValue(E,names(r)[!J],r[!J]))
		y <- adjust(e, r[J],...)
		out[J,i]    <- y$x
		acc[i]      <- y$accuracy
		dur[i,]     <- editrules:::getDuration(y$duration)
		status[i]   <- y$status
		nit[i]      <- y$niter
		status[i]   <- y$status
	}
   cat("\n")

	list(
		adjusted = as.data.frame(t(out)),
		status = data.frame(
			accuracy = acc,
		   niter    = nit,
			status   = status,
			dur
		)
	)
}






