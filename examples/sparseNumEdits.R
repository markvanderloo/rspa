
library(editrules)

E <- editmatrix(expression(
  x1 + x2 == x3,
  x3 + 2*x4 == 5))

dyn.load("../pkg/src/sparseNumEdits.so")


