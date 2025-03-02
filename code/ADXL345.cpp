#include "ADXL345.h"

#include <cstdio>	// printf()
#include <cstdint>	// uint8_t
#include <vector>	// vector{}

using namespace std;

constexpr uint8_t	ADXL345_address	= 0x53;

// =============
//  Constructor
// =============
ADXL345::ADXL345 (void)
	: I2C(ADXL345_address)
{
	// ===================
	//  Setup data format
	// ===================
	write({0x31, 0x00});

	// ==========
	//  Power up
	// ==========
	write({0x2D, 1 << 3});
}
