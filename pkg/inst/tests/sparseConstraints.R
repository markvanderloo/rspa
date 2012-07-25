

library(editrules)
   E <- editmatrix(expression(
      x  + y == 1,
      y  > 0,
      x >= 0
   ))
   e <- sparseConstraints(E)

context("SparseConstraints")


test_that("sparseConstraints are properly constructed from editmatrix",{
   expect_equal(e$nvar(), 2)
   expect_equal(e$getVars(), c("x","y"))
   expect_equal(e$nconstr(), 3)
   
})


test_that("sparseConstraints arithmethic is correct",{
   x <- c(1,-2);
   expect_equivalent(e$multiply(x),c(-1,2,-1))
   expect_equivalent(e$diffvec(x), c(-2,2,-1))
   expect_equivalent(e$diffmax(x), 2)
   expect_equivalent(e$diffsum(x), 4)
})

test_that("sparseConstraints adjustment methods work",{
   x <- c(x=0,y=1)
   # no adjusting necessary
   expect_equal(e$adjust(x)$x, x)
   
   # adjusting necessary
   x <- c(x=0,y=0)
   expect_equal(e$adjust(x)$x, c(x=0.5,y=0.5))

})

