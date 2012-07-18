
library(editrules)

E <- editmatrix(expression(
  3*x1 + 4*x2 == 5*x3,
  6*x3 + 7*x4 == 8,
  x5 < 0))

dyn.load("../pkg/src/R_sparseConstraints.so")

e <- .Call("R_sc_from_matrix",getA(E),getb(E),2L,1e-8)

.Call("R_print_sc",e)

.Call("R_sne_del",e)



