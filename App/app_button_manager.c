#include "app_button_manager.h"

Button_Event_t App_Button_Manager_Process(void)
{
	if (BSP_Button_Press_Event() == 1)
	{
		return BUTTON_EVENT_PRESSED;
	}
	return BUTTON_EVENT_NONE;
}