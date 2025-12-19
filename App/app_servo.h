#ifndef APP_SERVO_H
#define APP_SERVO_H

#include "bsp_pwm.h"
#include <stdint.h>

typedef struct 
{
	uint16_t min_pulse_us;
	uint16_t max_pulse_us;
	uint16_t max_angle;
	Servo_ID_t pwm_channel_id;
	uint8_t currentAngle;
} Servo_Handle_t;

void App_Servo_Init(Servo_Handle_t *servo, Servo_ID_t pwm_channel);

void App_Servo_SetAngle(Servo_Handle_t *servo, uint8_t angle);

#endif