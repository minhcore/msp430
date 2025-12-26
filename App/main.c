#include <msp430.h>
#include "bsp_timer.h"
#include "bsp_adc.h"
#include "bsp_system.h"
#include "bsp_gpio.h"
#include "app_sensor.h"



void main (void)
{
	WDTCTL = WDTPW + WDTHOLD;
	BSP_System_InitClock_16MHZ();
	BSP_Timer_Init();
	BSP_GPIO_Init();
	
	App_Sensor_Init();
	
    __bis_SR_register(GIE);

	while (1)
        {
		App_Sensor_Run();
			

          
        }
}
