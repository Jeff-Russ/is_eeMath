class WelfordOnlineStats:
	"""
	Class to maintain online statistics (mean and variance) using Welford's Algorithm.(Welford's Online Update)
	This algorithm is designed for online updates, meaning it incorporates data points one by one. 
	Due to the way it calculates the mean incrementally, there can be a slight difference compared 
	to the standard mean, especially for small datasets. This difference usually diminishes as more 
	data points are added.
	"""
	def __init__(self):
		"""Initializes the class with mean, second moment, and count set to 0."""
		self._mean = 0.0
		self.m2 = 0.0 # m2 (Second Moment) represents sum of squared deviations from the mean.
									# It's a mathematical concept used to calculate variance.
		self.count = 0

	def update(self, new_value):
		"""Updates the mean and variance with a new data point.
		Args:
			new_value: The new data point to incorporate."""
		self.count += 1
		delta = new_value - self._mean
		self._mean += delta / self.count
		delta2 = new_value - self._mean
		self.m2 += delta * delta2

	# @property
	def mean(self):
		return self._mean

	# @property
	def variance(self):
		"""Calculates and returns the variance based on the current data.

		Variance signifies how spread out the data points are in a dataset relative to their mean. 
		A higher variance indicates that the data points are further away from the mean on average, 
		while a lower variance suggests the data points are clustered closer to the mean.

		Variance will go down when inputs become more stable relative to the mean but never down all
		the way.

		Returns:
				The variance of the data stream seen so far (0 if less than 2 data points)."""
		return self.m2 / (self.count - 1) if self.count > 1 else 0.0
	
	# @property
	def stddev(self):
		return self.variance() ** 0.5


class AdaptiveWelford:
	'''
	Q: If there was a way to adapt this algo to give it the ability to remove past datapoints which deviate too much 
	after stability has been reached then variance could come down even further. Make sense? Do you think this is possible?

	chatGPT:

		Yes, it makes sense, and it is possible to adapt Welford's algorithm to allow for the removal of past data points 
		that deviate significantly. This would involve tracking each data point so that it can be removed later if necessary. 
		Here's an approach to achieve this:

		Track individual data points: Maintain a list of all data points along with their indices.
		Update the statistics: Use Welford's algorithm to update the mean and variance as new data points are added.
		Remove outliers: If a data point deviates too much from the current mean (using a threshold based on the standard deviation), 
		remove it and recalculate the mean and variance.
	'''
	def __init__(self, threshold=2.0):
		self.count = 0
		self._mean = 0.0
		self.m2 = 0.0
		self.data_points = []
		self.threshold = threshold

	def update(self, new_value):
		self.count += 1
		self.data_points.append(new_value)
		delta = new_value - self._mean
		self._mean += delta / self.count
		delta2 = new_value - self._mean
		self.m2 += delta * delta2

	def remove_outliers(self):
		mean = self._mean
		stddev = self.stddev()
		self.data_points = [x for x in self.data_points if abs(x - mean) <= self.threshold * stddev]
		self.recalculate()

	def recalculate(self):
		self.count = len(self.data_points)
		self._mean = 0.0
		self.m2 = 0.0
		for value in self.data_points:
			delta = value - self._mean
			self._mean += delta / self.count
			delta2 = value - self._mean
			self.m2 += delta * delta2

	def mean(self):
		return self._mean

	def variance(self):
		if self.count < 2:
			return float('nan')
		return self.m2 / (self.count - 1)

	def stddev(self):
		return self.variance() ** 0.5
	


class AdaptiveWelfordLimitedMemory:
	'''
	Q: What about on a machine with extremely limited memory (a microcontroller) where I would like to not 
	store anything but rather just fudge the self._mean to skew it toward being closer to incoming values 
	that appear to stabilize? 

	chatGPT:
	
	In a memory-constrained environment like a microcontroller, you can modify Welford's algorithm to adjust 
	the mean dynamically without storing past data points. You can "fudge" the mean towards new incoming 
	values that appear to stabilize, thereby achieving a similar effect to removing outliers. Here's an 
	approach that adapts the mean towards incoming values that are within a certain range of the current mean:

	Adaptive Mean Skew Algorithm

	1. Initialization: Start with initial values for the mean (M), variance (S), and the count of numbers processed (n).
	2. Update with each new data point:
			* Increment the count n.
			* Calculate the new mean and update it.
			* Update the variance using the new data point and the previously computed mean.
	3. Adjust the mean: If a new data point is within a certain range of the mean (e.g., within a few standard deviations), 
	update the mean. If it is outside this range, "fudge" the mean towards the new value without fully incorporating it.

	Here is an implementation in Python that demonstrates this approach:
	'''
	def __init__(self, threshold=2.0, alpha=0.3):
		self.n = 0
		self._mean = 0.0
		self.m2 = 0.0
		self.threshold = threshold
		self.alpha = alpha  # Fudge factor for mean adjustment

	def update(self, new_value):
		self.n += 1
		delta = new_value - self._mean
		self._mean += delta / self.n
		delta2 = new_value - self._mean
		self.m2 += delta * delta2

		# Adjust mean if new value is an outlier
		if abs(delta) > self.threshold * self.stddev():
			self._mean += self.alpha * delta

	def mean(self):
		return self._mean

	def variance(self):
		if self.n < 2:
			return float('nan')
		return self.m2 / (self.n - 1)

	def stddev(self):
		return self.variance() ** 0.5

