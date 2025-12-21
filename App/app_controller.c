#include "app_controller.h"
#include "bsp_uart.h"
#include "bsp_pwm.h"
#include "app_servo.h"
#include "app_console.h"
#include <string.h>

static Servo_Handle_t Pan_Servo;
static char buffer[64];

static void Int_to_String(int32_t n, char *buffer) {
	char *ptr = buffer;
	uint8_t signFlag = 0;
	if (n < 0) {
		n = -n;
		signFlag = 1;
	}
	do {
		int32_t digit = n % 10;
		char c = digit + '0';
		*ptr++ = c;
		n = n / 10;
	} while (n > 0);
	
	if (signFlag) *ptr++ = '-';
	
	*ptr = '\0';
	
	// Reverse strings
	char *start = buffer;
	char *end = ptr - 1;
	char temp;
	
	while (start < end) {
		temp = *start;
		*start = *end;
		*end = temp;
		
		start++;
		end--;
	}
	//return (int)(ptr - buffer);
}


void App_Controller_Init(void)
{
	BSP_UART_Init();
	BSP_PWM_Init(SERVO_ID_PAN);
	BSP_Timer_Init();
	
	App_Servo_Init(&Pan_Servo, SERVO_ID_PAN);
	Pan_Servo.acceleration = TO_FIXED(0.02);
	Pan_Servo.deceleration = TO_FIXED(0.004);
	Pan_Servo.v_max = TO_FIXED(1);
        Pan_Servo.target_angle = TO_FIXED(90);
	
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

void App_Send_Log_Data(void)
{
	Int_to_String(FROM_FIXED(Pan_Servo.v_current), buffer);
	BSP_UART_PutString("V = ");
	BSP_UART_PutString(buffer);
	BSP_UART_PutString("\n");
}

