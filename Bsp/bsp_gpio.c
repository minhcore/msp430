#include "bsp_gpio.h"
#include <stdint.h>
#include <msp430.h>


void BSP_GPIO_Init(void)
{
	// Cấu hình P1.0 là Output
	P1DIR |= LED_PIN; // P1DIR là thanh ghi hướng (1 = Out, 0 = In)
	
	// Tắt LED ban đầu
	P1OUT &= ~LED_PIN;
	
	// Cấu hình BUTTON (P1.3)
	P1DIR &= ~BUTTON_PIN;
	P1REN |= BUTTON_PIN;
	P1OUT |= BUTTON_PIN;
	
	// Configure p1.6 analog input
	
	
	
	
}

void BSP_LED_Toggle(void)
{
	// Đảo trạng thái bit (XOR)
	P1OUT ^= LED_PIN;
}
void BSP_LED_On(void)
{
	P1OUT |= LED_PIN;
}
void BSP_LED_Off(void)
{
	P1OUT &= ~LED_PIN;
}






