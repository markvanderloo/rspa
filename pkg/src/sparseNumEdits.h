
#ifndef avls_snumedits
#define avls_snumedits


// We use a ragged adday for sparse storage of numerical edit sets.
typedef struct _SparseNumEdits {
    // number of edits
    unsigned int nedits;
    // ragged coefficient array
    double **A;
    // column indices of coefficients
    int **index;
    // number of coefficients in each row
    int *nrag;
    // constants
    double *b;
} SparseNumEdits;



void sne_del(SparseNumEdits *);

SparseNumEdits * sne_from_matrix(double *, double *, int, int, double);

SparseNumEdits * sne_new( int );



#endif



