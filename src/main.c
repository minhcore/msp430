#include <msp430.h>
#include "drivers/io.h"
#include "drivers/mcu_init.h"
#include "common/assert_handler.h"
#include "common/defines.h"
#include "drivers/led.h"

int main(void)
{
    test_blink_led();
    return 0;
}