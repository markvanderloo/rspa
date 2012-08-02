

adjustRecords <- function(E, dat, adjust, w=array(1,dim=c(nrow(dat), ncol(dat))), ...){

	
	out <- t(dat)
	n <- nrow(dat)
	acc <- numeric(n)
	tpl <- editrules:::getDuration(proc.time())
	dur <- array(0,dim=c(n,length(tpl)))
	colnames(dur) <- names(tpl)
	nit <- numeric(n)
	status <- character(n)

	cat("\n")
	for ( i in 1:nrow(dat) ){
		cat( sprintf("\rAdjusting record %4d / %d ",i,n))
		r <- do.call(c,as.list(dat[i,]))
		J <- adjust[i,]
		if (!any(J)) next
	
		e <- reduce(substValue(E,names(r)[!J],r[!J]))
		y <- adjust(e, r[J])
		out[J,i] <- y$x
		acc[i] <- y$accuracy
		dur[i,] <- editrules:::getDuration(y$duration)
		status[i] <- y$status
		nit[i] <- y$niter
		status[i] <- y$status
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


