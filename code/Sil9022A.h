#pragma once

extern "C"
{
	#include "xgpiops.h"		// XGpioPs{}
}

#include "I2C.h"	// I2C{}

// =============================================================================
//	Reference:
//		"For each distinct base class that is specified virtual,
//		the most derived object contains only one base class subobject
//		of that type, even if the class appears many times in the
//		inheritance hierarchy (as long as it is inherited virtual every time)."
//	: https://en.cppreference.com/w/cpp/language/derived_class
// =============================================================================
class Sil9022A: virtual I2C
{
public:
	Sil9022A (void);
	
private:
	// ======================
	//  Single use functions
	// ======================
	XGpioPs XGpioPs_init (void) const;
	void raise_RESETn (void) const;
};
