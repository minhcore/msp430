#ifndef BSP_GPIO_H
#define BSP_GPIO_H

// Include thư viện MSP430 và các thư viện khác
#include <msp430.h>
#include <stdint.h>

// Định nghĩa chân LED
#define LED_PIN BIT0
#define BUTTON_PIN BIT3


// Nguyên mẫu hàm (Function Prototypes)
void BSP_GPIO_Init(void);
void BSP_LED_Toggle(void);
void BSP_LED_On(void);
void BSP_LED_Off(void);


#endif
