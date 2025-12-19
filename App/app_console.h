#ifndef APP_CONSOLE_H
#define APP_CONSOLE_H

#include <stdint.h>
#include <stdbool.h>
#include "bsp_uart.h"
#include "bsp_gpio.h" 



#define CMD_BUFFER_SIZE 32

void App_Console_Init(void);
void App_Console_Task(void);

#endif