#include "app_servo.h"
#include <stdlib.h> // Use abs(), labs()



void App_Servo_Init(Servo_Handle_t *servo, Servo_ID_t pwm_channel)
{
	servo->min_pulse_us = 500;
	servo->max_pulse_us = 2450;
	servo->max_angle = 180;
	servo->min_angle = 0;
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

void App_Servo_UpdatePWM(Servo_Handle_t *servo)
{
	if (servo->current_angle > TO_FIXED(servo->max_angle)) servo->current_angle = TO_FIXED(servo->max_angle);
	else if (servo->current_angle < TO_FIXED(servo->min_angle)) servo->current_angle = TO_FIXED(servo->min_angle);
	
	uint32_t pulse_range = servo->max_pulse_us - servo->min_pulse_us;
	
	uint32_t target_pulse = servo->min_pulse_us + (servo->current_angle * pulse_range) / (servo->max_angle << SHIFT_AMOUNT);
	
	if (target_pulse > servo->max_pulse_us) target_pulse = servo->max_pulse_us;
	else if (target_pulse < servo->min_pulse_us) target_pulse = servo->min_pulse_us;
	
	BSP_PWM_SetPulse_Us(servo->pwm_channel_id, (uint16_t)target_pulse);
}

void App_Servo_Process(Servo_Handle_t *servo)
{
	// 1. Tính toán vị trị lỗi
	int32_t distance = servo->target_angle - servo->current_angle;
	
	// 2. Tính quãng đường phanh
	int32_t v_square = (int32_t)servo->v_current * servo->v_current;
	int32_t s_brake = v_square / (2 * servo->deceleration);
	
	// 3. Tính vận tốc

	if (labs(distance) > s_brake)
	{
		if (distance > 0) servo->v_target = servo->v_max;
		else if (distance < 0) servo->v_target = -(servo->v_max);
	}
	else 
	{
		servo->v_target = 0;
	}
	
	int16_t delta_v = servo->v_target - servo->v_current;
	int16_t step;
	
	if ((servo->v_current > 0 && delta_v > 0) || (servo->v_current < 0 && delta_v < 0) || (servo->v_current == 0)) // acceleration
	{
		step = servo->acceleration;
	}
	else
	{
		step = servo->deceleration;
	}
	
	if (labs(delta_v) <= step) servo->v_current = servo->v_target;
	else if (delta_v > 0) servo->v_current += step;
	else if (delta_v < 0) servo->v_current -= step;
	
	
	// 4. Cập nhật góc
	servo->current_angle = servo->current_angle + servo->v_current;
	// Deadband check
	if (labs(distance) < TO_FIXED(0.1) && labs(servo->v_current) < step)
	{
		servo->current_angle = servo->target_angle;
		servo->v_current = 0;
	}
	
	// 5. Xuất phần cứng chạy PWM
	App_Servo_UpdatePWM(servo);
}

void App_Servo_SetTarget(Servo_Handle_t *servo, uint32_t angle_q8)
{
	servo->target_angle = angle_q8;
}