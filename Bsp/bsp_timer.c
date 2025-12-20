#include "bsp_timer.h"

volatile uint32_t g_system_tick	= 0;
volatile uint8_t 10ms_flag = 0;
void BSP_Timer_Init(void)
{
	// Cấu hình chạy liên tục cho Timer A0 (System Tick 10ms)
	TACCR0 = 19999;
	TACTL = TASSEL_2 + MC_1 + TACLR + ID_3;
	TACCTL0 = CCIE; // cho phép ngắt CCR0
}


// Hàm để App lấy thời gian hiện tại (Get Tick)
uint32_t BSP_GetTick(void)
{
	uint32_t tick;
	// Tắt ngắt tạm thời để đọc an toàn
	__disable_interrupt();
	tick = g_system_tick;
	__enable_interrupt();
	return tick;
}

uint8_t BSP_10ms_Flag(void)
{
	if (10ms_flag == 1)
	{
		10ms_flag = 0;
		return 1;
	}
	return 0;
}

// Hàm ISR của Timer (Nhịp tim 10ms)
#pragma vector=TIMER0_A0_VECTOR
__interrupt	void timerA_ISR(void)
{
	// Tăng biến đếm tổng thời gian hệ thống
	g_system_tick++;
	
	10ms_flag = 1;
	
	static uint8_t tick_5ms = 0;
	tick_5ms++;
	if (tick_5ms >= 5)
	{
		tick_5ms = 0;
		BSP_Button_Driver_Scan_All();
	}
	
	// Đánh thức CPU 
	__bic_SR_register_on_exit(LPM0_bits);
}