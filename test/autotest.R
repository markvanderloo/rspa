library(testthat)



dyn.load("../pkg/src/rspa.so")
auto_test("../pkg/R", "../pkg/inst/tests")

