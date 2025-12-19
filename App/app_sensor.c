#include "bsp_adc.h"
#include "bsp_uart.h"
#include "bsp_gpio.h"
#include "bsp_timer.h"
#include "app_sensor.h"
#include <msp430.h>

volatile uint16_t g_currentTempRaw = 0; // Biến lưu giá trị thô ADC

// Hàm phụ trợ: Chuyển số nguyên thành chuỗi và gửi đi
void PrintNumber(uint16_t num)
{
    char buf[6]; // Buffer tạm (tối đa 5 chữ số + null)
    char *ptr = &buf[5];
    *ptr = '\0';
    
    // Thuật toán chia lấy dư để tách số
    do {
        *--ptr = (num % 10) + '0';
        num /= 10;
    } while (num != 0);
    
    BSP_UART_PutString(ptr);
}


void App_Sensor_Init(void)
{
	BSP_ADC_Init(); // Khởi tạo ADC
	BSP_UART_Init(); // Khởi tạo UART
	BSP_UART_PutString("System Ready!\r\n");
}

void App_Sensor_Run(void)
{
	uint32_t currentTick = BSP_GetTick();
	
	// Đo nhiệt độ (mỗi giây 1 lần)
	static uint32_t lastMeasureTime = 0;
	
	if (currentTick - lastMeasureTime >= 1000)
	{
		lastMeasureTime = currentTick;
		BSP_ADC_StartConversion(); // Ra lệnh đo
	}
	
	// Xử lý kết quả
	if (BSP_ADC_IsDataReady()) 
	{
				
		g_currentTempRaw = BSP_ADC_GetLastValue();
		BSP_UART_PutString("ADC RAW: ");
		PrintNumber(g_currentTempRaw);
		BSP_UART_PutString("\r\n");
				
                
    }
	
}