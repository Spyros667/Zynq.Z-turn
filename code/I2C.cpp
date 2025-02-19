#include "I2C.h"

extern "C"
{
	#include "xiicps.h"		// XIic_Send(), XIic_Recv()
	#include "sleep.h"		// usleep()
}

#include <cstdio>	// printf()
#include <cstdint>	// uint8_t
#include <vector>	// vector{}

using namespace std;

constexpr uint32_t XIICPS_BASEADDRESS	= XPAR_XIICPS_0_BASEADDR;
constexpr uint32_t IIC_SCLK_RATE		= 100e3;

I2C::I2C (const uint8_t& SLAVE_ADDRESS)
	: Iic(XIicPs_init()), SLAVE_ADDRESS(SLAVE_ADDRESS)
{
}

void I2C::write (const vector<uint8_t>& v)
{
	// ===============================
	//	Wait for [I2C] bus to be free
	// ===============================
	while (XIicPs_BusIsBusy(&Iic))
	{
		//wait();
	}

	// =================================
	//  (using blocked I/O and polling)
	// =================================
	int Status;
	Status = XIicPs_MasterSendPolled(&Iic, const_cast<uint8_t*>(v.data()), v.size(), SLAVE_ADDRESS);	// Is const_cast ugly here, or is it just me!?

	// =======
	//  Error
	// =======
	if (Status != XST_SUCCESS)
	{
		printf("%s:%d %d\r\n", __FILE__, __LINE__, Status);
	}
}

vector<uint8_t> I2C::read (const vector<uint8_t>& v, const uint32_t& size)
{
	// =======================
	//  Send register address
	//  Send page w/ offset
	// =======================
	write(v);

	// ===============================
	//	Wait for [I2C] bus to be free
	// ===============================
	while (XIicPs_BusIsBusy(&Iic))
	{
		//wait();
	}

	// =================================
	//  (using blocked I/O and polling)
	// =================================
	vector<uint8_t> result(size);
	int Status;
	Status = XIicPs_MasterRecvPolled(&Iic, result.data(), result.capacity(), SLAVE_ADDRESS);

	// =======
	//  Error
	// =======
	if (Status != XST_SUCCESS)
	{
		printf("%s:%d %d\r\n", __FILE__, __LINE__, Status);
	}

	// ==============
	//  (Debug only)
	// ==============
	print_vec(v[0], result);
	
	// ===================
	//  Return burst read
	// ===================
	return result;
}

// ========================
//  (Overloading function)
// ========================
vector<uint8_t> I2C::read (const uint8_t& reg_addr, const uint32_t& size)
{
	return read(vector<uint8_t>{reg_addr}, size);
}

XIicPs I2C::XIicPs_init (void)
{
	XIicPs_Config* ConfigPtr;
	ConfigPtr = XIicPs_LookupConfig(XIICPS_BASEADDRESS);

	// =======
	//  Error
	// =======
	if (ConfigPtr == NULL)
	{
		printf("ERROR %d\r\n", __LINE__);
	}

	XIicPs Iic;
	int Status;
	Status = XIicPs_CfgInitialize(&Iic, ConfigPtr, ConfigPtr->BaseAddress);

	// =======
	//  Error
	// =======
	if (Status != XST_SUCCESS)
	{
		printf("%s:%d %d\r\n", __FILE__, __LINE__, Status);
	}

	// ====================================================================
	//	Run a self-test.
	//	From the API:
	//		"The self-test is destructive in that a reset of the device is
	//		performed in order to check the reset values of the registers
	//		and to get the device into a known state."
	// ====================================================================
	Status = XIicPs_SelfTest(&Iic);

	// =======
	//  Error
	// =======
	if (Status != XST_SUCCESS)
	{
		printf("%s:%d %d\r\n", __FILE__, __LINE__, Status);
	}

	// ==================================
	//  Set the [I2C] clock rate (in Hz)
	// ==================================
	Status = XIicPs_SetSClk(&Iic, IIC_SCLK_RATE);

	// =======
	//  Error
	// =======
	if (Status != XST_SUCCESS)
	{
		printf("%s:%d %d\r\n", __FILE__, __LINE__, Status);
	}

	return Iic;
}

// ================================================================================
//  Provide a common "waiting" interval, elliminating the changes in.. intervals 😌
// ================================================================================
void I2C::wait (void) const
{
	usleep(1e3);
}

// ==================
//  Helper functions
//  For example:
//    0A0BFF
// ==================
void I2C::print_vec (const vector<uint8_t>& v) const
{
	for (const auto& byte: v)
	{
		printf("%02x ", byte);
	}
	printf("\r\n");
}

// ==================
//  Helper functions
//  For example:
//    A1h: 0A0BFF
// ==================
void I2C::print_vec (const uint8_t& reg, const vector<uint8_t>& v) const
{
	printf("%02Xh: ", reg);
	print_vec(v);
}
