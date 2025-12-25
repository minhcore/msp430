#include <msp430.h>
#include "bsp_gpio.h"



void main (void)
{
	WDTCTL = WDTPW + WDTHOLD;
	BSP_GPIO_Init();
	
	volatile int i;
	
        __bis_SR_register(GIE);
	while (1)
        {
			__delay_cycles(1000000);
			BSP_LED_Toggle();
			
			
          
        }
	
		
	
}
