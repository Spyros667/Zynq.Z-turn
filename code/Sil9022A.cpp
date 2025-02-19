#include "Sil9022A.h"

extern "C"
{
	#include "xparameters.h"	// XPAR_XGPIOPS_0_BASEADDR
	#include "xgpiops.h"		// XGpioPs{}
}

#include <cstdio>	// printf()
#include <cstdint>	// uint8_t
#include <vector>	// vector{}

using namespace std;

constexpr uint8_t	Sil9022A_address	= 0x3B;	// 0x76 >> 1
constexpr uint32_t	XGPIOPS_BASEADDR	= XPAR_XGPIOPS_0_BASEADDR;
constexpr uint32_t	Sil9022A_RESETn_pin	= 51;	// This is connected to PS as MIO51

Sil9022A::Sil9022A (void)
	: I2C(Sil9022A_address)
{
	vector<uint8_t> vtmp;

	raise_RESETn();

	// ================================================
	//  Enable TPI (Transmitter Programming Interface)
	//  Should be done right after RESET
	// ================================================
	write({0xC7, 0x00});	// Enable TPI (first thing after RESET)

	// ======================================================================================
	//  Wait for ID to stabilize. ID should be 0xB00203 (in addition to a reading from 0x30)
	// ======================================================================================
	do
	{
		vtmp = read(0x1B, 3);
	} while (vtmp[0] != 0xB0 or vtmp[1] != 0x02 or vtmp[2] != 0x03);

	// ==============================================
	//  Change [power] state, from D2 to D0 (active)
	// ==============================================
	//sil.write({0x1A, 0x10});	// Disable TMDS output (default)
	write({0x1E, 0x00});	// Change mode (D2 -> D0)
	//write({0x1A, 0x00});	// Enable TMDS output (DVI version)
	write({0x1A, 0x01});	// Enable TMDS output (HDMI version TODO does this matter? HDMI only? includes audio!)

	// =====================================================================================================================
	//  Enable "transmitter source termination" (this may be unnecessary, depending? on the schematic/ external resistors?)
	// =====================================================================================================================
	write({0xBC, 0x01, 0x82});	// TODO update with read({0xBC, 0x01, 0x82, 0xBE}, 1);
	vtmp = read(0xBE, 1);
	vtmp[0] |= 0x01;
	write({0xBE, vtmp[0]});
}

// ===========================
//  Open "RESETn" for writing
// ===========================
XGpioPs Sil9022A::XGpioPs_init (void) const
{
	XGpioPs_Config* ConfigPtr;
	ConfigPtr = XGpioPs_LookupConfig(XGPIOPS_BASEADDR);

	// =======
	//  Error
	// =======
	if (ConfigPtr == NULL)
	{
		printf("ERROR %d\r\n", __LINE__);
	}

	XGpioPs Gpio;
	int Status;
	Status = XGpioPs_CfgInitialize(&Gpio, ConfigPtr, ConfigPtr->BaseAddr);

	// =======
	//  Error
	// =======
	if (Status != XST_SUCCESS)
	{
		printf("%s:%d %d\r\n", __FILE__, __LINE__, Status);
	}

	return Gpio;
}

void Sil9022A::raise_RESETn (void) const
{
	// ===========================
	//  Open "RESETn" for writing
	// ===========================
	auto Gpio = XGpioPs_init();
	XGpioPs_SetDirectionPin(&Gpio, Sil9022A_RESETn_pin, 1);
	XGpioPs_SetOutputEnablePin(&Gpio, Sil9022A_RESETn_pin, 1);

	// ================
	//  Raise "RESETn"
	// ================
	XGpioPs_WritePin(&Gpio, Sil9022A_RESETn_pin, 1);
}
