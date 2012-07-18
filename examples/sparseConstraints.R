
library(editrules)

# example of Pannekoek and Chun (2012)
E <- editmatrix(expression(
    x1 - x5 + x8 == 0,
    x5 - x3 - x4 == 0,
    x8 - x6 - x7 == 0
))
x <- c(33,2,100,3,103,50,20,70)*10
w <- rep(1,length(x))

dyn.load("../pkg/src/R_solve.so")
fs <- dir("../pkg/R/",full.names=TRUE)
for ( f in fs ) dmp <- source(f)

e <- sparseConstraints(E)
print(e)

.Call('R_solve_sc_spa',e$sc,x, w, 1e-5)



