#include "app_controller.h"
#include "bsp_uart.h"
#include "bsp_pwm.h"
#include "app_console.h"
#include <string.h>

static Servo_Handle_t Pan_Servo;

void App_Controller_Init(void)
{
	BSP_UART_Init();
	BSP_PWM_Init(SERVO_ID_PAN);
	
	App_Servo_Init(&Pan_Servo, SERVO_ID_PAN);
	App_Console_Init();
	
	App_Console_RegisterCallback(MyController_HandleConsole);
}

void App_Servo_Controller(Servo_ID_t servo_id, uint8_t angle)
{
	Servo_Handle_t *pTargetServo = NULL;
	
	// Mapping
	
	switch (servo_id)
	{
		case SERVO_ID_PAN:
			pTargetServo = &Pan_Servo;
			break;
			
		default:
			return;
	}
	
	if (pTargetServo != NULL)
	{
		if (BSP_10ms_Flag)
		App_Servo_Process(pTargetServo);
	}
	
}

void App_MyController_HandleConsole(const char* cmd, int32_t value)
{
	if (strcpy(cmd, "PAN") == 0)
	{
		App_Servo_SetTarget(&Pan_Servo, value);
	}
}