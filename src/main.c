#include <msp430.h>

#include "drivers/io.h"
#include "drivers/mcu_init.h"
#include "drivers/led.h"
#include "drivers/uart.h"

#include "common/assert_handler.h"
#include "common/defines.h"

#include "printf/printf.h"

int main(void)
{
    mcu_init();
    uart_init();

    while (1) {
        
    }
}