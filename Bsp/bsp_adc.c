#include "bsp_adc.h"
#include <msp430.h>

// Biến lưu kết quả đo
static volatile uint16_t g_adc_result = 0;
static volatile uint8_t g_data_ready = 0;

void BSP_ADC_Init(void)
{
	// Xem datasheet ADC10bit
	
	// 1. Cấu hình ADC10CTL0
	ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON + ADC10IE;
	
	// 2. Cấu hình ADC10CTL1
	ADC10CTL1 = INCH_10 + ADC10DIV_3; // chia clock cho 3 để ADC chạy chậm, ổn định
	
	// Bật delay để điện áp tham chiếu ổn định trước khi đo (khoảng 30 us)
	__delay_cycles(100);
	
}

void BSP_ADC_StartConversion(void)
{
	g_data_ready = 0; // Xóa cờ cũ
	ADC10CTL0 |= ENC + ADC10SC; // Bật enable và bắt đầu
	
}

uint16_t BSP_ADC_GetLastValue(void)
{
	g_data_ready = 0; // Đọc xong thì xóa cờ
	return g_adc_result;
}
uint8_t BSP_ADC_IsDataReady(void)
{
	return g_data_ready;
}

// Vector ngắt ADC
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
	g_adc_result = ADC10MEM; 
	g_data_ready = 1; // Báo cờ cho App biết
	
	// Tắt bit ENC để tiết kiệm điện
	ADC10CTL0 &= ~ENC;
	
	// Đánh thức CPU
	//__bic_SR_register_on_exit(LPM0_bits);
}