#include "app_servo.h"



void App_Servo_Init(Servo_Handle_t *servo, Servo_ID_t pwm_channel)
{
	servo->min_pulse_us = 500;
	servo->max_pulse_us = 2450;
	servo->max_angle = 180;
	servo->pwm_channel_id = pwm_channel;
}

void App_Servo_SetAngle(Servo_Handle_t *servo, uint8_t angle)
{
	// safety first
	if (angle > servo->max_angle)
	{
		angle = servo->max_angle;
	}
	
	uint32_t pulse_range = servo->max_pulse_us - servo->min_pulse_us;
	
	uint32_t target_pulse = servo->min_pulse_us + ((uint32_t)angle * pulse_range) / servo->max_angle;
	
	BSP_PWM_SetPulse_Us(servo->pwm_channel_id, (uint16_t)target_pulse);
	
}