#pragma once

/**
* @file freqAndTimeTools.h
* @brief math helpers related to frequency and time, such as for RC circuits
*/


/**
 * @brief use this function to determine how much an RC circuit has changed voltage in terms of tau.
 * Note that: 1) tau equals C * R  2) tau is not unitless: it is in seconds! 3) 1 * tau approx= 63.212% 
 * complete so return would be about 0.63212 4) even if you know the time elapsed, this function is 
 * useful in determining details about the circuit (solving for R or C) or to predict when the voltage 
 * will be mostly complete (>4*tau or so).
 * 
 * @param pcnt01 percent complete as 0...1, i.e. (v_now-v_start)/(v_goal-v_start)
 * @return double of how many tau have been traversed. 
 */
double nTauOfPcnt01(double pcnt01);

/**
 * @brief use this function to determine how much an RC circuit's output voltage has changed in terms 
 * percent as 0...1 given ntau, the number of tau that have progressed. 
 * Note that: 1) tau equals C * R  2) tau is not unitless: it is in seconds! 3) 1 * tau approx= 63.212% 
 * complete so return would be about 0.63212 
 * 
 * @param ntau number of tau that have progressed
 * @return double of percent complete as 0...1, i.e. (v_now-v_start)/(v_goal-v_start)
 */
double pcnt01OfnTau(double ntau);
