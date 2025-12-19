#ifndef BSP_PWM_H
#define BSP_PWM_H

#include "bsp_gpio.h"
#include "bsp_timer.h"
#include <stdint.h>

#define PWM_PERIOD_COUNT 39999
#define PAN_PIN BIT1

typedef enum 
{
	SERVO_ID_PAN = 0,
	SERVO_ID_TILT,
	SERVO_ID_MAX
} Servo_ID_t;

void BSP_PWM_Init(Servo_ID_t servo_id);

void BSP_PWM_SetDuty(uint8_t percent);

void BSP_PWM_SetPulse_Us(Servo_ID_t servo_id, uint16_t us_value);

#endif

