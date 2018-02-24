#include <iostream>
#include <lapacke.h>
 
int main() {

	lapack_int n, nrhs, lda, ldb, info;
	int i, j;
	double *A, *b, *x;
	lapack_int *ipiv;
 
	n = 5; nrhs = 1; 
	lda=n, ldb=nrhs;

	A = (double *)malloc(n*n*sizeof(double));
	if (A==NULL){ std::cout << "error of memory allocation\n" << std::endl; exit(0); }
	b = (double *)malloc(n*nrhs*sizeof(double));
	if (b==NULL){ std::cout << "error of memory allocation\n" << std::endl; exit(0); }
	x = (double *)malloc(n*nrhs*sizeof(double));
	if (x==NULL){ std::cout << "error of memory allocation\n" << std::endl; exit(0); }
	ipiv = (lapack_int *)malloc(n*sizeof(lapack_int));
	if (ipiv==NULL){ std::cout << "error of memory allocation\n" << std::endl; exit(0); }
 
	for( i = 0; i < n; i++ ) {
		for( j = 0; j < n; j++ ) {
			A[i*lda+j] = ((double) rand()) / ((double) RAND_MAX) - 0.5;
		}

		for(i=0;i<n*nrhs;i++) {
			b[i] = ((double) rand()) / ((double) RAND_MAX) - 0.5;
			x[i] = b[i];
		}
	}
        info = LAPACKE_dgesv( LAPACK_ROW_MAJOR, n, nrhs, A, lda, ipiv, x, ldb );
	for( i = 0; i < n; i++ ) {
		std::cout << b[i] << " -> " << x[i] << std::endl;
	}
        return 0;
} 
