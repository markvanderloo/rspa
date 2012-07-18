
#' generate sparse restriction set
#'
#' @param x 
#'
sparseConstraints = function(x, ...){
    UseMethod("sparseConstraints")
}


#' Sparse numeric edits from {\sf editmatrix} object
#'
#'
#'
sparseConstraints.editmatrix = function(x, tol=1e-8, ...){
    require(editrules)

    ieq <- getOps(E) == '=='
    I <- c(which(ieq),which(!ieq))
    x <- x[I,];
    e <- new.env();
    e$sc <- .Call("R_sc_from_matrix", getA(x), getb(x), sum(ieq), tol)
    e$names <- getVars(E)
    structure(e,class="sparseConstraints")
}


print.sparseConstraints <- function(x,...){
    dump <- .Call("R_print_sc",x$sc, x$names)
}


names.sparseConstraints <- function(x) x$names



