

## all_finite

expect_true(rspa:::all_finite(c(1:3)))
expect_false(rspa:::all_finite(c(1,2,NA)))
expect_false(rspa:::all_finite(c(NA,1,2)))
expect_false(rspa:::all_finite(c(NaN,1,2)))
expect_false(rspa:::all_finite(c(Inf,1,2)))
expect_false(rspa:::all_finite(c(-Inf,1,2)))
expect_false(rspa:::all_finite(c(1,2,NA)))
expect_false(rspa:::all_finite(c(1,2,NaN)))
expect_false(rspa:::all_finite(c(1,2,Inf)))
expect_false(rspa:::all_finite(c(1,2,-Inf)))






