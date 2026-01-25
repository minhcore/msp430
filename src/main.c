#include <msp430.h>

#include "drivers/io.h"
#include "drivers/mcu_init.h"
#include "drivers/led.h"
#include "drivers/uart.h"

#include "common/assert_handler.h"
#include "common/defines.h"

int main(void)
{
    mcu_init();

    while (1) {
        _putchar('D');
        _putchar('a');
        _putchar('n');
        _putchar('g');
        _putchar('M');
        _putchar('\n');
        _putchar('\r');
        BUSY_WAIT_ms(500);
    }
}