def welfordTester(welford_obj=None, dataset=None):
	if welford_obj is None:
		welford_obj = WelfordOnlineStats()

	if dataset is None:
		dataset = [2, 2, 3, 3, 8, 20, 12, 12, 13, 12, 11, 12]

	for data in dataset:
		welford_obj.update(data)
		print(f"Data point: {data}, Mean: {welford_obj.mean():.2f}, Variance: {welford_obj.variance():.2f}, stddev: {welford_obj.stddev():.2f}")
		
from math import inf as infinity

class DecayingAverage:
		# self.average
		# self.datapoint_count

	def __init__(self, alpha=0.5):
		self.setAlpha(alpha)
		self.average = None
		self.datapoint_count = 0 # use this externally to avoid filtering outliers until datapoint_count is high enough
		self.setAlphaIncr(0)

	def setAlpha(self, alpha, alpha_inc):
		''' Sets factor applied to a newly datapoint number (num) on the average so that
		average becomes: alpha*num + (1-alpha)*average
		NOTE: self.alpha must be 0...1 and this is enforced by setting
		self.alpha = 1 / (1 + exp(-10 * (alpha - 0.5))) 
		'''
		self.alpha = self._sigmod(alpha)
		return self
	
	def setAlphaIncr(self, alpha_incr=0):
		''' Sets the amount that is added to self.alpha each time .accumlate, which is obviously 
		disabled if 0 is passed
		NOTE: self.alpha_incr must be -1...1 and this is enforced by setting
		self.alpha_incr = 2 / (1 + exp(-5 * (alpha_incr))) - 1
		'''
		if alpha_incr == 0: self.alpha_incr = 0
		else: self.alpha_incr = self._sigmod(alpha_incr, True)
			
		return self

	def defineOutlierMinMax(self, outlier_min_delta=None, outlier_max_delta=None, min_datapoints=infinity):
		'''
		arg1 (outlier_min_delta) and arg2 (outlier_max_delta): 
			These should be either both None (defaults) or both numbers where outlier_min_delta >= outlier_max_delta
		arg3: min_datapoints=infinity where this default means outliers will not be considered.
			This sets the value for self.min_datapoints_for_outlier.
			if min_datapoints < infinity, neither arg1 nor arg2 can be None
		How it works: when a number 'num' is passed to .accumlate(num) and there has been 
				>= self.min_datapoints_for_outlier datapoint so far, the num will be compared to the outlier
				min and max and if it is within or beyond this range (but not below min), we will: 
				1) not accumulate the number (or increment self.datapoint_count) into the average if |num-self.average| > outlier_max_delta
				2) temporarily multiply alpha by some number beween 1 and 0 
					if |num-self.average| < outlier_max_delta and |num-self.average| > outlier_min_delta 
					in a linear fasion
		'''
		self.min_datapoints_for_outlier = min_datapoints

		if outlier_min_delta is not None and outlier_max_delta is not None:

			if outlier_min_delta > outlier_max_delta:
				print('accumlateWCalcAlpha was called with outlier_max_delta < outlier_max_delta, which is invalid')
				return self.defineOutlierMinMax() # burn it all to the ground
			
			self.outlier_min_delta = outlier_min_delta
			self.outlier_max_delta = outlier_max_delta

		elif min_datapoints < infinity: # and outlier_min_delta is None or outlier_max_delta is None
			print('accumlateWCalcAlpha was called with min_datapoints < infinity ,which enables outlier detection')
			print('This is not allowed without setting outlier_min_delta and outlier_max_delta')
			
			return self.defineOutlierMinMax() # burn it all to the ground
		# else, either min or max are None and min_datapoints was set to < 0, which is fine
		
		return self

	def clearAverage(self):
		self.average = None
		self.datapoint_count = 0

	def getAverage(self):
		'''Get the current decaying average'''
		return self.average
	
	def accumulate(self, num):
		'''accumulates a new number to the decaying average (unless 
		self.datapoint_count > self.min_datapoints_for_outlier and |num-self.average| > outlier_max_delta )
		and returns that average. '''
		self.datapoint_count += 1

		if self.datapoint_count == 1: # handling first datapoint is simple: no alpha_incr or outliers
			self.average = num
			return self.average
		
		if self.alpha_incr: self.setAlpha(self.alpha + self.alpha_incr)
		alpha = self.alpha

		if self.datapoint_count >= self.min_datapoints_for_outlier: # (+1 because we haven't incremented it yet)
			delta = abs(num - self.average)

			if delta > self.outlier_max_delta: 
				self.datapoint_count -= 1 # we don't accumulate (or increment datapoint_count) if over the max
				return self.average

			if delta >= self.outlier_min_delta: # we are somewhere in the middle so we need to scale alpha down temporarily 
				scaler = (delta - self.outlier_min_delta) / (self.outlier_max_delta - self.outlier_min_delta)
				delta = scaler * alpha 

		self.average = alpha * num + (1 - alpha) * self.average  # Optimized for readability
		return self.average
		
	def _sigmod(x, bipolar=False):
		from math import exp as math_exp
		if not bipolar:
			return 1 / (1 + math_exp(-10 * (x - 0.5))) 
		else:
			return 2 / (1 + math_exp(-5.7 * (x))) - 1
