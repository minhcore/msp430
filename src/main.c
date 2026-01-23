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
    uart_init();

    __enable_interrupt();
    while (1) {
        /*
        uart_put_char_polling('M');
        uart_put_char_polling('i');
        uart_put_char_polling('n');
        uart_put_char_polling('h');
*/

        uart_put_char_interrupt('M');
        uart_put_char_interrupt('i');
        uart_put_char_interrupt('n');
        uart_put_char_interrupt('h');
        uart_put_char_interrupt('\n');
        uart_put_char_interrupt('\r');
        BUSY_WAIT_ms(500);
    }
}