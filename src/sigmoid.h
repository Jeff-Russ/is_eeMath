#pragma once

/**
 * @brief Sigmoid function based on 1 / (1 + exp(-x)), where output approaches 0 or 1 
 * when input approaches -infinity or +infinity, respectively.
 * 
 * @param x Input value.
 * @param x_val_at_y_eq_0p5 Output will be 0.5 at this x value.
 * @param steepness Increasing this pushes the output closer to 0 and 1 at the inputs 0 and 1, respectively.
 * @return double Result of the sigmoid function.
 */
double sigmoid(double x, double x_val_at_y_eq_0p5 = 0.5, double steepness = 10.0);

/**
 * @brief Sigmoid function based on 1 / (1 + exp(-x)), where output approaches 0 or 1 
 * when input approaches -infinity or +infinity, respectively, and scaled to range [-1, 1].
 * 
 * @param x Input value.
 * @param steepness Increasing this pushes the output closer to 0 and 1 at the inputs 0 and 1, respectively.
 * @return double Result of the scaled sigmoid function.
 */
double sigmoid_n1_p1(double x, double steepness = 5.7);


