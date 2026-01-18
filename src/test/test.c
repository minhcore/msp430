#include "drivers/io.h"
#include "drivers/mcu_init.h"
#include "drivers/led.h"

#include "common/assert_handler.h"
#include "common/defines.h"

#include <msp430.h>

SUPPRESS_UNUSED
static void test_setup(void)
{
    mcu_init();
}

SUPPRESS_UNUSED
static void test_assert(void)
{
    test_setup();
    ASSERT(0);
}

SUPPRESS_UNUSED
static void test_blink_led(void)
{
    test_setup();
    led_init();
    led_state_e led_state = LED_STATE_OFF;
    while (1) {
        led_state = (led_state == LED_STATE_OFF) ? LED_STATE_ON : LED_STATE_OFF;
        led_set(LED_TEST, led_state);
        BUSY_WAIT_ms(2000);
    }
}

int main () 
{
    TEST();
    ASSERT(0);
    return 0;
}