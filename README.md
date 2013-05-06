rspa
====

An R package for optimal record adjustment

The package adjusts numeric records x, such that Ax <= b is met after adjustment. The
adjustment is kept 'as small as possible' by minimizing (x-x0)'W(x-x0), where x0 is the original value,
x the final value and W a _diagonal_ matrix with positive weights. Here, x may be fairly large; rspa has
been tested with vectors of about half a million elements under 60k (in)-equality restrictions.

h1. Todo

The main addition I'd like to make is to be able to use a general (sparse) W-matrix.

