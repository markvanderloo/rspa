#' A package for minimal vector adjustment.
#' 
#' @section Overview:
#'
#' Given a vector \eqn{\boldsymbol{x}^0}, and a set linear restrictions of the
#' form  \eqn{\boldsymbol{a}_i\cdot\boldsymbol{x}_i=b_i} and/or 
#' \eqn{\boldsymbol{a}_i\cdot\boldsymbol{x}_i\leq b_i} with \eqn{i=1,2,\ldots,m}. This package finds the 
#' nearest vector to \eqn{\boldsymbol{x}^0} (in the (weighted) euclidean sense)
#' that satisfies all restrictions. 
#' 
#' Much of this package's functionality, including algorithms for working
#' with large, sparse problems has been moved to the \code{lintools} package.
#' This package will serve as a front-end for application of the succsessive
#' projection algorithm for data stored in data.frame like objects.
#' 
#' @name rspa-package
#' @docType package
#' @useDynLib rspa, .registration=TRUE
#' @import graphics validate lintools
#' @importFrom stats density
{}


