
context("match_restrictions")
test_that("adjusts correctly",{
  v <- validator(x + y == 1, x > 0, y >= 0)
  dat <- data.frame(x=1,y=2)
  expect_equal( match_restrictions(dat,v)
    , data.frame(x=0,y=1)
    , tolerance=0.01
  )
  
  # extra, unrelated variable
  dat <- data.frame(x=1,y=2,z=0)
  expect_equal( match_restrictions(dat,v)
    , data.frame(x=0,y=1,z=0)
    , tolerance=0.01
  )
  
  
})




