library(testthat)



dyn.load("../pkg/src/dc_spa.so")
auto_test("../pkg/R", "../pkg/inst/tests")

