# TODO: MUCH TO SIMPLIFY
#

### example of Pannekoek and Chun (2012)
# define constraints ('edits'):
E <- editmatrix(expression(
    x1 - x5 + x8 == 0,
    x5 - x3 - x4 == 0,
    x8 - x6 - x7 == 0,
    x4 > 0
))

x <- c(x1=33,x2=2,x3=100,x4=3,x5=95,x6=50,x7=20,x8=70)*10

# In the reference, variable 'x5' considered ok, and therefore fixed
E <- substValue(E,'x5',x['x5'])

# generate sparseConstraints objects
e <- sparseConstraints(E)

I <- match(e$getVars(), names(x))

u <- x[I]


y <- adjust(e,u, tol=1e-5)
y

## assemble the answer 
xt <-  x;
xt[I] <- y$x;

# we can check that the new vector indeed obeys all restrictions (upto the tolerance)

# new vector
violatedEdits(E,xt, tol=1e-2)


