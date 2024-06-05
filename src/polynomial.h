#pragma once

/**
 * @file polynomial.h
 * @brief polynomial fitting and related functions
 */

namespace is {

/**
 * @brief Least-squares fit of a polynomial to data
 * 
 * @param x The array of x-coordinates of the sample points
 * @param y The array of y-coordinates of the sample points
 * @param size The number of sample points
 * @param deg The degree of the fitting polynomial
 * @param coeffs The array to store the resulting polynomial coefficients
 */
void polyfit(double* x, double* y, int size, int deg, double* coeffs);

} // end namespace
