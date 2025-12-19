#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#include <stdio.h>
#include "app_servo.h"


void App_Controller_Init(void);

void App_Servo_Controller(Servo_ID_t servo_id, uint8_t angle);

#endif