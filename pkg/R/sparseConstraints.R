
#' generate sparse restriction set
#'
#' @param x R object to be translated to sparseConstraints format.
#'
sparseConstraints = function(x, ...){
    UseMethod("sparseConstraints")
}


#' Sparse numeric edits from {\sf editmatrix} object
#'
#' @param tol Tolerance for testing where coefficients are zero
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

#'
#'
#' @param base do the row and column indices start at 1 (R-style) or at 0?
#' @param b constant vector
#' @param names optional vector with variable names.
#' @rdname sparseConstraints
#' @export
sparseConstraints.matrix = function(x, b, neq, base=min(x[,2]), names, ...){
    if (length(b) != length(unique(x[,1]))){ 
        stop("length of b unequal to number of unique rows")
    }
    if (base > 1){ 
        stop("base should be 1 or 0 (does your matrix have empty columns?)")
    }
    e <- new.env()
    e$sc <- .Call("R_sc_from_sparse_matrix", 
        as.integer(rowcol[,1]), 
        as.integer(rowcol[,2]-base),
        as.double(coef), 
        as.double(b),
        as.integer(3)
    )
    if ( !missing(names) ) e$names <- names;
    structure(e, class="sparseConstraints")
}

print.sparseConstraints <- function(x, maxprint=10L, ...){
    if (is.null(x$names)) {
        vars = " ";
        maxprint = 0L
    } else {
        vars = x$names
    }
    dump <- .Call("R_print_sc",x$sc, vars, as.integer(maxprint))
}


names.sparseConstraints <- function(x) x$names



