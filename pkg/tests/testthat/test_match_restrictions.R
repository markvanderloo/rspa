
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


context("tagging")

test_that("tagging works",{
  w <- women
  w[1,1] <- NA
  w <- tag_missing(w)
  expect_identical(tagged_values(w),is.na(w))
})

test_that("tags are picked up",{
  d <- data.frame(x=NA,y=0.5)
  d <- tag_missing(d)
  d$x <- 1
  v <- validator(x + y == 1, x>0,y>0)
  expect_equivalent(
    match_restrictions(d,v)
    , data.frame(x=0.5,y=0.5)
  )
})



