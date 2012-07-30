# define constraints from editmatrix object:
E <- editmatrix(expression(
     x1 + x8 ==  950,
     x3 + x4 ==  950 ,
     x6 + x7 == x8,
     x4 > 0
))

# generate sparseConstraints object
sparseConstraints(E)

# same constraints, from data.frame
rc <- data.frame( 
   row = c( 1, 1, 2, 2, 3, 3, 3, 4),
   col = c( 1, 2, 3, 4, 2, 5, 6, 4),
  coef = c(-1,-1,-1,-1, 1,-1,-1,-1)
)
b <- c(-950, -950, 0,0) 

sparseConstraints(rc, b, neq=3)


# same constraints, from dense matrix

A <- getA(E)
b <- getb(E)
sparseConstraints(A, b, neq=3)



