#include "matrices.h"

#include <math.h>

namespace is {

void transposeMatrices(double** src, double** dest, int n, int m) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			dest[j][i] = src[i][j]; // Transpose element (i, j) to (j, i)
		}
	}
}

void multiplyMatrices(double** A, double** B, double** C, int n, int m, int p) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < p; j++) {
			C[i][j] = 0; // Initialize result matrix element to 0
			for (int k = 0; k < m; k++) {
				C[i][j] += A[i][k] * B[k][j]; // Sum of products
			}
		}
	}
}

void multiplyMatrixWithVector(double** A, double* x, double* y, int n, int m) {
	for (int i = 0; i < n; i++) {
		y[i] = 0; // Initialize result vector element to 0
		for (int j = 0; j < m; j++) {
			y[i] += A[i][j] * x[j]; // Sum of products
		}
	}
}

void gaussianElimination(double** A, double* b, double* coeffs, int n) {
	for (int i = 0; i < n; i++) {
		int max_row = i; // Index of the row with the largest pivot element
		for (int k = i + 1; k < n; k++) {
			if (fabs(A[k][i]) > fabs(A[max_row][i])) {
				max_row = k;
			}
		}

		// Swap rows to make the largest pivot element the current row
		double* temp_row = A[i];
		A[i] = A[max_row];
		A[max_row] = temp_row;

		double temp_b = b[i];
		b[i] = b[max_row];
		b[max_row] = temp_b;

		// Eliminate entries below the pivot
		for (int k = i + 1; k < n; k++) {
			double factor = A[k][i] / A[i][i];
			b[k] -= factor * b[i];
			for (int j = i; j < n; j++) {
				A[k][j] -= factor * A[i][j];
			}
		}
	}

	// Back substitution to solve for coefficients
	for (int i = n - 1; i >= 0; i--) {
		coeffs[i] = b[i] / A[i][i];
		for (int k = i - 1; k >= 0; k--) {
			b[k] -= A[k][i] * coeffs[i];
		}
	}
}

} // end namespace
