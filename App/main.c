#include <msp430.h>
#include "bsp_gpio.h"
#include "bsp_uart.h"
#include "bsp_pwm.h"
#include "bsp_system.h"
#include "app_console.h"
#include "app_servo.h"
#include "app_controller.h"


void main (void)
{
	WDTCTL = WDTPW + WDTHOLD;
	BSP_System_InitClock_16MHZ();
	App_Controller_Init();
	
        __bis_SR_register(GIE);
	while (1)
        {
			App_Console_Task();
		  
			if (BSP_10ms_Flag())
			{
				// auto delete Flag();
				App_Controller_Task();
			}
          
        }
	
		
	
}