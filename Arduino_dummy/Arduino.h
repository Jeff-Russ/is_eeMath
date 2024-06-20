#pragma once


#include <stdint.h>

// Define Arduino constants
#define HIGH 0x1
#define LOW  0x0
#define INPUT 0x0
#define OUTPUT 0x1

// Define Arduino types and functions
typedef unsigned long millis_t;

inline millis_t millis() {
	return 0;  // Dummy implementation
}

inline millis_t micros() {
	return 0;  // Dummy implementation
}

inline void delay(millis_t ms) {
	// Dummy implementation
}

// Add other necessary Arduino functions and types as needed