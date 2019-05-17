
## SparseConstraints

if (requireNamespace("editrules",quietly=TRUE)){

E <- editrules::editmatrix(expression(
  x  + y == 1,
  y  > 0,
  x >= 0
))
  
  # because deprecated
suppressWarnings({
  e <- sparseConstraints(E)
})

  # sparseConstraints are properly constructed from editmatrix
expect_equal(e$.nvar(), 2)
expect_equal(e$.nconstr(), 3)
   
  
  
# sparseConstraints arithmethic is correct
x <- c(1,-2)
expect_equivalent(e$.multiply(x),c(-1,2,-1))
expect_equivalent(e$.diffvec(x), c(-2,2,-1))
expect_equivalent(e$.diffmax(x), 2)
expect_equivalent(e$.diffsum(x), 4)
  
  
# sparseConstraints adjustment methods work
x <- c(x=0.5,y=0.5)
# no adjusting necessary
expect_equal(e$.adjust(x,w=c(1,1),tol=0.01,maxiter=100)$x, x,tolerance=0.01)

# adjusting necessary
x <- c(x=0,y=0)
expect_equal(e$.adjust(x,w=c(1,1), tol=0.01, maxiter=100)$x, c(x=0.5,y=0.5), tolerance=0.01)

}
