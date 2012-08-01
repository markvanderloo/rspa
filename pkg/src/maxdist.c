

#include <math.h>

// computes ||x-y||_Inf
double maxdist(double *x, double *y, int n){
    double t, m = fabs(x[0]-y[0]);
    
    for ( int j=1; j<n; j++ ){ 
        t = fabs(x[j]-y[j]);
        m = t > m ? t : m;
    }
    return m;
}

double absmax(double *conv, double *awa, int neq, int nconstraints){

	double d, dmax=0;
	for ( int i=0;  i<neq; ++i ){
		d = fabs(conv[i] * awa[i]);
      dmax = (d > dmax) ? d : dmax;
	}
	for ( int i=neq; i<nconstraints; ++i){
		d = (conv[i] < 0) ? 0 : conv[i]*awa[i];
		dmax = (d > dmax) ? d : dmax;
	}
	return dmax;
}

// d = Ax-b for equalities eps=|d|, for inequalities eps=(|d|+d)/2. returns max(eps)
double dc_diffmax(double *A, double *b, double *x, int neq, int m, int n){
	double ax, d, dmax=0;
	for ( int i=0; i < neq; ++i ){
		ax = 0;
		for ( int j=0; j < n; ++j ){
			ax += A[i + m*j]*x[j];
		}
	   d = fabs(ax - b[i]);
		dmax = d > dmax ? d : dmax;
	}

	for ( int i=neq; i < m; ++i ){
		ax = 0;
		for ( int j=0; j < n; ++j ){
			ax += A[i + m*j]*x[j];
		}
		d =  ax < b[i]  ? 0 : ax - b[i];
		dmax =  d > dmax  ? d : dmax;
	}

	return dmax;
}


int diverged(double *x, int n){
   for (int i=0; i<n; ++i){
      if (!isfinite(x[i])) return 1;
   }
   return 0;
}




