#ifndef BSP_ADC_H
#define BSP_ADC_H

#include <stdint.h>

void BSP_ADC_Init(void);

// Hàm bắt đầu đo (Trigger)
void BSP_ADC_StartConversion(void);

// Hàm lấy kết quả đo được (gọi từ App)
uint16_t BSP_ADC_GetLastValue(void);

// Hàm kiểm tra đo xong chưa 
uint8_t BSP_ADC_IsDataReady(void);

#endif