#ifndef APP_SERVO_H
#define APP_SERVO_H

#include "bsp_pwm.h"
#include <stdint.h>

#define SHIFT_AMOUNT 8 // Dùng cho Qx.8
#define TO_FIXED(x) ((int32_t)((x) * (1 << SHIFT_AMOUNT)))
#define FROM_FIXED(x) ((x) >> SHIFT_AMOUNT)

typedef struct 
{
	uint16_t min_pulse_us;
	uint16_t max_pulse_us;
	uint16_t max_angle;
	uint16_t min_angle;
	Servo_ID_t pwm_channel_id;
	
	int32_t current_angle; // pwm set Q24.8
	int32_t target_angle; // uart set Q24.8
	int16_t v_current; // đơn vị: đơn vị code / chu kỳ timer, Q8.8
	int16_t v_target;
	int16_t v_max; // Q8.8
	int16_t acceleration; // Q8.8
	int16_t deceleration; // Q8.8
} Servo_Handle_t;

void App_Servo_Init(Servo_Handle_t *servo, Servo_ID_t pwm_channel);

void App_Servo_SetAngle(Servo_Handle_t *servo, uint8_t angle);

void App_Servo_UpdatePWM(Servo_Handle_t *servo);

void App_Servo_Process(Servo_Handle_t *servo);

void App_Servo_SetTarget(Servo_Handle_t *servo, uint32_t angle);

#endif