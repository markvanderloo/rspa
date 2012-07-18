
library(editrules)

# example of Pannekoek and Chun (2012)
E <- editmatrix(expression(
    x1 - x5 + x8 == 0,
    x5 - x3 - x4 == 0,
    x8 - x6 - x7 == 0,
    x1 > 0,
    x3 > 0,
    x5 > 0,
    x7 > 0,
    x8 > 0
))
x <- c(x1=34,x2=2,x3=100,x4=3,x5=103,x6=50,x7=20,x8=70)*10



violatedEdits(E,x,tol=1e-4)


dyn.load("../pkg/src/R_solve.so")
fs <- dir("../pkg/R/",full.names=TRUE)
for ( f in fs ) dmp <- source(f)

e <- sparseConstraints(E)
print(e)


I <- match(getVars(E),names(x),nomatch=0)
I
u <- x[I]
w <- rep(1,length(u))

u
y = .Call('R_solve_sc_spa',e$sc,u, w, 1e-5,50L)
y
sol <- x;
sol[I] <- y


violatedEdits(E,sol,tol=1e-4)
sol
x
sqrt(sum((x-sol)^2))


