#include "sigmoid.h"

#include <math.h>

double sigmoid(double x, double x_val_at_y_eq_0p5=0.5, double steepness=10.0) {
	return 1.0 / (1.0 + exp(-fabs(steepness) * (x - x_val_at_y_eq_0p5)));
}

double sigmoid_n1_p1(double x, double steepness=5.7) {
	return 2.0 / (1.0 + exp(-fabs(steepness) * x)) - 1.0;
}
