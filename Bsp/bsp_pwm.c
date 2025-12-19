#include "bsp_pwm.h"

void BSP_PWM_Init(Servo_ID_t servo_id)
{
	switch (servo_id)
	{
		case SERVO_ID_PAN:
			// Cấu hình PWM cho Timer A1 
			P2DIR |= PAN_PIN;
			P2SEL |= PAN_PIN;
			P2SEL2 &= ~PAN_PIN;
			TA1CCR0 = PWM_PERIOD_COUNT;
			TA1CCTL1 = OUTMOD_7; // Reset/ Set
			TA1CCR1 = 0;
			TA1CTL = TASSEL_2 + MC_1 + TACLR + ID_3;
			break;
		
		case SERVO_ID_TILT:
			break;
			
		default:
			break;
	}
}

void BSP_PWM_SetDuty(uint8_t percent) 
{
	if (percent > 100) percent = 100;
	
	uint16_t duty_counts = percent * 400;
	
	TA1CCR1 = duty_counts;
}

void BSP_PWM_SetPulse_Us(Servo_ID_t servo_id, uint16_t us_value)
{
	switch(servo_id)
	{
		case SERVO_ID_PAN:
			if (us_value > 20000) us_value = 20000;
	
			uint16_t duty_counts = 2 * us_value;
	
			TA1CCR1 = duty_counts;
			
			break;
			
		case SERVO_ID_TILT:
			break;
	}
}