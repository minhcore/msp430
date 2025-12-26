#include "bsp_adc.h"
#include "bsp_uart.h"
#include "bsp_gpio.h"
#include "bsp_timer.h"
#include "bsp_uart.h"
#include "app_sensor.h"
#include <msp430.h>

volatile uint16_t x_value_raw;
volatile uint16_t y_value_raw;


void PrintNumber(uint16_t num)
{
    char buf[6]; 
    char *ptr = &buf[5];
    *ptr = '\0';
    
   
    do {
        *--ptr = (num % 10) + '0';
        num /= 10;
    } while (num != 0);
    
    BSP_UART_PutString(ptr);
}


void App_Sensor_Init(void)
{
	BSP_ADC_Init(); 
	BSP_UART_Init(); 
	BSP_UART_PutString("System Ready!\r\n");
}

void App_Sensor_Run(void)
{
	uint32_t currentTick = BSP_GetTick();
	
	
	static uint32_t lastMeasureTime = 0;
	
	if (currentTick - lastMeasureTime >= 25)
	{
		lastMeasureTime = currentTick;
		BSP_ADC_StartConversion(); 
	}
	
	if (BSP_ADC_IsDataReady())
	{
		x_value_raw = BSP_ADC_GetLastValue(X_PIN);
		BSP_UART_PutString("X value: ");
		PrintNumber(x_value_raw);
		
		y_value_raw = BSP_ADC_GetLastValue(Y_PIN);
		BSP_UART_PutString(" | Y value: ");
		PrintNumber(y_value_raw);
		BSP_UART_PutString("\r\n");
		
		BSP_ADC_ClearFlag();
	}
	
}
