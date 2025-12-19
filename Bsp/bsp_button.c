#include "bsp_button.h"

static Button_t Button0;

static void Button_Update_Logic(Button_t *button)
{
	button->history = button->history << 1;
	uint8_t current_value = ((P1IN & button->pinID) != 0) ? 1 : 0;
	button->history |= current_value;
	
	if (button->history == 0xFF) 
	{
		button->stable_state = 1; //release
	}
	else if (button->history == 0x00)
	{
		button->stable_state = 0; //press
	}
}

void BSP_Button_Init(void)
{
	Button0.pinID = BUTTON_PIN; // In gpio.h
	Button0.history = 0xFF;
	Button0.stable_state = 1;
	Button0.last_stable_state = 1;
}

void BSP_Button_Driver_Scan_All(void)
{
	Button_Update_Logic(&Button0);
}

uint8_t BSP_Button_Press_Event(void)
{
	uint8_t event = 0;
	if (Button0.last_stable_state == 1 && Button0.stable_state == 0)
	{
		event = 1;
	}
	
	Button0.last_stable_state = Button0.stable_state;
	
	return event;
}




