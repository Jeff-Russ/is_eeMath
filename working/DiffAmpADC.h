#include <Arduino.h>


// #if !defined(uint8_t)
// typedef unsigned char uint8_t; 
// #endif

/*

---------------------------------------------------------------------------------------------------
n_adc <-> v_adc:
"For single ended conversion, the result is
         v_adc * 1024
n_adc = ───────────
         V_REF       "

so the max voltage, indicated by 1023, is actually: 
               n_adc            1023
v_adc = V_REF⋅ ──────  = V_REF⋅ ──────
               1024             1024
which means we can't detect the V_REF voltage itself accurately but we do have an accurate 
midpoint (relative to V_REF) as n_adc = 512, even though 512 is not the midpoint bw 0 and 1023. 

See for yourself:
               512
v_adc = V_REF⋅ ─────
               1024
      = V_REF/2 # exactly! Example:
      = 4.4/2   # ...we get 2.5 volts:
      = 2.2     # exactly!

BUT IN PRACTICE.... when the ADC gets v_adc = V_REF, analogRead() returns 1023 or 1022 and when it 
gets ADC gets v_adc = V_REF/2, analogRead() returns 510 or 511; I haven't seen it spit out 512 but 
maybe I need to calibrate 
---------------------------------------------------------------------------------------------------
v_diff (v_adc):
	The ADC is fed the voltage called v_diff, which is 
	V_REF (AREF) is set to 4.4V so that the midpoint is 2.2V, which indicates zero v_diff (now==goal)
	With exact resistor values (12k and 5k6), perfect op-amps and perfect calib, v_diff ranges from:

	0.289V  ...  2.2V  ...  4.111V
	when:        when:      when:
	goal=0V      goal==now  goal=4.095V
	now=4.095V              now=0V 

	if subtractor op-amp had a gain of 1, the range would be 2.2-4.095=-1.895 ...to...2.2+4.095=6.295,
	which is a span of 2*4.095=8.19 but instead we have a span of 4.111-0.289=3.822 so the gain is
	3.822/8.19=0.466666[6 is repeating]
---------------------------------------------------------------------------------------------------
n_adc -> v_diff
	0.289V  ...  2.2V  ...  4.111V
	is:          is:        is:
	67.25[81rep] 512        956.741[81rep]
	lets say:               lets say:
	67                      957 (or 956?)
	

*/

int pin = A0;
/**
	* Reads an ADC fed the output from an op-amp in differential amplifier configuration with a bias, 
	* performing the operation gain*(A + B) + zero_diff, where zero_diff is typically the ADC's 
	* AREF/2 such that when there is zero difference between A and B, the ADC receives the zero_diff 
	* voltage which is the 10-bit ADC's 512 value. See an example circuit in Falstad's Circuit Simulator: 
	* https://tinyurl.com/2c7m9oxu
	* 
	*
	*/
class DiffAmpADC {
 public:
	int _pin;
	int _n_tolerance;
	int _n_max_diff;
	int _n_zero_diff;
	int _n_min_diff;
	int n_prev;
	double norm_prev;

	DiffAmpADC(int pin=A0, int n_tolerance=3, int n_zero_diff=512, int n_min_diff=67, int n_max_diff=957) 
		: _pin{pin}, _n_tolerance{n_tolerance}, _n_zero_diff{n_zero_diff}, _n_min_diff{n_min_diff}, _n_max_diff{n_max_diff} {}
		
	
	double readNormalized () {
		n_prev = analogRead(_pin);
		double unbiased = n_prev - _n_zero_diff;

		if      (unbiased > _n_tolerance)  norm_prev = unbiased/(_n_max_diff-_n_zero_diff);
		else if (unbiased < -_n_tolerance) norm_prev = unbiased/(_n_min_diff-_n_zero_diff);
		else norm_prev = 0.0;
		return norm_prev;
	}

	int readN() { return n_prev = analogRead(_pin); }

	int setMaxDiffFromADC()  { return _n_max_diff  = n_prev = analogRead(_pin); }

	int setZeroDiffFromADC() { return _n_zero_diff = n_prev = analogRead(_pin); }

	int setMinDiffFromADC()  { return _n_min_diff  = n_prev = analogRead(_pin); }
};





















