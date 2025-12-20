#ifndef BSP_TIMER_H
#define BSP_TIMER_H

#include <msp430.h>
#include "bsp_gpio.h"
#include "bsp_button.h"
#include <stdint.h>

void BSP_Timer_Init(void);
uint32_t BSP_GetTick(void);
uint8_t BSP_10ms_Flag(void);


#endif


