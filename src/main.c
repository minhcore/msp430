#include <msp430.h>
#include "drivers/io.h"
#include "drivers/mcu_init.h"
#include "common/assert_handler.h"
#include "common/defines.h"
#include "drivers/led.h"

static void test_setup(void)
{
    mcu_init();
}
/*
static void test_assert(void)
{
    test_setup();
    ASSERT(0);
}
*/

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

int main(void)
{
    // test_assert();
    test_blink_led();
    return 0;
}