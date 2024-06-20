#pragma once

/**
 * @brief Class to maintain online statistics (mean and variance) using Welford's Algorithm.
 * 
 * This algorithm is designed for online updates, meaning it incorporates data points one by one. 
 * Due to the way it calculates the mean incrementally, there can be a slight difference compared 
 * to the standard mean, especially for small datasets. This difference usually diminishes as more 
 * data points are added.
 */
class WelfordOnlineStats {
public:
	WelfordOnlineStats();

	/**
	 * @brief Updates the mean and variance with a new data point.
	 * @param new_value The new data point to incorporate.
	 */
	void update(float new_value);

	/**
	 * @brief Gets the mean of the data stream seen so far.
	 * @return The mean of the data stream.
	 */
	float mean() const;

	/**
	 * @brief Calculates and returns the variance based on the current data.
	 * 
	 * @return The variance of the data stream seen so far (0 if less than 2 data points).
	 */
	float variance() const;

	/**
	 * @brief Calculates and returns the standard deviation based on the current data.
	 * 
	 * @return The standard deviation of the data stream seen so far.
	 */
	float stddev() const;

private:
	float _mean;   ///< Mean of the data points
	float m2;      ///< Sum of squared deviations from the mean
	unsigned long count; ///< Number of data points seen so far
};

/**
 * @brief Class to maintain adaptive online statistics (mean and variance) with outlier removal using Welford's Algorithm.
 */
class AdaptiveWelford {
public:
	AdaptiveWelford(float threshold = 2.0);

	/**
	 * @brief Updates the mean and variance with a new data point.
	 * @param new_value The new data point to incorporate.
	 */
	void update(float new_value);

	/**
	 * @brief Removes outliers based on the threshold and recalculates the statistics.
	 */
	void remove_outliers();

	/**
	 * @brief Gets the mean of the data stream seen so far.
	 * @return The mean of the data stream.
	 */
	float mean() const;

	/**
	 * @brief Calculates and returns the variance based on the current data.
	 * 
	 * @return The variance of the data stream seen so far (0 if less than 2 data points).
	 */
	float variance() const;

	/**
	 * @brief Calculates and returns the standard deviation based on the current data.
	 * 
	 * @return The standard deviation of the data stream seen so far.
	 */
	float stddev() const;

private:
	static const int max_data_points = 100; ///< Maximum number of data points to store
	float data_points[max_data_points];     ///< Buffer to store data points
	unsigned long count; ///< Number of data points seen so far
	float _mean;   ///< Mean of the data points
	float m2;      ///< Sum of squared deviations from the mean
	float threshold; ///< Threshold for outlier detection

	/**
	 * @brief Recalculates the mean and variance from the stored data points.
	 */
	void recalculate();
};

/**
 * @brief Class to maintain a decaying average for a series of data points.
 */
class DecayingAverage {
public:
	DecayingAverage(float alpha = 0.5);

	/**
	 * @brief Sets factor applied to a new data point number (num) on the average.
	 * @param alpha The factor to apply.
	 * @param alpha_incr The increment to adjust alpha.
	 */
	void setAlpha(float alpha, float alpha_incr);

	/**
	 * @brief Sets the amount that is added to alpha each time accumulate is called.
	 * @param alpha_incr The increment to adjust alpha.
	 */
	void setAlphaIncr(float alpha_incr);

	/**
	 * @brief Defines the outlier detection parameters.
	 * @param outlier_min_delta Minimum delta for outlier detection.
	 * @param outlier_max_delta Maximum delta for outlier detection.
	 * @param min_datapoints Minimum data points for outlier detection.
	 */
	void defineOutlierMinMax(float outlier_min_delta, float outlier_max_delta, unsigned long min_datapoints);

	/**
	 * @brief Clears the current average and resets the count of data points.
	 */
	void clearAverage();

	/**
	 * @brief Gets the current decaying average.
	 * @return The current decaying average.
	 */
	float getAverage() const;

	/**
	 * @brief Accumulates a new number to the decaying average and returns that average.
	 * @param num The new data point to incorporate.
	 * @return The updated decaying average.
	 */
	float accumulate(float num);

private:
	float alpha;               ///< Factor applied to a new data point
	float average;             ///< Current decaying average
	unsigned long datapoint_count; ///< Number of data points processed
	float alpha_incr;          ///< Increment to adjust alpha
	unsigned long min_datapoints_for_outlier; ///< Minimum data points for outlier detection
	float outlier_min_delta;   ///< Minimum delta for outlier detection
	float outlier_max_delta;   ///< Maximum delta for outlier detection

	/**
	 * @brief Sigmoid function.
	 * @param x Input value.
	 * @param bipolar Whether to use bipolar sigmoid.
	 * @return Sigmoid function result.
	 */
	float sigmoid(float x, bool bipolar = false) const;

	/**
	 * @brief Reset the outlier settings to default values.
	 */
	void defineOutlierMinMax();
};

