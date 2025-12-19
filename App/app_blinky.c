#include "app_blinky.h"
#include "bsp_gpio.h"
#include <msp430.h>
#include "bsp_timer.h"

static BlinkyState_t currentState = STATE_IDLE;
static uint32_t lastBlinkTime = 0;

void App_Blinky_Init(void) 
{
	currentState = STATE_IDLE;
	BSP_LED_Off();
}

void App_Blinky_Run(void) 
{
	// Lấy thời gian hiện tại
	uint32_t currentTick = BSP_GetTick();
	
	// Logic chuyển trạng thái
	if (BSP_Event_IsButtonPressed())
	{
		switch (currentState)
		{
			case STATE_IDLE: currentState = STATE_SLOW; break;
			case STATE_SLOW: currentState = STATE_FAST; break;
			case STATE_FAST: currentState = STATE_IDLE; BSP_LED_Off(); break;
		}
	}
	
	// Logic nháy đèn (OUTPUT)
	switch (currentState)
	{
		case STATE_IDLE: break;
		case STATE_SLOW:
			// Nếu đã trôi qua 1s kể từ lần nháy cuối
			if (currentTick - lastBlinkTime >= 1000)
			{
				BSP_LED_Toggle();
				lastBlinkTime = currentTick; // Cập nhật mốc thời gian mới
			}
			break;
		case STATE_FAST:
			// Nếu đã trôi qua 100 ms
			if (currentTick - lastBlinkTime	>= 100)
			{
				BSP_LED_Toggle();
				lastBlinkTime = currentTick; // Cập nhật mốc thời gian
			}
			break;
	}
	
}	

