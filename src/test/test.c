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

SUPPRESS_UNUSED
static void io_11_isr(void)
{
    led_set(LED_TEST, LED_STATE_ON);
}

SUPPRESS_UNUSED
static void io_20_isr(void)
{
    led_set(LED_TEST, LED_STATE_OFF);
}

SUPPRESS_UNUSED
static void test_io_interrupt(void)
{
    test_setup();
    const struct io_config input_config = {
        .select = IO_SELECT_GPIO,
        .resistor = IO_RESISTOR_ENABLED,
        .direction = IO_DIR_INPUT,
        .out = IO_OUT_HIGH // pull-up
    };
    io_configure(IO_11, &input_config);
    io_configure(IO_20, &input_config);
    led_init();
    io_configure_interrupt(IO_11, IO_TRIGGER_FALLING, io_11_isr);
    io_configure_interrupt(IO_20, IO_TRIGGER_FALLING, io_20_isr);
    io_enable_interrupt(IO_11);
    io_enable_interrupt(IO_20);
    while(1);
}

int main ()  
{
    TEST();
    ASSERT(0);
    return 0;
}