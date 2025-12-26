#include "bsp_timer.h"

volatile uint32_t g_system_tick	= 0;
static volatile uint8_t ten_ms_flag = 0;
void BSP_Timer_Init(void)
{
	
	TACCR0 = 19999;
	TACTL = TASSEL_2 + MC_1 + TACLR + ID_3;
	TACCTL0 = CCIE; 
}



uint32_t BSP_GetTick(void)
{
	uint32_t tick;
	
	__disable_interrupt();
	tick = g_system_tick;
	__enable_interrupt();
	return tick;
}

uint8_t BSP_10ms_Flag(void)
{
	uint8_t status;
	
	__disable_interrupt();
	status = ten_ms_flag;
	ten_ms_flag = 0;
	__enable_interrupt();
	
	return status;
}


#pragma vector=TIMER0_A0_VECTOR
__interrupt	void timerA_ISR(void)
{
	
	g_system_tick++;
	
	ten_ms_flag = 1;
	
	static uint8_t tick_5ms = 0;
	tick_5ms++;
	if (tick_5ms >= 5)
	{
		tick_5ms = 0;
		//BSP_Button_Driver_Scan_All();
	}
	
	
	__bic_SR_register_on_exit(LPM0_bits);
}
