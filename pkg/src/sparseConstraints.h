
#ifndef avls_snumedits
#define avls_snumedits


// We use a ragged adday for sparse storage of numerical edit sets.
typedef struct _SparseConstraints {
    // number of edits
    int nconstraints;
    // the first neqs represent equations ax=b, the others inequations ax < b
    int neq;
    // number of variables
    int nvar;
    // ragged coefficient array
    double **A;
    // column indices of coefficients
    int **index;
    // number of coefficients in each row
    int *nrag;
    // constants
    double *b;
} SparseConstraints;



void sc_del(SparseConstraints *);

SparseConstraints * sc_new( int );

SparseConstraints * sc_from_matrix(double *, double *, int, int, int, double);

SparseConstraints * sc_from_sparse_matrix(int *, int *, double *, int, double *, int, int);

int get_max_nrag(SparseConstraints *);

#endif



