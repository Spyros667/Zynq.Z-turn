#include "LM75BD.h"

#include <cstdio>	// printf()
#include <cstdint>	// uint8_t
#include <vector>	// vector{}

using namespace std;

constexpr uint8_t	LM75BD_address	= 0x49;

LM75BD::LM75BD (void)
	: I2C(LM75BD_address)
{
}
