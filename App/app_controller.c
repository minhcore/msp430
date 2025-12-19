#include "app_controller.h"
#include "bsp_uart.h"
#include "bsp_pwm.h"
#include "app_console.h"

static Servo_Handle_t Pan_Servo;

void App_Controller_Init(void)
{
	BSP_UART_Init();
	BSP_PWM_Init(SERVO_ID_PAN);
	
	App_Servo_Init(&Pan_Servo, SERVO_ID_PAN);
	App_Console_Init();
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
		App_Servo_SetAngle(pTargetServo, angle);
	}
	
}