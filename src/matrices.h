#pragma once

/**
 * @file matrices.h
 * @brief mathematical operations involving matrices (implemented as C arrays/pointers)
 */

namespace is {

/**
 * @brief Transposes a matrix
 * 
 * @param src The source matrix (n x m)
 * @param dest The destination matrix (m x n)
 * @param n The number of rows in the source matrix (src is src[0]...src[n-1])
 * @param m The number of columns in the source matrix 
 */
void transposeMatrices(double** src, double** dest, int n, int m);

/**
 * @brief Multiplies two matrices
 * 
 * @param A The first matrix (n x m)
 * @param B The second matrix (m x p)
 * @param C The resulting matrix (n x p)
 * @param n The number of rows in the first matrix
 * @param m The number of columns in the first matrix and rows in the second matrix
 * @param p The number of columns in the second matrix
 */
void multiplyMatrices(double** A, double** B, double** C, int n, int m, int p);

/**
 * @brief Multiplies a matrix with a vector
 * 
 * @param A The matrix (n x m)
 * @param x The vector (m)
 * @param y The resulting vector (n)
 * @param n The number of rows in the matrix
 * @param m The number of columns in the matrix
 */
void multiplyMatrixWithVector(double** A, double* x, double* y, int n, int m);

/**
 * @brief Solves a system of linear equations using Gaussian elimination
 * 
 * @param A The matrix of coefficients (n x n)
 * @param b The vector of constants (n)
 * @param coeffs The vector to store the resulting coefficients (n)
 * @param n The number of equations (and unknowns)
 */
void gaussianElimination(double** A, double* b, double* coeffs, int n);

} // end namespace
