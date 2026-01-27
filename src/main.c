#include <msp430.h>
#include <stdint.h>

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
    uint8_t buffer[16];
    uint8_t count = 0;

    while (count < 16) {
        if (uart_get_char((char*)&buffer[count])) {
            _putchar(buffer[count]);
            count++;
        }
    }
        printf("\nNumber:\n");
        for (uint8_t k = 0; k < 16; k++) {
            _putchar(buffer[k]);
        }
    while (1) {
    }
}