#ifndef BSP_BUTTON_H
#define BSP_BUTTON_H

#include "bsp_gpio.h"
#include <stdint.h>


typedef struct
{
	uint8_t pinID;
	uint8_t history;
	uint8_t stable_state;
	uint8_t last_stable_state;
} Button_t;

// khai báo các hàm
void BSP_Button_Init(void);

// Hàm public - hàm cho Timer A0 gọi
void BSP_Button_Driver_Scan_All(void);

// Hàm cho App gọi
uint8_t BSP_Button_Press_Event(void);


#endif


