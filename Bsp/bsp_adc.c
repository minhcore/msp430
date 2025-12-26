#include "bsp_adc.h"
#include <msp430.h>

// Biến lưu kết quả đo

volatile uint16_t adc_buf[9];
volatile uint8_t adc_idx = 0;
volatile uint8_t adc_ready;

void BSP_ADC_Init(void)
{
	// Xem datasheet ADC10bit
	
	// 1. Cấu hình ADC10CTL0
	ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE + MSC;
	
	// 2. Cấu hình ADC10CTL1
	ADC10CTL1 = INCH_7 + ADC10DIV_3 + CONSEQ_1; // chia clock cho 3 để ADC chạy chậm, ổn định
	
	ADC10AE0 = BIT6 + BIT7;
	
	// Bật delay để điện áp tham chiếu ổn định trước khi đo (khoảng 30 us)
	__delay_cycles(100);
	
}

void BSP_ADC_StartConversion(void)
{
	adc_idx = 0;
	adc_ready = 0;
	ADC10CTL0 &= ~ENC;
	ADC10CTL0 |= ENC + ADC10SC; // Bật enable và bắt đầu
	
}

uint16_t BSP_ADC_GetLastValue(uint8_t pin)
{
	return adc_buf[pin];
}
uint8_t BSP_ADC_IsDataReady(void)
{
	return adc_ready;
}

void BSP_ADC_ClearFlag(void)
{
	adc_ready = 0;
}

// Vector ngắt ADC
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
	adc_buf[adc_idx++] = ADC10MEM;
	
	if (adc_idx >= 8)
	{
		adc_ready = 1;
		ADC10CTL0 &= ~ENC;
		adc_idx = 0;
	}
	
	
	// Đánh thức CPU
	//__bic_SR_register_on_exit(LPM0_bits);
}
