#include "platform.h"
#include "Sil9022A.h"	// Sil9022A{}
#include "LM75BD.h"		// LM75BD{}
#include <cstdio>		// printf()

extern "C"
{
	#include "sleep.h"	// usleep()
}

int main()
{
	init_platform();
	printf("Hellow World\r\n");

	// ===========================================
	//  Initialize/ Power up the HDMI transmitter
	// ===========================================
	Sil9022A sil;

	LM75BD lm;

	while (true)
	{
		lm.read({0x00}, 2);
		usleep(1300000);
	}

	printf("Successfully ran Hellow World application\r\n");
	cleanup_platform();

	return 0;
}
