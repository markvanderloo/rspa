
# a very simple adjustment example
E <- editrules::editmatrix(expression(
	x + y == 10,
	x > 0,
	y > 0
)) 

# x and y will be adjusted by the same amount
adjust(E, c(x=4,y=5))

# One of the inequalies violated
adjust(E, c(x=-1,y=5))

# Weighted distances: 'heavy' variables change less
adjust(E,c(x=4,y=5), w=c(100,1))

# if w=1/x0, the ratio between coefficients of x0 stay the same (to first order)
x0 <- c(x=4,y=5)
x1 <- adjust(E,x0,w=1/x0)

x0[1]/x0[2]
x1$x[1] / x1$x[2]



