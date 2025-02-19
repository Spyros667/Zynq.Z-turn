#pragma once

#include "I2C.h"	// I2C{}

// =============================================================================
//	Reference:
//		"For each distinct base class that is specified virtual,
//		the most derived object contains only one base class subobject
//		of that type, even if the class appears many times in the
//		inheritance hierarchy (as long as it is inherited virtual every time)."
//	: https://en.cppreference.com/w/cpp/language/derived_class
// =============================================================================
struct LM75BD: virtual I2C
{
public:
	LM75BD (void);
};
