#ifndef APP_CONSOLE_H
#define APP_CONSOLE_H

#include <stdint.h>
#include <stdbool.h>
#include "bsp_uart.h"
#include "bsp_gpio.h" 



#define CMD_BUFFER_SIZE 32

void App_Console_Init(void);
void App_Console_Task(void);


// Định nghĩa một function pointer
typedef void (*App_Console_Callback_t)(const char* cmd, int32_t value);

void App_Console_RegisterCallback(App_Console_Callback_t cb);
#endif