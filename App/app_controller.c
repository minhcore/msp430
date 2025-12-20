#include "app_controller.h"
#include "bsp_uart.h"
#include "bsp_pwm.h"
#include "app_servo.h"
#include "app_console.h"
#include <string.h>

static Servo_Handle_t Pan_Servo;

void App_Controller_Init(void)
{
	BSP_UART_Init();
	BSP_PWM_Init(SERVO_ID_PAN);
	
	App_Servo_Init(&Pan_Servo, SERVO_ID_PAN);
	Pan_Servo.acceleration = TO_FIXED(0.5);
	Pan_Servo.deceleration = TO_FIXED(0.5);
	Pan_Servo.v_max = TO_FIXED(1.5);
	
	App_Console_Init();
	
	App_Console_RegisterCallback(App_Controller_UpdateTarget);
}

void App_Controller_Task()
{
	App_Servo_Process(&Pan_Servo);
	
}

void App_Controller_UpdateTarget(const char* cmd, int32_t value)
{
	if (strcmp(cmd, "PAN") == 0)
	{
		App_Servo_SetTarget(&Pan_Servo, TO_FIXED(value));
	}
}

