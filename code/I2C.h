#pragma once

extern "C"
{
	#include "xparameters.h"	// XPAR_XIICPS_0_BASEADDR

	#include "xiicps.h"         // XIicPs{}
}

#include <cstdint>	// uint8_t
#include <vector>	// vector{}

class I2C
{
public:
	I2C (const uint8_t& SLAVE_ADDRESS);

	// ====================================================
	//  Perform a burst I2C write at register address v[0]
	// ====================================================
	void write (const std::vector<uint8_t>& v);

	// ===========================================
	//  Perform a burst I2C read of size "size"
	//  Vector "v" or "reg_addr" is written first
	// ===========================================
	std::vector<uint8_t> read (const std::vector<uint8_t>& v, const uint32_t& size);
	std::vector<uint8_t> read (const uint8_t& reg_addr, const uint32_t& size);
	
protected:
	// =====================
	//  Auxiliary functions
	// =====================
	void print_vec (const std::vector<uint8_t>& v) const;
	void print_vec (const uint8_t& reg, const std::vector<uint8_t>& v) const;

	// ==================
	//  Common functions
	// ==================
	void wait (void) const;	

	// ===========
	//  Variables
	// ===========
	XIicPs Iic;
	const uint8_t SLAVE_ADDRESS;
	
private:
	// ======================
	//  Single use functions
	// ======================
	XIicPs XIicPs_init (void);
};
