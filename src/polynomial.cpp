#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrices.h"

namespace is {

/*
	Explanation of the Main Variables:
	A: The Vandermonde matrix based on the x-coordinates of the data points. Each element of A[i][j] is x[i] raised to the power j.
	AT: The transpose of the Vandermonde matrix A.
	ATA: The product of AT and A, which is a square matrix used in the normal equation for least squares fitting.
	ATy: The product of AT and the vector y, used in the normal equation for least squares fitting.
	coeffs: The resulting polynomial coefficients after solving the normal equations.
	These comments and explanations should help you understand the purpose of each part of the code and the role of the variables.
*/

void polyfit(double* x, double* y, int size, int deg, double* coeffs) {
	int n = deg + 1; // Number of polynomial coefficients

	// Construct Vandermonde matrix A
	double** A = (double**)malloc(size * sizeof(double*));
	for (int i = 0; i < size; i++) {
		A[i] = (double*)malloc(n * sizeof(double));
		for (int j = 0; j < n; j++) {
			A[i][j] = pow(x[i], j); // Each element is x[i]^j
		}
	}

	// Transpose of A
	double** AT = (double**)malloc(n * sizeof(double*));
	for (int i = 0; i < n; i++) {
		AT[i] = (double*)malloc(size * sizeof(double));
	}
	transposeMatrices(A, AT, size, n);

	// ATA = AT * A
	double** ATA = (double**)malloc(n * sizeof(double*));
	for (int i = 0; i < n; i++) {
		ATA[i] = (double*)malloc(n * sizeof(double));
	}
	multiplyMatrices(AT, A, ATA, n, size, n);

	// ATy = AT * y
	double* ATy = (double*)malloc(n * sizeof(double));
	multiplyMatrixWithVector(AT, y, ATy, n, size);

	// Solve ATA * coeffs = ATy using Gaussian elimination
	gaussianElimination(ATA, ATy, coeffs, n);

	// Reverse the coefficients for consistency with polynomial representation
	for (int i = 0; i < n / 2; i++) {
		double temp = coeffs[i];
		coeffs[i] = coeffs[n - 1 - i];
		coeffs[n - 1 - i] = temp;
	}

	// Free dynamically allocated memory
	for (int i = 0; i < size; i++) { free(A[i]); }
	free(A);

	for (int i = 0; i < n; i++) { free(AT[i]); }
	free(AT);

	for (int i = 0; i < n; i++) { free(ATA[i]); }
	free(ATA);

	free(ATy);
}

} // end namespace