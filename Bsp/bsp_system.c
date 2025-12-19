#include "bsp_system.h"

void BSP_System_InitClock_16MHZ(void)
{
	if (CALBC1_16MHZ == 0xFF)
	{
		while(1); // Nếu chip bị mất calibration data
	}
	
	DCOCTL = 0;
	BCSCTL1 = CALBC1_16MHZ; // Set range
	DCOCTL = CALDCO_16MHZ; // Set DCO step
}