#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#include <stdio.h>
#include "app_servo.h"


void App_Controller_Init(void);

void App_Controller_Task();

void App_Controller_UpdateTarget(const char * cmd, int32_t value);

void App_Send_Log_Data(void);

#endif