

#include "math.h"

// computes ||x-y||_Inf
double maxdist(double *x, double *y, int n){
    double t, m = fabs(x[0]-y[0]);
    
    for ( int j=1; j<n; j++ ){ 
        t = fabs(x[j]-y[j]);
        m = t > m ? t : m;
    }
    return m;
}

