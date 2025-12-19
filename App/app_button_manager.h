#ifndef APP_BUTTON_MANAGER_H
#define APP_BUTTON_MANAGER_H

#include "bsp_button.h"
#include <stdint.h>

typedef enum {
	BUTTON_EVENT_NONE = 0,
	BUTTON_EVENT_PRESSED 
} Button_Event_t;

Button_Event_t App_Button_Manager_Process(void);

#endif