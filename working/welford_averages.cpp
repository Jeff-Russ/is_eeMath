#include "welford_averages.h"

#include <cmath>
#include <cfloat>


class WelfordOnlineStats {
public:
	WelfordOnlineStats() : _mean(0.0), m2(0.0), count(0) {}

	void update(float new_value) {
		count++;
		float delta = new_value - _mean;
		_mean += delta / count;
		float delta2 = new_value - _mean;
		m2 += delta * delta2;
	}


	float mean() const {
		return _mean;
	}


	float variance() const {
		return (count > 1) ? (m2 / (count - 1)) : 0.0;
	}

	float stddev() const {
		return sqrt(variance());
	}

private:
	float _mean;   ///< Mean of the data points
	float m2;      ///< Sum of squared deviations from the mean
	unsigned long count; ///< Number of data points seen so far
};

class AdaptiveWelford {
public:
	AdaptiveWelford(float threshold = 2.0) : count(0), _mean(0.0), m2(0.0), threshold(threshold) {
		for(int i = 0; i < max_data_points; ++i) {
			data_points[i] = 0.0;
		}
	}

	void update(float new_value) {
		if(count < max_data_points) {
			data_points[count] = new_value;
			count++;
		} else {
			// If the buffer is full, we can either ignore new values or implement a sliding window
			// Here, we'll just replace the oldest value (simple FIFO buffer)
			for(int i = 0; i < max_data_points - 1; ++i) {
				data_points[i] = data_points[i + 1];
			}
			data_points[max_data_points - 1] = new_value;
		}
		recalculate();
	}

	void remove_outliers() {
		float current_mean = _mean;
		float current_stddev = stddev();
		for(int i = 0; i < count; ++i) {
			if(abs(data_points[i] - current_mean) > threshold * current_stddev) {
				for(int j = i; j < count - 1; ++j) {
					data_points[j] = data_points[j + 1];
				}
				count--;
				i--; // Re-check the new value at this index
			}
		}
		recalculate();
	}

	float mean() const {
		return _mean;
	}

	float variance() const {
		return (count > 1) ? (m2 / (count - 1)) : 0.0;
	}

	float stddev() const {
		return sqrt(variance());
	}

private:
	static const int max_data_points = 100; ///< Maximum number of data points to store
	float data_points[max_data_points];     ///< Buffer to store data points
	unsigned long count; ///< Number of data points seen so far
	float _mean;   ///< Mean of the data points
	float m2;      ///< Sum of squared deviations from the mean
	float threshold; ///< Threshold for outlier detection

	void recalculate() {
		_mean = 0.0;
		m2 = 0.0;
		for(int i = 0; i < count; ++i) {
			float delta = data_points[i] - _mean;
			_mean += delta / (i + 1);
			float delta2 = data_points[i] - _mean;
			m2 += delta * delta2;
		}
	}
};

class DecayingAverage {
public:
	DecayingAverage(float alpha = 0.5) : alpha(sigmoid(alpha)), average(0.0), datapoint_count(0), alpha_incr(0.0), min_datapoints_for_outlier(FLT_MAX), outlier_min_delta(0.0), outlier_max_delta(0.0) {}

	void setAlpha(float alpha, float alpha_incr) {
		this->alpha = sigmoid(alpha);
		setAlphaIncr(alpha_incr);
	}

	void setAlphaIncr(float alpha_incr) {
		if (alpha_incr == 0) {
			this->alpha_incr = 0.0;
		} else {
			this->alpha_incr = sigmoid(alpha_incr, true);
		}
	}

	void defineOutlierMinMax(float outlier_min_delta, float outlier_max_delta, unsigned long min_datapoints) {
		this->min_datapoints_for_outlier = min_datapoints;
		if (outlier_min_delta > outlier_max_delta) {
			// Invalid input; reset
			defineOutlierMinMax();
		} else {
			this->outlier_min_delta = outlier_min_delta;
			this->outlier_max_delta = outlier_max_delta;
		}
	}

	void clearAverage() {
		average = 0.0;
		datapoint_count = 0;
	}

	float getAverage() const {
		return average;
	}

	float accumulate(float num) {
		datapoint_count++;
		if (datapoint_count == 1) {
			average = num;
			return average;
		}
		if (alpha_incr) setAlpha(alpha + alpha_incr);

		float current_alpha = alpha;
		if (datapoint_count >= min_datapoints_for_outlier) {
			float delta = abs(num - average);
			if (delta > outlier_max_delta) {
				datapoint_count--;
				return average;
			}
			if (delta >= outlier_min_delta) {
				float scaler = (delta - outlier_min_delta) / (outlier_max_delta - outlier_min_delta);
				current_alpha *= scaler;
			}
		}
		average = current_alpha * num + (1 - current_alpha) * average;
		return average;
	}

private:
	float alpha;               ///< Factor applied to a new data point
	float average;             ///< Current decaying average
	unsigned long datapoint_count; ///< Number of data points processed
	float alpha_incr;          ///< Increment to adjust alpha
	unsigned long min_datapoints_for_outlier; ///< Minimum data points for outlier detection
	float outlier_min_delta;   ///< Minimum delta for outlier detection
	float outlier_max_delta;   ///< Maximum delta for outlier detection

	float sigmoid(float x, bool bipolar = false) const {
		if (!bipolar) {
			return 1.0 / (1.0 + exp(-10.0 * (x - 0.5)));
		} else {
			return 2.0 / (1.0 + exp(-5.7 * x)) - 1.0;
		}
	}

	void defineOutlierMinMax() {
		min_datapoints_for_outlier = FLT_MAX;
		outlier_min_delta = 0.0;
		outlier_max_delta = 0.0;
	}
};
