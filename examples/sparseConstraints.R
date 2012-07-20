

### sparse constraints from editmatrix.
library(editrules)

# example of Pannekoek and Chun (2012)
E <- editmatrix(expression(
    x1 - x5 + x8 == 0,
    x5 - x3 - x4 == 0,
    x8 - x6 - x7 == 0,
    x4 > 0
))
x <- c(x1=33,x2=2,x3=100,x4=3,x5=95,x6=50,x7=20,x8=70)*10

E <- substValue(E,'x5',x['x5'])


violatedEdits(E,x,tol=1e-4)


dyn.load("../pkg/src/rspa.so")
fs <- dir("../pkg/R/",full.names=TRUE)
for ( f in fs ) dmp <- source(f)

e <- sparseConstraints(E)
e
e$print(3:6)
e$getVars()
e$nconstr()

I <- match(getVars(E),names(x),nomatch=0)
I
u <- x[I]


y <- e$adapt(u,tol=1e-5)
y
xt <-  x;
xt[I] <- y$x;

x
xt

q();

### sparse constraints from row-column-coef-b representation
A <- getA(E)
rowcol = which(A != 0, arr.ind=TRUE)
coef = A[which(A != 0)]

o <- order(rowcol[,1])
rowcol <- rowcol[o,]
coef <- coef[o]
b <- getb(E)
cbind(rowcol,coef)

f <- sparseConstraints(rowcol,b,3,base=1,names=getVars(E))
f
E

