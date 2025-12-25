#include <msp430.h>
#include "bsp_gpio.h"



void main (void)
{
	WDTCTL = WDTPW + WDTHOLD;
	BSP_System_InitClock_16MHZ();
	BSP_GPIO_Init();
	
	
	
        __bis_SR_register(GIE);
	while (1)
        {
			__delay_cycles(16000000);
			BSP_LED_Toggle();
			
			
          
        }
	
		
	
}
