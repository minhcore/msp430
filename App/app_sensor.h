#ifndef APP_SENSOR_H
#define APP_SENSOR_H

// Include thư viện MSP430 và các thư viện khác
#include <msp430.h>
#include <stdint.h>

#define X_PIN 0
#define Y_PIN 1

// Hàm khởi tạo trạng thái ban đầu
void App_Sensor_Init(void);

// Hàm thực thi chính
void App_Sensor_Run(void);

#endif
